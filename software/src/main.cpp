#include "HixConfig.h"
#include "HixDisplay.h"
#include "HixMQTT.h"
#include "HixWebServer.h"
#include "secret.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <DS18B20Temperature.h>
#include <HixPinDigitalOutput.h>
#include <HixTimeout.h>
#include <IRac.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRtext.h>
#include <IRutils.h>
#include <MHZ19.h>
#include <SoftwareSerial.h>
#include <ir_Samsung.h>

   
//connected devices and software modules
HixConfig           g_config;
HixTimeout          g_sampler(1000, true);
HixTimeout          g_logger(5000, true);
HixDisplay          g_display;
HixPinDigitalOutput g_beeper(2);
DS18B20Temperature  g_temperature(14);
Adafruit_NeoPixel   g_rgbLed = Adafruit_NeoPixel(1, 0, NEO_RGB + NEO_KHZ400);
MHZ19               g_mhz19;
SoftwareSerial      g_mhz19Serial(13, 16);
HixWebServer        g_webServer(g_config);
IRSamsungAc         g_IRTransmitter(15);
//disable IR Receiving seams to make firmware more stable
//seam to be getting random resets when enabled...
#ifdef IRIN_ENABLED
IRrecv g_IRReciever(12, 1024, 40, true);
#endif
//global variables
float g_fCurrentTemp = 0;
int   g_nCurrentCO2  = 0;
int   g_nCurrentRSSI = 0;
bool  g_bLoopToggle  = false;
bool  g_bACIsOn      = false;

HixMQTT g_mqtt(Secret::WIFI_SSID,
               Secret::WIFI_PWD,
               g_config.getMQTTServer(),
               g_config.getDeviceType(),
               g_config.getDeviceVersion(),
               g_config.getRoom(),
               g_config.getDeviceTag());

enum Color : uint32_t {
    black  = 0x000000,
    white  = 0xFFFFFF,
    red    = 0xFF0000,
    green  = 0x00FF00,
    blue   = 0x0000FF,
    orange = 0xFF8C00
};

//////////////////////////////////////////////////////////////////////////////////
// Helper functions
//////////////////////////////////////////////////////////////////////////////////

void resetWithMessage(const char * szMessage) {
    Serial.println(szMessage);
    delay(2000);
    ESP.reset();
}

void configureOTA() {
    Serial.println("Configuring OTA, my hostname:");
    Serial.println(g_mqtt.getMqttClientName());
    ArduinoOTA.setHostname(g_mqtt.getMqttClientName());
    ArduinoOTA.setPort(8266);
    //setup handlers
    ArduinoOTA.onStart([]() {
        Serial.println("OTA -> Start");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("OTA -> End");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("OTA -> Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA -> Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
            Serial.println("OTA -> Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
            Serial.println("OTA -> Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
            Serial.println("OTA -> Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
            Serial.println("OTA -> Receive Failed");
        else if (error == OTA_END_ERROR)
            Serial.println("OTA -> End Failed");
    });
    ArduinoOTA.begin();
}

void setLedColor(Color color, u32_t nDimFactor = 16) {
    //make base colors
    u32_t nR = color >> 16;
    u32_t nG = (color & 0x00FF00) >> 8;
    u32_t nB = color & 0x0000FF;
    //take dimming into account
    nR /= nDimFactor;
    nG /= nDimFactor;
    nB /= nDimFactor;
    //reassemble
    u32_t nDimmedColor = (nR << 16) | (nG << 8) | nB;
    g_rgbLed.setPixelColor(0, nDimmedColor);
    g_rgbLed.show();
}

void setLedColorForCO2(int nCO2ppm) {
    if (nCO2ppm < 800) {
        setLedColor(Color::green);
        return;
    }
    if (nCO2ppm < 1200) {
        setLedColor(Color::orange);
        return;
    }
    setLedColor(Color::red);
}

void printACState() {
    Serial.println("Samsung A/C remote is in the following state:");
    Serial.printf("  %s\n", g_IRTransmitter.toString().c_str());
}

void AC_On(int nTemperature) {
    Serial.print("AC ON to ");
    Serial.print(nTemperature);
    Serial.println(" C");
    //to avoid detecting our own transmitted signal, switch off detector first
#ifdef IRIN_ENABLED
    g_IRReciever.disableIRIn();
#endif
    //transmit our commands
    g_IRTransmitter.on();
    g_IRTransmitter.setFan(kSamsungAcFanAuto);
    g_IRTransmitter.setMode(kSamsungAcAuto);
    g_IRTransmitter.setTemp(nTemperature);
    g_IRTransmitter.setSwing(true);
    g_IRTransmitter.send();
    //re-enable our IR detector
#ifdef IRIN_ENABLED
//g_IRReciever.enableIRIn();
#endif
    //keep our state
    g_bACIsOn = true;
}


void AC_Off(void) {
    Serial.print("AC OFF");
    //to avoid detecting our own transmitted signal, switch off detector first
#ifdef IRIN_ENABLED
    g_IRReciever.disableIRIn();
#endif
    //transmit our commands
    g_IRTransmitter.off();
    g_IRTransmitter.send();
    //re-enable our IR detector
    //g_IRReciever.enableIRIn();
    //keep our state
    g_bACIsOn = false;
}

void AC_toggle(int nTemperature) {
    if (g_bACIsOn)
        AC_Off();
    else
        AC_On(nTemperature);
}

bool handleIRCommand(decode_results results) {
    switch (results.value) {
        //Telenet IR : play button
    case 0x48C6EAFF:
        AC_On(28);
        return true;
        //Telenet IR : pause button
    case 0xAA33049:
        AC_Off();
        return true;
    default:
        Serial.print("Unrecognized code received ");
        Serial.println(resultToHexidecimal(&results));
    }
    //nothing handled
    return false;
}

bool checkIR(void) {
    decode_results results;
    //Check if the IR code has been received.
#ifdef IRIN_ENABLED
    if (g_IRReciever.decode(&results)) {
        // Check if we got an IR message that was to big for our capture buffer.
        if (results.overflow) Serial.println("Error IR capture buffer overflow");
        // Display the basic output of what we found.
        Serial.print("IR Received: ");
        Serial.println(resultToHexidecimal(&results));
        //did find something!
        return handleIRCommand(results);
    }
#endif
    //return not found
    return false;
}

void selfTest(void) {
    Serial.println(F("Running self test..."));
    //display version while running the self test...
    Serial.println(F("Showing version on display"));
    g_display.drawDisplayVersion(g_config.getDeviceType(), g_config.getDeviceVersion());
    //show colors on led
    Serial.println(F("Showing green-red-oragne-blue-white on LED"));
    setLedColor(Color::green);
    delay(300);
    setLedColor(Color::red);
    delay(300);
    setLedColor(Color::orange);
    delay(300);
    setLedColor(Color::blue);
    delay(300);
    setLedColor(Color::white);
    //beep our led 5 times
    Serial.println(F("Beeping 5 times"));
    g_beeper.blink(true, 5, 100);
    //switch off airco
    Serial.println(F("Setting AC off"));
    AC_Off();
    //all done
    Serial.println(F("Selftest complate"));
}

//////////////////////////////////////////////////////////////////////////////////
// Setup
//////////////////////////////////////////////////////////////////////////////////

void setup() {
    Serial.begin(115200);
    Serial.print(F("Startup "));
    Serial.print(g_config.getDeviceType());
    Serial.print(F(" "));
    Serial.println(g_config.getDeviceVersion());
    //increment our counter
    g_config.incrementNumberOfBootUps();
    Serial.print(F("Number of bootups: "));
    Serial.println(g_config.getNumberOfBootUps());
    //disconnect WiFi -> seams to help for bug that after upload wifi does not want to connect again...
    Serial.println(F("Disconnecting WIFI"));
    WiFi.disconnect();
    // setup display
    Serial.println(F("Setting up display"));
    if (!g_display.begin()) resetWithMessage("SSD1306 allocation failed, resetting");
    g_display.clearDisplay();
    g_display.display();
    //setup beeper
    Serial.println(F("Setup beeper"));
    g_beeper.begin();
    //setup DS18B20
    Serial.println(F("Setup DS18B20"));
    if (!g_temperature.begin()) {
        resetWithMessage("Could not locate DS18B20");
    }
    //setup RGB let
    Serial.println(F("Setup RDB LED"));
    g_rgbLed.begin();
    setLedColor(Color::black);
    // setup CO2 sensor
    Serial.println(F("Setting up CO2 sensor"));
    g_mhz19Serial.begin(9600);
    g_mhz19.begin(g_mhz19Serial);
    g_mhz19.setRange(5000);
    g_mhz19.autoCalibration();
    //configure ir transmitter
    Serial.println("Setting up IR Transmitter");
    g_IRTransmitter.begin();
    //configure receiver
#ifdef IRIN_ENABLED
    Serial.println("Setting up IR Receiver");
    g_IRReciever.setUnknownThreshold(12);
    g_IRReciever.enableIRIn();
#endif
    // configure MQTT
    Serial.println(F("Setting up MQTT"));
    if (!g_mqtt.begin()) resetWithMessage("MQTT allocation failed, resetting");
    //setup SPIFFS
    Serial.println(F("Setting up SPIFFS"));
    if (!SPIFFS.begin()) resetWithMessage("SPIFFS initialization failed, resetting");
    //setup the server
    Serial.println(F("Setting up web server"));
    g_webServer.begin();
    //run selftest if required
    if (g_config.getSelfTestEnabled()) {
        selfTest();
    }
    //all done
    Serial.println(F("Setup complete"));
}

//////////////////////////////////////////////////////////////////////////////////
// Loop
//////////////////////////////////////////////////////////////////////////////////

void loop() {
    //other loop functions
    g_mqtt.loop();
    g_webServer.handleClient();
    ArduinoOTA.handle();
    checkIR();
    //my own processing
    if (g_sampler.isExpired(true)) {
        g_bLoopToggle = !g_bLoopToggle;
        // load CO2 and check error condition
        int nTempCO2 = g_mhz19.getCO2();
        //only use the new value if there was no error
        if (g_mhz19.errorCode == RESULT_OK) {
            g_nCurrentCO2 = nTempCO2;
        }
        //read other sensor values
        g_fCurrentTemp = g_temperature.getTemp();
        g_nCurrentRSSI = WiFi.RSSI();
        //set led
        if (g_config.getLEDEnabeled()) {
            if (g_mqtt.isConnected()) {
                setLedColorForCO2(g_nCurrentCO2);
            } else {
                setLedColor(Color::blue);
            }
        } else {
            setLedColor(Color::black);
        }
        //show on display
        if (g_config.getOLEDEnabled()) {
            g_display.showStatus(g_fCurrentTemp,
                                 g_nCurrentCO2,
                                 g_nCurrentRSSI,
                                 g_bLoopToggle);
        }
        // log to serial
        Serial.print(g_fCurrentTemp);
        Serial.print(F(" C ; "));
        Serial.print(g_nCurrentCO2);
        Serial.print(F(" PPM"));
        Serial.println();
    }
    if (g_logger.isExpired(true)) {
        g_mqtt.publishStatusValues(g_nCurrentCO2, g_fCurrentTemp);
    }
}

//////////////////////////////////////////////////////////////////////////////////
// Required by the MQTT library
//////////////////////////////////////////////////////////////////////////////////

void onConnectionEstablished() {
    //setup OTA
    if (g_config.getOTAEnabled()) {
        configureOTA();
    } else {
        Serial.println("OTA is disabled");
    }
    //plushing values
    g_mqtt.publishDeviceValues();
}
