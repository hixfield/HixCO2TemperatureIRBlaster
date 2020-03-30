#include "HixWebServer.h"
#include "secret.h"
#include <FS.h>

HixWebServer::HixWebServer(HixConfig & config) : ESP8266WebServer(80),
                                                 m_config(config) {
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
    char buf[1024];
    Serial.println("handleFileRead: " + path);
    //check for postback of config data
    if (path.endsWith("/postconfig")) {
        handlePostConfig();
        return true;
    }
    // If a folder is requested, send the index file
    if (path.endsWith("/")) path += "index.html";
    // Get the MIME type
    String contentType = getContentType(path);
    if (SPIFFS.exists(path)) {
        File file = SPIFFS.open(path, "r");
        if (shouldReplacePlaceholders(path)) {
            //read file in memory and...
            size_t size = file.readBytes(buf, sizeof(buf));
            //...zero terminate
            buf[size] = 0;
            //create string of it
            String contents(buf);
            m_config.replacePlaceholders(contents);
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
    if (arg("password") != Secret::CONFIGPWD) {
        send(400, "text/plain", "Bad password");
        return false;
    }
    //handle parameters
    if (hasArg("mqtt_server")) m_config.setMQTTServer(arg("mqtt_server").c_str());
    if (hasArg("room")) m_config.setRoom(arg("room").c_str());
    if (hasArg("device_tag")) m_config.setDeviceTag(arg("device_tag").c_str());
    m_config.setOTAEnabled(hasArg("ota_enabled"));
    //write to epprom
    m_config.commitToEEPROM();
    //send reply
    const char * szSavedHtml = "/saved.html";
    File file = SPIFFS.open(szSavedHtml, "r");
    streamFile(file, getContentType(String(szSavedHtml)));
    file.close();
    //reset!
    delay(1000);
    ESP.reset();
    return true;
}