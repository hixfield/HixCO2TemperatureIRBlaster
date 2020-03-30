#include "HixConfig.h"
#include "HixDisplay.h"
#include "HixMQTT.h"
#include "HixWebServer.h"
#include "secret.h"
#include <ArduinoOTA.h>
#include <FS.h>
#include <HixTimeout.h>
#include <MHZ19.h>
#include <SoftwareSerial.h>

// runtime global variables
HixConfig      g_config;
MHZ19          g_mhz19;
SoftwareSerial g_mhz19Serial(D5, D6);
HixDisplay     g_display;
HixTimeout     g_sampler(1000, true);
HixTimeout     g_logger(5000, true);
float          g_fCurrentTemp = 0;
int            g_nCurrentCO2  = 0;
int            g_nCurrentRSSI = 0;
bool           g_bLoopToggle  = false;
HixWebServer   g_webServer(g_config);

HixMQTT g_mqtt(Secret::WIFI_SSID,
               Secret::WIFI_PWD,
               g_config.getMQTTServer(),
               g_config.getDeviceType(),
               g_config.getDeviceVersion(),
               g_config.getRoom(),
               g_config.getDeviceTag());


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

//////////////////////////////////////////////////////////////////////////////////
// Setup
//////////////////////////////////////////////////////////////////////////////////

void setup() {
    Serial.begin(115200);
    Serial.print(F("Startup "));
    Serial.print(g_config.getDeviceType());
    Serial.print(F(" "));
    Serial.println(g_config.getDeviceVersion());
    //disconnect WiFi -> seams to help for bug that after upload wifi does not want to connect again...
    Serial.println(F("Disconnecting WIFI"));
    WiFi.disconnect();
    // setup display
    Serial.println(F("Setting up display"));
    if (!g_display.begin()) resetWithMessage("SSD1306 allocation failed, resetting");
    g_display.drawDisplayVersion(g_config.getDeviceType(), g_config.getDeviceVersion());
    delay(2000);
    // setup CO2 sensor
    Serial.println(F("Setting up CO2 sensor"));
    g_mhz19Serial.begin(9600);
    g_mhz19.begin(g_mhz19Serial);
    g_mhz19.setRange(5000);
    g_mhz19.autoCalibration();
    // configure MQTT
    Serial.println(F("Setting up MQTT"));
    if (!g_mqtt.begin()) resetWithMessage("MQTT allocation failed, resetting");
    //setup SPIFFS
    Serial.println(F("Setting up SPIFFS"));
    if (!SPIFFS.begin()) resetWithMessage("SPIFFS initialization failed, resetting");
    //setup the server
    Serial.println(F("Setting up web server"));
    g_webServer.begin();
    // all done
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
    //my own processing
    if (g_sampler.isExpired(true)) {
        g_bLoopToggle = !g_bLoopToggle;
        // load sensor values
        g_fCurrentTemp = g_mhz19.getTemperature(true);
        g_nCurrentCO2  = g_mhz19.getCO2();
        g_nCurrentRSSI = WiFi.RSSI();
        g_display.showStatus(g_fCurrentTemp,
                             g_nCurrentCO2,
                             g_nCurrentRSSI,
                             g_bLoopToggle);
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
