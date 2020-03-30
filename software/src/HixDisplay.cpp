#include "HixDisplay.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>


HixDisplay::HixDisplay() : Adafruit_SSD1306(128, 64, &Wire, -1) {
}

bool HixDisplay::begin(void) {
    return Adafruit_SSD1306::begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void HixDisplay::drawDisplayVersion(const char * szName, const char * szVersion) {
    clearDisplay();
    //draw temperature in big
    setCursor(0, 0);
    setTextSize(2);
    setTextColor(WHITE);
    println(szName);
    print(szVersion);
    display();
}

void HixDisplay::showStatus(float fTemp, int nCO2, int nRSSI, bool bLoopToggle) {
    char szVar1[10]      = "";
    char szVar2[10]      = "";
    char szTempSymbol[2] = "C";
    clearDisplay();
    setTextColor(WHITE);
    //draw temperature
    dtostrf(fTemp, 0, 1, szVar1);
    sprintf(szVar2, "%s %s", szVar1, szTempSymbol);
    printCenter(szVar2, 5);
    //draw co2
    itoa(nCO2, szVar1, 10);
    sprintf(szVar2, "%s ppm", szVar1);
    printCenter(szVar2, 23);
    //draw RSSI
    itoa(nRSSI, szVar1, 10);
    sprintf(szVar2, "%s dBm", szVar1);
    printCenter(szVar2, 43);
    //draw updating
    if (bLoopToggle) {
        drawCircle(2, 5, 2, WHITE);
    }
    //show on screen
    display();
}

void HixDisplay::drawBoxes(int16_t x, int16_t y, bool bFanOn) {
    static byte progress = 0;
    if (!bFanOn) {
        return;
    }
    for (int i = 0; i < progress + 1; i++) {
        fillRect(x, y, 4, 21, WHITE);
        fillRect(width() - x - 4, y, 4, 21, WHITE);
        x += 6;
    }

    progress++;
    progress %= 4;
}

//returns the end x position of the string
int16_t HixDisplay::printCenter(const char * szText, int16_t y) {
    int16_t  x1, y1;
    uint16_t w, h;
    getTextBounds(szText, 0, y, &x1, &y1, &w, &h);
    setCursor((width() - w) / 2, y);
    print(szText);
    return width() - ((width() - w) / 2);
}

int16_t HixDisplay::printRight(const char * szText, int16_t y) {
    int16_t  x1, y1;
    uint16_t w, h;
    getTextBounds(szText, 0, y, &x1, &y1, &w, &h);
    setCursor(width() - w, y);
    print(szText);
    return width() - w;
}
