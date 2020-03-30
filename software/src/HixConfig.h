#ifndef HixConfig_h
#define HixConfig_h

#include <arduino.h>

class HixConfig {
  private:
    //base address for epprom
    const int m_nEPPROMBaseAddress = 0;
    //hilds the data
    struct {
        char          szMQTTServer[50];
        char          szRoom[50];
        char          szDeviceTag[50];
        bool          bOTAEnabled;
        unsigned long crc;
    } data;
    //determine crc
    unsigned long calculateCRC(void);
    //store defaults
    void commitDefaults(void);

  public:
    HixConfig();
    //getters
    const char * getDeviceType(void) { return "HixCO2"; };
    const char * getDeviceVersion(void) { return "1.8.0"; };
    const char * getMQTTServer(void);
    const char * getRoom(void);
    const char * getDeviceTag(void);
    bool         getOTAEnabled(void);
    //setters
    void setMQTTServer(const char * szValue);
    void setRoom(const char * szValue);
    void setDeviceTag(const char * szValue);
    void setOTAEnabled(bool bValue);
    //save to eeprom
    void commitToEEPROM(void);
    //replaces placeholders in string with config values
    void replacePlaceholders(String & contents);
};

#endif
