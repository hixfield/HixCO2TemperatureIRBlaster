#ifndef HixMQTT_h
#define HixMQTT_h

#include <HixMQTTBase.h>
#include <ArduinoJson.h>

class HixMQTT : public HixMQTTBase {
  private:
    //used when constructing json (seams to be more stable then allocating on stack...)
    DynamicJsonDocument m_jsonDoc;
    String m_jsonString;
    //puts all values in one single json compatible with influxDB
    String influxDBJson(int nCO2, float fTemperature);

  public:
    HixMQTT(const char * szWifi_SSID,
            const char * szWiFi_Password,
            const char * szMQTT_Server,
            const char * szDeviceType,
            const char * szDeviceVersion,
            const char * szRoom,
            const char * szDeviceTag);
    //publishes all the values for the sensor
    bool publishStatusValues(int nCO2, float fTemperature);
};

#endif
