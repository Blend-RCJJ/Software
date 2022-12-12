// led.h

#ifndef _LED_H_
#define _LED_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel.h>

class WS2812B {
   public:
    WS2812B(int brightness);

    void battery(int percent);

    void show(void);

    unsigned long colorRGB(int red, int green, int blue);
    unsigned long colorHSV(int hue, int saturation, int brightness);

    unsigned long red = colorHSV(0,255,255);
    unsigned long green = colorHSV(90,255,255);
    unsigned long blue = colorHSV(180,255,255);
    unsigned long pink = colorHSV(0,125,255);
    unsigned long yellow = colorHSV(45,255,255);
    void setAllColor(unsigned long color);
    void setLeftColor(unsigned long color);

   private:
};

#endif