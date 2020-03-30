#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
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

// runtime global variables
HixTimeout          g_sampler(3000, true);
HixPinDigitalOutput g_beeper(2);
DS18B20Temperature  g_temperature(14);
Adafruit_NeoPixel   g_rgbLed = Adafruit_NeoPixel(1, 0, NEO_RGB + NEO_KHZ400);
MHZ19               g_mhz19;
SoftwareSerial      g_mhz19Serial(13, 16);
IRSamsungAc         g_IRTransmitter(15);
IRrecv              g_IRReciever(12, 1024, 40, true);
bool                g_bACIsOn = false;

//////////////////////////////////////////////////////////////////////////////////
// Helper functions
//////////////////////////////////////////////////////////////////////////////////

void resetWithMessage(const char * szMessage) {
    Serial.println(szMessage);
    delay(2000);
    ESP.reset();
}

void setLedColor(uint32_t color) {
    g_rgbLed.setPixelColor(0, color);
    g_rgbLed.show();
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
    g_IRReciever.disableIRIn();
    //transmit our commands
    g_IRTransmitter.on();
    g_IRTransmitter.setFan(kSamsungAcFanAuto);
    g_IRTransmitter.setMode(kSamsungAcAuto);
    g_IRTransmitter.setTemp(nTemperature);
    g_IRTransmitter.setSwing(true);
    g_IRTransmitter.send();
    //re-enable our IR detector
    g_IRReciever.enableIRIn();
    //keep our state
    g_bACIsOn = true;
}


void AC_Off(void) {
    Serial.print("AC OFF");
    //to avoid detecting our own transmitted signal, switch off detector first
    g_IRReciever.disableIRIn();
    //transmit our commands
    g_IRTransmitter.off();
    g_IRTransmitter.send();
    //re-enable our IR detector
    g_IRReciever.enableIRIn();
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
    // Check if the IR code has been received.
    if (g_IRReciever.decode(&results)) {
        // Check if we got an IR message that was to big for our capture buffer.
        if (results.overflow) Serial.println("Error IR capture buffer overflow");
        // Display the basic output of what we found.
        Serial.print("IR Received: ");
        Serial.println(resultToHexidecimal(&results));
        //did find something!
        return handleIRCommand(results);
    }
    //return not found
    return false;
}


//////////////////////////////////////////////////////////////////////////////////
// Setup
//////////////////////////////////////////////////////////////////////////////////

void setup() {
    Serial.begin(115200);
    Serial.println(F("Startup "));
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
    setLedColor(0xFF0000);
    delay(300);
    setLedColor(0x00FF00);
    delay(300);
    setLedColor(0x0000FF);
    delay(300);
    setLedColor(0x000000);
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
    Serial.println("Setting up IR Receiver");
    g_IRReciever.setUnknownThreshold(12);
    g_IRReciever.enableIRIn();
    //all done
    g_beeper.blink(true, 5, 100);
    Serial.println(F("Setup complete"));
}

//////////////////////////////////////////////////////////////////////////////////
// Loop
//////////////////////////////////////////////////////////////////////////////////

void loop() {
    if (g_sampler.isExpired(true)) {
        Serial.print(g_temperature.getTemp());
        Serial.print("\t");
        Serial.println(g_mhz19.getCO2());
        AC_toggle(23);
    }
    checkIR();
}
