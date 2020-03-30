#include "DS18B20Temperature.h"

DS18B20Temperature::DS18B20Temperature(int nPinOneWire) : m_oneWire(nPinOneWire),
                                                          m_dallasTemperature(&m_oneWire) {
}

bool DS18B20Temperature::begin(void) {
    m_dallasTemperature.begin();
    if (!m_dallasTemperature.getAddress(m_deviceAddress, 0)) return false;
    m_dallasTemperature.setResolution(m_deviceAddress, 11);
    m_dallasTemperature.setWaitForConversion(false);
    m_dallasTemperature.requestTemperatures();
    return true;
}

float DS18B20Temperature::getTemp(void) {
    //if previous still handing do nothing
    if (m_dallasTemperature.isConversionComplete()) {
        //get new temperature
        float m_fTempTemp = m_dallasTemperature.getTempC(m_deviceAddress);
        if (m_fTempTemp != DEVICE_DISCONNECTED_C) {
            m_fTemperature  = m_fTempTemp;
            m_nErrorCounter = 0;
        } else {
            m_nErrorCounter++;
                Serial.println("DS18B20 failed to get temperature!!!");            
            if (m_nErrorCounter > 10) {
                Serial.println("DS18B20 to many failures getting temp, resetting...");
                ESP.reset();
            }
        }

        //immeditatly start a new conversion
        m_dallasTemperature.requestTemperatures();
    }
    //return current measured temp
    return m_fTemperature;
}
