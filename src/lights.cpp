#include "lights.h"
#include "globals.h"


NeoPixelBus<NeoGrbwFeature, NeoEsp32I2s1800KbpsMethod> strip( PixelCount , PixelPin);
//NeoPixelBus<NeoRgbwFeature, NeoEsp32BitBang800KbpsMethod> strip( PixelCount , PixelPin);

void Lights::initlaize()
{
  strip.Begin();
  strip.Show();
}

//Color Algorithm HSV Control --> RGBW Format
//To Do:
//Integrate Accurate Color Temperature
//Integrate Brightness Control W/ Temporal Dithering

void Lights::update(uint8_t wait)
{

}

void Lights::show(){
  //portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
  //portENTER_CRITICAL(&mux);
  strip.Show();
  //portEXIT_CRITICAL(&mux);
}

void Lights::setTemerature(int temp){
  int val_canidate = temp;
  if (val_canidate > temp_max){ val_canidate =temp_max;}
  if (val_canidate < temp_min ){ val_canidate = temp_min;}
  temperature = val_canidate;
}

void Lights::setBrightness(int value){
  float val_canidate = ((float)value) / float(100);
  if (val_canidate > 1){ val_canidate = 1;}
  if (val_canidate < 0 ){ val_canidate = 0;}
  brightness_multiplier = val_canidate;
}

//Color Utility Functions
void Lights::color_all(RgbwColor color){
  for (uint8_t  i=0; i<NUM_LEDS; i++){
		strip.SetPixelColor(i, color);
	}
}
void Lights::color_outside(RgbwColor color){
  for (uint8_t  i=OUTSIDE_START; i<OUTSIDE_END; i++){
		strip.SetPixelColor(i, color);
	}
}
void Lights::color_inside(RgbwColor color){
  for (uint8_t i=INSIDE_START; i<INSIDE_END; i++){
		strip.SetPixelColor(i, color);
	}
}

//Lighting Order Positive Increase Incriment:
//RIGHT OUTSIDE LEDS: OUTSIDE_TOP -> OUTSIDE_BOTOTM
//LEFT OUTSIDE LEDS: 0 -> OUTSIDE TOP + OUTSIDE_BOTTOM -> OUTSIDE END

//LEFT INSIDE LEDS: INSIDE_START -> INSIDE_BOTTOM + INSIDE_TOP -> INSIDE_END
//RIGHT INSIDE LEDS:LEDS_INSIDE_BOTTOM -> INSIDE_TOP
void Lights::color_left(RgbwColor color){

  //Left Outside Bottom
  for (uint8_t i= 0; i< LEDS_OUTSIDE_TOP; i++){
		strip.SetPixelColor(i, color);
	}
  //Left Outside Top
  for (uint8_t i= LEDS_OUTSIDE_BOTTOM; i< LEDS_OUTSIDE_END; i++){
		strip.SetPixelColor(i, color);
	}

  //Left Inside Bottom
  for (uint8_t i= LEDS_INSIDE_START; i< LEDS_INSIDE_BOTTOM; i++){
		strip.SetPixelColor(i, color);
	}
  //Left Inside Top
  for (uint8_t i= LEDS_INSIDE_TOP; i< LEDS_INSIDE_END; i++){
		strip.SetPixelColor(i, color);
	}

}

void Lights::color_right(RgbwColor color){
  //
  //Right Outside
  for (uint8_t i= LEDS_OUTSIDE_TOP; i<LEDS_OUTSIDE_BOTTOM; i++){
		strip.SetPixelColor(i, color);
	}
  //Right Inside
  for (uint8_t i= LEDS_INSIDE_BOTTOM; i< LEDS_INSIDE_TOP; i++){
		strip.SetPixelColor(i, color);
	}
}

void Lights::setPixelColorRGB(uint8_t index, uint8_t R, uint8_t G, uint8_t B, uint8_t W){
  strip.SetPixelColor(index,RgbwColor(R,G,B,W));
}
void Lights::setPixelColorHSV(uint8_t index, uint8_t H, uint8_t S, uint8_t V){
  strip.SetPixelColor(index, hsvColortoRgbw(H,S,V));
}


void Lights::allOff(){
  color_all(RgbwColor(0,0,0,0));
}
void Lights::red(){
  color_all(RgbwColor(255,0,0,0));
}
void Lights::green(){
  color_all(RgbwColor(0,255,0,0));
}
void Lights::blue(){
  color_all(RgbwColor(0,0,255,0));
}
void Lights::warm_white(){
  color_all(RgbwColor(0,0,0,255));
}
void Lights::rgb_white(){
  color_all(RgbwColor(255,255,255,0));
}
void Lights::max_white(){
  color_all(RgbwColor(255,255,255,255));
}

//NeoPixelBus Based Functions
RgbwColor Lights::whiteColorTemp(int temperature){

  int minbreak = 1000;
  int redbreak = 2700;
  int whitebreak = 4000;
  int bluebreak = 5500;
  int maxbreak = 8000;

  //Manage Color Temp Ranges
  if (temperature < minbreak) {temperature = minbreak; }
  if (temperature > maxbreak) {temperature = maxbreak; }

  if (temperature < redbreak){
     return RgbwColor(0, 150 *(1 - (temperature - minbreak)/(redbreak - minbreak)),0,255);
  }
  else if( temperature < whitebreak ){ //Warm White + Scaling RGB
     int rgbval = 255 * ( temperature - redbreak ) / (whitebreak - redbreak);
     return RgbwColor(rgbval,rgbval,rgbval, 255);
  }

  else if( temperature < bluebreak ){ //RGB + Dimming Warm White
    int wwval = 255 * ( bluebreak - temperature ) / (bluebreak - whitebreak);
    return RgbwColor(255,255,255,wwval);
  }
  else{
    int bbval = 255 *(1 - 0.5*( temperature - bluebreak ) / (150 + maxbreak - bluebreak));
    return RgbwColor(bbval*0.9,bbval,255,0);
  }
}

RgbwColor Lights::hsvColortoRgbw(float H, float S, float V){
  //H, S and V input range = 0 ÷ 1.0
  //R, G and B output range = 0 ÷ 255
  uint8_t R,G,B,W;
  uint8_t Wr,Wg,Wb,Ww;
  float var_h, var_1, var_2, var_3,var_r,var_g,var_b;
  int var_i;
  // float H = iH / 255.;
  // float S = iS / 255.;
  // float V = (iV / 255.) * brightness_multiplier;
  V = V * brightness_multiplier;


  RgbwColor whiteColor = whiteColorTemp(temperature);
  Wr = whiteColor.R; Wg = whiteColor.G; Wb = whiteColor.B, Ww = whiteColor.W;
  if (temperature < 2700){
    Wg = Ww;
    Wb = Ww;
  }

  if ( S == 0 )
  {
     R = V * Wr;
     G = V * Wg;
     B = V * Wb;
     W = V * Ww;
  }
  else
  {
     var_h = H * 6;
     if ( var_h == 6 ) var_h = 0;      //H must be < 1
     var_i = int( var_h );             //Or ... var_i = floor( var_h )
     var_1 = V * ( 1 - S );
     var_2 = V * ( 1 - S * ( var_h - var_i ) );
     var_3 = V * ( 1 - S * ( 1 - ( var_h - var_i ) ) );

     if      ( var_i == 0 ) { var_r = V     ; var_g = var_3 ; var_b = var_1; }
     else if ( var_i == 1 ) { var_r = var_2 ; var_g = V     ; var_b = var_1; }
     else if ( var_i == 2 ) { var_r = var_1 ; var_g = V     ; var_b = var_3; }
     else if ( var_i == 3 ) { var_r = var_1 ; var_g = var_2 ; var_b = V ;    }
     else if ( var_i == 4 ) { var_r = var_3 ; var_g = var_1 ; var_b = V ;    }
     else                   { var_r = V     ; var_g = var_1 ; var_b = var_2; }

     R = var_r * Wr;
     G = var_g * Wg;
     B = var_b * Wb;
     W = var_1 * Ww;
  }
  return RgbwColor((uint8_t) R, (uint8_t) G, (uint8_t) B, (uint8_t) W);
}


RgbwColor Lights::hsvColortoRgbw(uint8_t H, uint8_t S, uint8_t Vi) {

  uint8_t R,G,B,W,  V;
  float Wr,Wg,Wb,Ww;
  float var_h, var_1, var_2, var_3,var_r,var_g,var_b;
  int var_i;

  float maxVal(255);
  V = (uint8_t) (Vi * brightness_multiplier);
  RgbwColor whiteColor = whiteColorTemp(temperature);
  Wr = ((float)whiteColor.R)/maxVal; Wg = ((float)whiteColor.G)/maxVal;
  Wb = ((float)whiteColor.B)/maxVal, Ww = ((float)whiteColor.W)/maxVal;

  if (temperature < 2700){
    Wg = Ww;
    Wb = Ww;
  }

  unsigned char region, remainder, p, q, t;

  region = H / 43;
  remainder = (H - (region * 43)) * 6;

  p = (V * (255 - S)) >> 8;
  q = (V * (255 - ((S * remainder) >> 8))) >> 8;
  t = (V * (255 - ((S * (255 - remainder)) >> 8))) >> 8;


  if (S == 0)
  {
      R = V * Wr;
      G = V * Wg;
      B = V * Wb;
      W = V * Ww;
  }
  else{
    switch (region)
    {
        case 0:
            R = V* Wr; G = t* Wg; B = p* Wb;
            break;
        case 1:
            R = q* Wr; G = V* Wg; B = p* Wb;
            break;
        case 2:
            R = p* Wr; G = V *Wg; B = t* Wb;
            break;
        case 3:
            R = p* Wr; G = q *Wg; B = V* Wb;
            break;
        case 4:
            R = t* Wr; G = p* Wg; B = V* Wb;
            break;
        case 5:
            R = V* Wr; G = p* Wg; B = q* Wb;
            break;
    }
    W = p * Ww;
  }

  return RgbwColor((uint8_t) R, (uint8_t) G, (uint8_t) B, (uint8_t) W);
}
