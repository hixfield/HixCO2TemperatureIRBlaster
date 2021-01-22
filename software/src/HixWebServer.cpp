#include "HixWebServer.h"
#include "secret.h"
#include <FS.h>

HixWebServer::HixWebServer(HixConfig & config, MHZ19 & mhz19) : ESP8266WebServer(80),
                                                                m_config(config),
                                                                m_mhz19(mhz19) {
    onNotFound([this]() {
        if (!handleFileRead(uri()))
            send(404, "text/plain", "404: Not Found");
    });
}

String HixWebServer::getContentType(String filename) {
    if (filename.endsWith(".htm"))
        return "text/html";
    else if (filename.endsWith(".html"))
        return "text/html";
    else if (filename.endsWith(".css"))
        return "text/css";
    else if (filename.endsWith(".js"))
        return "application/javascript";
    else if (filename.endsWith(".png"))
        return "image/png";
    else if (filename.endsWith(".gif"))
        return "image/gif";
    else if (filename.endsWith(".jpg"))
        return "image/jpeg";
    else if (filename.endsWith(".ico"))
        return "image/x-icon";
    else if (filename.endsWith(".xml"))
        return "text/xml";
    else if (filename.endsWith(".pdf"))
        return "application/x-pdf";
    else if (filename.endsWith(".zip"))
        return "application/x-zip";
    else if (filename.endsWith(".gz"))
        return "application/x-gzip";
    return "text/plain";
}

bool HixWebServer::shouldReplacePlaceholders(String filename) {
    if (filename.endsWith(".htm")) return true;
    if (filename.endsWith(".html")) return true;
    return false;
}

// send the right file to the client (if it exists)
bool HixWebServer::handleFileRead(String path) {
    Serial.println("handleFileRead: " + path);
    //check for postback of config data
    if (path.endsWith("/postconfig")) {
        handlePostConfig();
        return true;
    }
    //check calibration of data...
    if (path.endsWith("/calibrate")) {
        handleCalibrate();
        return true;
    }
    // If a folder is requested, send the index file
    if (path.endsWith("/")) path += "index.html";
    // Get the MIME type
    String contentType = getContentType(path);
    if (SPIFFS.exists(path)) {
        File file = SPIFFS.open(path, "r");
        if (shouldReplacePlaceholders(path)) {
            String contents = file.readString();
            m_config.replacePlaceholders(contents);
            replacePlaceholdersMHZ19(contents);
            //send it
            send(200, contentType, contents);
        } else {
            streamFile(file, contentType);
        }
        //close our input file
        file.close();
        //return non error
        return true;
    }
    //file not found
    return false;
}


bool HixWebServer::handlePostConfig(void) {
    //check password
    if (arg("password") != Secret::CONFIG_PWD) {
        send(400, "text/plain", "Bad password");
        return false;
    }
    //handle parameters
    if (hasArg("nbr_bootups")) m_config.setNumberOfBootUps(arg("nbr_bootups").toInt());
    if (hasArg("mqtt_server")) m_config.setMQTTServer(arg("mqtt_server").c_str());
    if (hasArg("room")) m_config.setRoom(arg("room").c_str());
    if (hasArg("device_tag")) m_config.setDeviceTag(arg("device_tag").c_str());
    if (hasArg("scale_corr_factor")) m_config.setScaleCorrectFactor(arg("scale_corr_factor").toInt());
    m_config.setOTAEnabled(hasArg("ota_enabled"));
    m_config.setLEDEnabled(hasArg("led_enabled"));
    m_config.setOLEDEnabled(hasArg("oled_enabled"));
    m_config.setSelfTestEnabled(hasArg("selftest_enabled"));
    m_config.setAutoBackgroundCalibrationEnabled(hasArg("abc_enabled"));
    //write to epprom
    m_config.commitToEEPROM();
    //send reply
    const char * szSavedHtml = "/saved.html";
    File         file        = SPIFFS.open(szSavedHtml, "r");
    streamFile(file, getContentType(String(szSavedHtml)));
    file.close();
    //reset!
    delay(1000);
    ESP.reset();
    return true;
}

bool HixWebServer::handleCalibrate(void) {
    //check password
    if (arg("password") != Secret::CONFIG_PWD) {
        send(400, "text/plain", "Bad password");
        return false;
    }
    //calibrate
    m_mhz19.calibrate();
    //send reply
    const char * szSavedHtml = "/calibrate.html";
    File         file        = SPIFFS.open(szSavedHtml, "r");
    streamFile(file, getContentType(String(szSavedHtml)));
    file.close();
    //return non error
    return true;
}

void HixWebServer::replacePlaceholdersMHZ19(String & contents) {
    //extract version
    char versionArray[4];
    m_mhz19.getVersion(versionArray);
    String version;
    version += versionArray[0];
    version += versionArray[1];
    version += '.';
    version += versionArray[2];
    version += versionArray[3];
    //replace different parameters
    contents.replace("||MHZ19_FIRMWARE||", version);
    contents.replace("||MHZ19_CO2||", String(m_mhz19.getCO2()));
    contents.replace("||MHZ19_CO2_CORR||", String(m_config.rescaleCO2Value(m_mhz19.getCO2())));    
    contents.replace("||MHZ19_TRANSMITTANCE||", String(m_mhz19.getTransmittance()));
    contents.replace("||MHZ19_TEMPERATURE||", String(m_mhz19.getTemperature()));
    contents.replace("||MHZ19_ACCURACY||", String(m_mhz19.getAccuracy()));
    contents.replace("||MHZ19_RANGE||", String(m_mhz19.getRange()));
    contents.replace("||MHZ19_BACKGROUNDCO2||", String(m_mhz19.getBackgroundCO2()));
    contents.replace("||MHZ19_TEMPERATURECAL||", String(m_mhz19.getTempAdjustment()));
    contents.replace("||MHZ19_ABC||", m_mhz19.getABC() ? "ON" : "OFF");
}
