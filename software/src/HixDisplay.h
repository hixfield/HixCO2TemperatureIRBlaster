#ifndef HixDisplay_h
#define HixDisplay_h

#include <Adafruit_SSD1306.h>


class HixDisplay : public Adafruit_SSD1306 {
  private:
    int16_t printCenter(const char * szText, int16_t y);
    int16_t printRight(const char * szText, int16_t y);
    void    drawBoxes(int16_t x, int16_t y, bool bFanOn);

  public:
    HixDisplay();
    bool begin();
    void drawDisplayVersion(const char * szName, const char * szVersion);
    void showStatus(float fTemp, int nCO2, int nRSSI, bool bLoopToggle);
};

#endif
