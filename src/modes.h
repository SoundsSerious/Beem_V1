#ifndef GAMES_H
#define GAMES_H

#include <Arduino.h>
#include "observer.h"
#include "event.h"
#include "system_info.h"
#include <vector>
#include <FastLED.h>
#include <NeoPixelBus.h>
#include "lib8tion/random8.h"

// DEFINE_GRADIENT_PALETTE( suncolors ) {
//       0,     0,  0,  0,   //black
//     128,   255,  0,  0,   //red
//     224,   255,255,  0,   //bright yellow
//     255,   255,255,255 }; //full white

class COMEvent;

class IMode: public Observer
{
  public:
    virtual void initialize() = 0;
    virtual void render() = 0;
    virtual void update() = 0;
    virtual void leave() = 0;
    virtual String name() = 0;
};

class Mode: public IMode
{
  public:
    Mode(){};
    virtual ~Mode(){};
    String mode_name = "Mode";
    virtual void render();
    virtual void initialize();
    virtual void update();
    virtual void leave();
    virtual String name();
    virtual ObserverType type();
    ObserverType _type = GAME;

    //Common Mode Inputs / Controls
    CHSV colors[8];
    CRGBPalette16 pallette;
    uint8_t settings[8];

    //Observer Function Overloading
    virtual void onNotify(Event *event);
    virtual void onNotify(MotionEvent *event);
    virtual void onNotify(COMEvent *event);

    //Utility
    virtual void log(String msg = "",bool force = false);

    float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
    {
     return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
};


class SolidColor: public Mode
{
  public:
    String mode_name = "SolidColor";
    virtual void render();
    virtual void initialize();
    virtual void update();
    // virtual void leave();
    virtual String name();

    CHSV darkness = CHSV(180,0,0);
    CHSV col_in_current, col_out_current;

    bool fadeAnimation = false;  //Scale = 0 => OFF
    unsigned long animationPeriod = 1 * 1000000;
    unsigned long animationStart, animationTime, dt_ani;
    unsigned long q1,q2,q3;
    float timeScale = 1.0; //-> 0.1 -> 10.0 0 == OFF
    float hold_fraction = 0.8;
    float fade_fraction = 0.2;
    //Observer Function Overloading
    //virtual void onNotify(COMEvent *event);

};

class FlashColor: public Mode
{
  public:
    String mode_name = "FlashColor";
    virtual void render();
    virtual void initialize();
    virtual void update();
    // virtual void leave();
    virtual String name();

    CHSV darkness = CHSV(180,0,0);
    CHSV col_left_current, col_right_current;

    bool fadeAnimation = false;  //Scale = 0 => OFF
    unsigned long animationPeriod = 1 * 1000000;
    unsigned long animationStart, animationTime, dt_ani;
    unsigned long q1,q2,q3;
    float timeScale = 1.0; //-> 0.1 -> 10.0 0 == OFF
    float hold_fraction = 0.8;
    float fade_fraction = 0.2;
    //Observer Function Overloading
    //virtual void onNotify(COMEvent *event);

};

class DynamicLighting: public Mode
{
  public:
    String mode_name = "DynamicLighting";
    virtual void render();
    virtual void initialize();
    virtual void update();
    // virtual void leave();
    virtual String name();

    uint8_t inner_leds = LEDS_OUTSIDE_END;
    CRGB leds[NUM_LEDS];
    uint8_t heat[LEDS_OUTSIDE_END];
    void animation();

    // SPARKING: What chance (out of 255) is there that a new spark will be lit?
    // Higher chance = more roaring fire.  Lower chance = more flickery fire.
    // Default 120, suggested range 50-200.
    uint8_t sparking = 120;
    uint8_t cooling  = 55;

    //Observer Function Overloading
    //virtual void onNotify(COMEvent *event);

};

class GradientLighting: public Mode
{
  public:
    String mode_name = "GradientLighting";
    virtual void render();
    virtual void initialize();
    virtual void update();
    // virtual void leave();
    virtual String name();

    //Animation Stuff
    uint8_t start_index,pos_index;
    uint16_t maxWidth = LEDS_OUTSIDE_END;
    uint16_t halfWidth = maxWidth / 2;
    uint8_t incriment = 255 / halfWidth;
    CRGB leds[LEDS_OUTSIDE_END+2];

    float grad_pos,V;
    float RPS = 1.0;
    float RPS_MAX = 2.5;
    float NPS = maxWidth * RPS;
    uint8_t index; //

    void animation();
    //Observer Function Overloading
    //virtual void onNotify(COMEvent *event);

};

class Flicker: public Mode
{
  //Every iteration has x percent chance to light up start a flicker at a cell...
  //Flicker Effect: Increase In Brightness -> Hold -> Dim to Zero

  public:
    String mode_name = "Flicker";
    virtual void render();
    virtual void initialize();
    virtual void update();
    // virtual void leave();
    virtual String name();

    bool sparking[NUM_LEDS];
    uint8_t spark_val[NUM_LEDS];
    void animation();

    // SPARKING: What chance (out of 255) is there that a new spark will be lit?
    // Higher chance = more roaring fire.  Lower chance = more flickery fire.
    // Default 120, suggested range 50-200.
    uint8_t num_sparks = 8; //setting[0] try rand(x) lighting chances per turn
    uint8_t sparking_threshold = 120; //setting[1] spark if rand8() > x
    uint8_t heating  = 55;  //setting[2]
    uint8_t cooling  = 55;  //setting[3] nscale(255 - rand8(cooling))

    //Observer Function Overloading
    //virtual void onNotify(COMEvent *event);

};

class FlickerPalette: public Mode
{
  //Every iteration has x percent chance to light up start a flicker at a cell...
  //Flicker Effect: Increase In Brightness -> Hold -> Dim to Zero

  public:
    String mode_name = "FlickerPalette";
    virtual void render();
    virtual void initialize();
    virtual void update();
    // virtual void leave();
    virtual String name();

    bool sparking[NUM_LEDS];
    uint8_t spark_val[NUM_LEDS];
    void animation();

    // SPARKING: What chance (out of 255) is there that a new spark will be lit?
    // Higher chance = more roaring fire.  Lower chance = more flickery fire.
    // Default 120, suggested range 50-200.
    uint8_t num_sparks = 8; //setting[0] try rand(x) lighting chances per turn
    uint8_t sparking_threshold = 120; //setting[1] spark if rand8() > x
    uint8_t heating  = 55;  //setting[2]
    uint8_t cooling  = 55;  //setting[3] nscale(255 - rand8(cooling))

    //Observer Function Overloading
    //virtual void onNotify(COMEvent *event);

};

class Flash: public Mode
{
  //Every iteration has x percent chance to light up start a flicker at a cell...
  //Flicker Effect: Increase In Brightness -> Hold -> Dim to Zero

  public:
    String mode_name = "Flash";
    virtual void render();
    virtual void initialize();
    virtual void update();
    // virtual void leave();
    virtual String name();

    bool sparking[NUM_LEDS];
    uint8_t spark_val[NUM_LEDS];
    void animation();

    //Observer Function Overloading
    //virtual void onNotify(COMEvent *event);

};

class DirectColor: public Mode
{
  //Every iteration has x percent chance to light up start a flicker at a cell...
  //Flicker Effect: Increase In Brightness -> Hold -> Dim to Zero

  public:
    String mode_name = "DirectColor";
    virtual void render();
    virtual void initialize();
    virtual void update();
    // virtual void leave();
    virtual String name();

    void animation();

    //Observer Function Overloading
    //virtual void onNotify(COMEvent *event);

};

// class FireWork: public Mode
// {
//   public:
//     String name = "Firework";
//     virtual void render();
//     virtual void initialize();
//     virtual void update();
//     //virtual void leave();
//
//     CRGB lights[NUM_LEDS] = {CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red};;
//     CRGB lights_mask[NUM_LEDS];
//     CRGB lights_asnd[NUM_LEDS] = {CRGB::Gold,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black};
//     uint16_t speed = 20;
//     int16_t scale = 100;
//
//     unsigned long lastUpdate,now;
//     unsigned long updateInterval = 1E6/5;
//
//     // This is the array that we keep our computed noise values in
//     uint8_t currentPalletteNo = 0;
//     //TBlendType    currentBlending;
//
//     CRGBPalette16 currentPalette;
//
//     //modes
//     bool launched, peaked, falling, exploded, landed;
//     bool fireworking = false;
//     uint8_t maskIndex;
//     unsigned long animationStart, interval, nextLEDTime;
//     unsigned long animationLength = 500000;
//     unsigned long ledPerMicro = animationLength / NUM_LEDS;
//
//
//     uint8_t ihue=100;
//     uint8_t noise[NUM_LEDS];
//     uint16_t x,z;
//     void fillnoise();
//     void mapNoiseToLEDsUsingPalette();
//     void maskTipLEDs(uint8_t numled);
//     void fadeLEDS(uint8_t fadeAmount);
//
//     uint8_t       colorLoop = 1;
//
//     //Observer Function Overloading
//     virtual void onNotify(COMEvent *event);
//     virtual void onNotify(MotionEvent *event);
// };



// class PACMAN: public Mode
// {
//   public:
//     String name = "PACMAN";
//     virtual void render();
//     virtual void initialize();
//     virtual void update();
//     virtual void leave();
//     CRGB pacManColor = CRGB(5,1,0);
//     CRGB mouthColor = CRGB(0,0,2);
//     CRGB eyeColor = CRGB(0,0,15);
//     CRGB eyeColorBright = CRGB(0,0,50);
//     CRGB restColor = CRGB(1,0,50);
//     float omega = 0;
//     unsigned long now;
//     unsigned long lastTime;
//
//     float omega_thresh = 25;
//     float dTheta = 0;
//     float dAng_LED = 120;
//     float LED_Ang = 0;
//     float theta = 0;
//
//     uint8_t eye_index = 3;
//     float eye_angle = 65;
//     float dThetaEye = 8;
//
//     bool mouthOpen = true;
//     unsigned long lastMouthEvent;
//     unsigned long openMouthTime = 1000000;
//     float mouthOpenAngle = 30.0;
//     float mouthCloseAngle = 10.0;
//     float dmouthAngledt = 50; //deg/s
//     float dmAdus = dmouthAngledt / 1E6;
//     float mouthAngle = mouthOpenAngle; //Open To Start
//
//     float doDaPacman = false;
//     virtual void onNotify(MotionEvent *event);
// };







// class TestMode2: public Mode
// {
//   public:
//     String name = "Test Mode 2";
//     virtual void render();
//     virtual void initialize();
//     virtual void update();
//     virtual void leave();
// };

class ModeManager: public IMode
{
  //Class that passes argument to current state
public:
  ModeManager();
  ~ModeManager(){};
  unsigned int modeIndex = 0;
  std::vector<Mode*>  _modes;
  //Important Funcitons
  Mode* currentMode();

  void addMode( Mode *mode);
  void setup_modes();
  virtual void switchToMode(unsigned int newModeIndex);

  //Mode Interface Methods
  virtual void initialize();
  virtual void initialize( unsigned int customIndex);
  virtual void leave();
  virtual void update();
  virtual void render();
  virtual String name();

};




#endif
