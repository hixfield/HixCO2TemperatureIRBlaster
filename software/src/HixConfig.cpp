#include "HixConfig.h"
#include "secret.h"
#include <EEPROM.h>

HixConfig::HixConfig() {
    EEPROM.begin(512);
    EEPROM.get(m_nEPPROMBaseAddress, data);
    if (calculateCRC() != data.crc) {
        commitDefaults();
    }
}

int HixConfig::getNumberOfBootUps(void) {
    return data.nNumberOfBootUps;
}

const char * HixConfig::getMQTTServer(void) {
    //    return "192.168.99.219";
    return data.szMQTTServer;
};

const char * HixConfig::getRoom(void) {
    //   return "test_room";
    return data.szRoom;
};

const char * HixConfig::getDeviceTag(void) {
    //  return "test_tag";
    return data.szDeviceTag;
};

bool HixConfig::getOTAEnabled(void) {
    return data.bOTAEnabled;
}

bool HixConfig::getLEDEnabeled(void) {
    return data.bLEDEnabled;
}

bool HixConfig::getOLEDEnabled(void) {
    return data.bOLEDEnabled;
}

bool HixConfig::getSelfTestEnabled(void) {
    return data.bSelfTestEnabled;
}

void HixConfig::setNumberOfBootUps(int bValue) {
    data.nNumberOfBootUps = bValue;
}


void HixConfig::setMQTTServer(const char * szValue) {
    memset(data.szMQTTServer, 0, sizeof(data.szMQTTServer));
    strncpy(data.szMQTTServer, szValue, sizeof(data.szMQTTServer) - 1);
}

void HixConfig::setRoom(const char * szValue) {
    memset(data.szRoom, 0, sizeof(data.szRoom));
    strncpy(data.szRoom, szValue, sizeof(data.szRoom) - 1);
}

void HixConfig::setDeviceTag(const char * szValue) {
    memset(data.szDeviceTag, 0, sizeof(data.szDeviceTag));
    strncpy(data.szDeviceTag, szValue, sizeof(data.szDeviceTag) - 1);
}

void HixConfig::setOTAEnabled(bool bValue) {
    data.bOTAEnabled = bValue;
}

void HixConfig::setLEDEnabled(bool bValue) {
    data.bLEDEnabled = bValue;
}

void HixConfig::setOLEDEnabled(bool bValue) {
    data.bOLEDEnabled = bValue;
}

void HixConfig::setSelfTestEnabled(bool bValue) {
    data.bSelfTestEnabled = bValue;
}

void HixConfig::incrementNumberOfBootUps(void) {
    data.nNumberOfBootUps++;
    commitToEEPROM();
}

void HixConfig::commitToEEPROM(void) {
    data.crc = calculateCRC();
    EEPROM.put(m_nEPPROMBaseAddress, data);
    EEPROM.commit();
}

unsigned long HixConfig::calculateCRC(void) {
    unsigned char * crcData = (unsigned char *)&data;
    size_t          size    = sizeof(data) - sizeof(data.crc);
    unsigned long   crc     = 1234;
    for (size_t index = 0; index < size; index++) {
        crc += crcData[index];
    }
    return crc;
}

void HixConfig::commitDefaults(void) {
    setNumberOfBootUps(0);
    setMQTTServer(Secret::MQTT_SERVER);
    setRoom("test_room");
    setDeviceTag("test_tag");
    setOTAEnabled(true);
    setLEDEnabled(true);
    setOLEDEnabled(true);
    setSelfTestEnabled(true);
    commitToEEPROM();
}

void HixConfig::replacePlaceholders(String & contents) {
    contents.replace("||NBR_BOOTUPS||", String(getNumberOfBootUps()));
    contents.replace("||MQTT_SERVER||", getMQTTServer());
    contents.replace("||ROOM||", getRoom());
    contents.replace("||DEVICE_TAG||", getDeviceTag());
    contents.replace("||DEVICE_TYPE||", getDeviceType());
    contents.replace("||DEVICE_VERSION||", getDeviceVersion());
    contents.replace("||OTA_ENABLED||", getOTAEnabled() ? "checked" : "");
    contents.replace("||LED_ENABLED||", getLEDEnabeled() ? "checked" : "");
    contents.replace("||OLED_ENABLED||", getOLEDEnabled() ? "checked" : "");
    contents.replace("||SELFTEST_ENABLED||", getSelfTestEnabled() ? "checked" : "");
}