#ifndef LIGHTS_H
#define LIGHTS_H

#include <Arduino.h>
//#include "dotstar.h"1
#include <FastLED.h>
// #include <SPI.h>
// #include <Wire.h>
#include <NeoPixelBus.h>
#include "system_info.h"

const uint16_t PixelCount = NUM_LEDS; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 4;  // make sure to set this to the correct pin, ignored for Esp8266

class Lights
{ //In Which We Light The World
public:
  //Lights();
  ~Lights(){};
  float brightness_multiplier = 0.05;
  bool _on = true;

  int temperature = 5500;
  int temp_max = 10000;
  int temp_min = 1900;

  const uint8_t OUTSIDE_START = LEDS_OUTSIDE_START;
  const uint8_t OUTSIDE_END = LEDS_OUTSIDE_END;
  const uint8_t INSIDE_START =  LEDS_INSIDE_START ;
  const uint8_t INSIDE_END = LEDS_INSIDE_END;

  //Important Funcitons
  virtual void update(uint8_t wait);
  virtual void initlaize();
  void refresh();
  void show();

  //Set Value
  void setTemerature( int temp);
  void setBrightness( int value);

  //Get Value
  void com_getTemerature( uint8_t temp) {};
  void com_getBrightness( uint8_t temp) {};

  //Color Functions
  void color_all(RgbwColor color);
  void color_outside(RgbwColor color);
  void color_inside(RgbwColor color);
  void color_right(RgbwColor color);
  void color_left(RgbwColor color);

  void setPixelColorRGB(uint8_t index, uint8_t R, uint8_t G, uint8_t B, uint8_t W);
  void setPixelColorHSV(uint8_t index, uint8_t H, uint8_t S, uint8_t V);

  void allOff();
  void red();
  void green();
  void blue();
  void warm_white();
  void rgb_white();
  void max_white();
  void color_loop(float hue);


  //NeoPixelBus Based Functions
  RgbwColor whiteColorTemp(int temperature);
  RgbwColor hsvColortoRgbw(float H, float S, float V);
  RgbwColor hsvColortoRgbw(uint8_t H, uint8_t S, uint8_t V);

};

// extern LiWire LightWire;

#endif //LIGHTS_H
