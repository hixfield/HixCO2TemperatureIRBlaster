#include "HixMQTT.h"
#include <Arduino.h>
#include <ArduinoJson.h>

HixMQTT::HixMQTT(const char * szWifi_SSID,
                 const char * szWiFi_Password,
                 const char * szMQTT_Server,
                 const char * szDeviceType,
                 const char * szDeviceVersion,
                 const char * szRoom,
                 const char * szDeviceTag) : HixMQTTBase(szWifi_SSID, szWiFi_Password, szMQTT_Server, szDeviceType, szDeviceVersion, szRoom, szDeviceTag) {
}


bool HixMQTT::publishStatusValues(int nCO2, float fTemperature) {
    //call base implementation
    if (!HixMQTTBase::publishStatusValues()) {
        return false;
    }
    //my custom implementation
    if (isConnected()) {
        //dynamic values are not published with a retain = default value = false
        publish(topicForPath("status/co2"), nCO2);
        publish(topicForPath("status/temperature"), fTemperature);
        //publish to influxdb topic
        publish(topicForPath("influxdb"), influxDBJson(nCO2, fTemperature));
        //return non error
        return true;
    }
    //not connected, return error
    return false;
}

/*
  [
   --> contain the measurements
   {
      "co2"           : 4564,
      "temperature"   : 45,
      "wifi_rssi"     : -45
   },
   --> contain the tags
   {
      "device_type"   : "sensor1",
      "device_version": "device2",
      "device_tag"    : "1"
      "room"          : "living",
      "wifi_ssid"     : "test"
   }
  ]
*/

String HixMQTT::influxDBJson(int nCO2, float fTemperature) {
    DynamicJsonDocument doc(500);

    //create the measurements => fields
    JsonObject doc_0     = doc.createNestedObject();
    doc_0["co2"]         = nCO2;
    doc_0["temperature"] = fTemperature;
    doc_0["wifi_rssi"]   = WiFi.RSSI();
    ;

    //the device props => tags
    JsonObject doc_1        = doc.createNestedObject();
    doc_1["device_type"]    = m_deviceType;
    doc_1["device_version"] = m_deviceVersion;
    doc_1["device_tag"]     = m_deviceTag;
    doc_1["room"]           = m_room;
    doc_1["wifi_ssid"]      = WiFi.SSID();

    //to string
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}
