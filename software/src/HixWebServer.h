#ifndef HixWebServdr_h
#define HixWebServdr_h

#include "HixConfig.h"
#include <ESP8266WebServer.h>
#include <MHZ19.h>

class HixWebServer : public ESP8266WebServer {
  private:
    HixConfig & m_config;
    MHZ19 &     m_mhz19;
    bool        handleFileRead(String path);
    String      getContentType(String filename);
    bool        shouldReplacePlaceholders(String filename);
    void        replacePlaceholdersMHZ19(String & contents);
    //takes posted config and stores to eeprom
    bool handlePostConfig(void);
    //calibrates the co2 sensor
    bool handleCalibrate(void);

  public:
    HixWebServer(HixConfig & config, MHZ19 & mhz19);
};

#endif
