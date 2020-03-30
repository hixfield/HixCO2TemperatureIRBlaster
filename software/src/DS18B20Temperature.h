#ifndef DS18B20Temperature_h
#define DS18B20Temperature_h

#include <DallasTemperature.h>
#include <OneWire.h>

class DS18B20Temperature {
    OneWire           m_oneWire;
    DallasTemperature m_dallasTemperature;
    DeviceAddress     m_deviceAddress;
    float             m_fTemperature  = 0;
    unsigned int      m_nErrorCounter = 0;

  public:
    //lifecycle
    DS18B20Temperature(int nPinOneWire);
    bool begin(void);

    //public api
    float getTemp(void);
};


#endif
