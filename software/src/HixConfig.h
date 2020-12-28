#ifndef HixConfig_h
#define HixConfig_h

#include <arduino.h>

class HixConfig {
  private:
    //base address for epprom
    const int m_nEPPROMBaseAddress = 0;
    //hilds the data
    struct {
        int           nNumberOfBootUps;
        char          szMQTTServer[50];
        char          szRoom[50];
        char          szDeviceTag[50];
        bool          bOTAEnabled;
        bool          bLEDEnabled;
        bool          bOLEDEnabled;
        bool          bSelfTestEnabled;
        bool          bAutoBackgroundCalibrationEnabled;
        unsigned long crc;
    } data;
    //determine crc
    unsigned long calculateCRC(void);
    //store defaults
    void commitDefaults(void);

  public:
    HixConfig();
    //getters
    int          getNumberOfBootUps(void);
    const char * getDeviceType(void) { return "HixCO2TemperatureIRBlaster"; };
    const char * getDeviceVersion(void) { return "2.2.3"; };
    const char * getMQTTServer(void);
    const char * getRoom(void);
    const char * getDeviceTag(void);
    bool         getOTAEnabled(void);
    bool         getLEDEnabeled(void);
    bool         getOLEDEnabled(void);
    bool         getSelfTestEnabled(void);
    bool         getAutoBackgroundCalibrationEnabled(void);
    //setters
    void setNumberOfBootUps(int bValue);
    void setMQTTServer(const char * szValue);
    void setRoom(const char * szValue);
    void setDeviceTag(const char * szValue);
    void setOTAEnabled(bool bValue);
    void setLEDEnabled(bool bValue);
    void setOLEDEnabled(bool bValue);
    void setSelfTestEnabled(bool bValue);
    void setAutoBackgroundCalibrationEnabled(bool bValue);
    //helpers
    void incrementNumberOfBootUps(void);
    //save to eeprom
    void commitToEEPROM(void);
    //replaces placeholders in string with config values
    void replacePlaceholders(String & contents);
};

#endif
