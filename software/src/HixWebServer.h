#ifndef HixWebServdr_h
#define HixWebServdr_h

#include "HixConfig.h"
#include <ESP8266WebServer.h>

class HixWebServer : public ESP8266WebServer {
  private:
    HixConfig & m_config;
    bool        handleFileRead(String path);
    String      getContentType(String filename);
    bool        shouldReplacePlaceholders(String filename);
    //takes posted config and stores to eeprom
    bool handlePostConfig(void);

  public:
    HixWebServer(HixConfig & config);
};

#endif
