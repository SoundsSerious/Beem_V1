#include "modes.h"
#include "globals.h"


//Declare Modes
//Mode Test_Mode_0;
//PACMAN pacman;
Flash flash;
SolidColor solidcolor;
DynamicLighting dynamiclighting;
GradientLighting gradientlighting;
FlashColor flashcolor;
Flicker flicker;
FlickerPalette flickerpalette;
DirectColor directcolor;
// FireWork firework;
// TestMode2 Test_Mode_2;


//Mode Class Defaults
void Mode::log(String msg, bool force)
{
  beem.com.log("Mode:\t"+msg,force);
}

void Mode::render()
{
  log( "Rendering: "+ this -> name() );
}
void Mode::initialize()
{
  log( "Initializing: "+ this -> name() );
}

void Mode::update()
{
  //log(  "Updating: " + name() );
}
void Mode::leave()
{
  log( "Leaving Mode: "+this -> name());
}

String Mode::name(){
  return this -> mode_name;
}

ObserverType Mode::type()
{
  return this -> _type;
}


void Mode::onNotify(Event *event){ }
void Mode::onNotify(MotionEvent *event){ }

//Default Event Behavior (Colors, Palettes, Settings)
void Mode::onNotify(COMEvent *event)
{
  EventTypes type = event -> type();
  log("Got Event Type: "+String(type),COM_EVT_DEBUG);
  switch( type ){
    case EventTypes::ModeColorEvt:{
      ModeColorEv *sc = static_cast<ModeColorEv *>(event);
      log("Got Color H:"+String(sc -> H)+" S:"+String(sc -> S)
          +" V:"+String(sc -> V)+" I:"+String(sc -> inx),COM_EVT_DEBUG);
          colors[sc -> inx] = CHSV( sc -> H, sc -> S, sc -> V);
      break;
    }
    break;
    case EventTypes::ModePalletteEvt:{
     ModePalletteEv *pal = static_cast<ModePalletteEv *>(event);
     log("Got Palette H:"+String(pal -> index),COM_EVT_DEBUG);
     switch(pal -> index){
        case 0:
          pallette = HeatColors_p;
          break;
        case 1:
          pallette = CloudColors_p;
          break;
        case 2:
          pallette = LavaColors_p;
          break;
        case 3:
          pallette = OceanColors_p;
          break;
        case 4:
          pallette = ForestColors_p;
          break;
        case 5:
          pallette = RainbowColors_p;
          break;
        case 6:
          pallette = RainbowStripeColors_p;
          break;
        case 7:
          pallette = PartyColors_p;
          break;
        default:
          pallette = HeatColors_p;
          break;
     }
     break;
    }

    case EventTypes::ModeSettingEvt:{
      ModeSettingEv *st = static_cast<ModeSettingEv *>(event);
      log("Got Setting I:"+String(st -> index)+" S:"+String(st -> setting),COM_EVT_DEBUG);
      settings[st -> index] = st -> setting;
      break;
    }
    break;
  }
}


//Solid Color //////////////////////////////////////////////////////////////////
void SolidColor::initialize()
{
  colors[0] = CHSV(25,255,255);
  colors[1] = CHSV(120,255,255);
}

void SolidColor::render()
{
  log(  "Rendering: " + name() );
  //beem.lights.color_all( beem.lights.hsvColorwithTemp(0.8,1,1) );

  //Handle Preprocessing Loop
  if (settings[0] == 0){ fadeAnimation = false;}
  else{ fadeAnimation = true;
        animationPeriod = (unsigned long) (1000000 * mapfloat(float(settings[0]), 0.0, 255.0, 0.0, 10.0));
        hold_fraction = mapfloat(float(settings[1]), 0.0, 255.0, 0.0, 1.0);
        fade_fraction = 1.0 - hold_fraction;
      }


  if (fadeAnimation){
    animationTime = micros();
    if ( (animationTime - animationStart) > animationPeriod){ animationStart =  micros();} //Reset
    dt_ani = animationTime - animationStart;
    q1 = animationPeriod *hold_fraction / 2;
    q2 = animationPeriod / 2;
    q3 = (animationPeriod * (0.5 + hold_fraction));

    if ( dt_ani < q1){
      col_in_current = darkness;
      col_out_current = colors[0];
    }
    else if (dt_ani < q2){
      uint8_t fadefrac = (uint8_t) map(dt_ani, q1, q2, 0, 255);
      col_in_current = blend(darkness,colors[1], fadefrac);
      col_out_current = blend(colors[0],darkness, fadefrac);
    }
    else if ( dt_ani < q3){
      col_in_current = colors[1];
      col_out_current = darkness;
    }
    else if (dt_ani < animationPeriod){
      uint8_t fadefrac = (uint8_t) map(dt_ani, q3, animationPeriod, 0, 255);
      col_in_current = blend(colors[1],darkness, fadefrac);
      col_out_current = blend(darkness,colors[0], fadefrac);
    }
    else{
      col_in_current = darkness;
      col_out_current = colors[0];
    }
    //Writei Strips
    beem.lights.color_outside( beem.lights.hsvColortoRgbw( col_out_current.h,
                                                           col_out_current.s,
                                                           col_out_current.v));
    beem.lights.color_inside( beem.lights.hsvColortoRgbw(  col_in_current.h,
                                                           col_in_current.s,
                                                           col_in_current.v));
  }
  else{
    beem.lights.color_outside( beem.lights.hsvColortoRgbw( colors[0].h,
                                                           colors[0].s,
                                                           colors[0].v));
    beem.lights.color_inside( beem.lights.hsvColortoRgbw(  colors[1].h,
                                                           colors[1].s,
                                                           colors[1].v));
  }
}

void SolidColor::update()
{
  //log(  "Updating: " + name() );
}

String SolidColor::name(){
  return this -> mode_name;
}



//Flash Color //////////////////////////////////////////////////////////////////
void FlashColor::initialize()
{
  colors[0] = CHSV(0,0,255);
  colors[1] = CHSV(240,0,255);
}

void FlashColor::render()
{
  log(  "Rendering: " + name() );
  //beem.lights.color_all( beem.lights.hsvColorwithTemp(0.8,1,1) );

  //Handle Preprocessing Loop
  if (settings[0] == 0){ fadeAnimation = false; }
  else{ fadeAnimation = true;
        animationPeriod = (unsigned long) (1000000 * mapfloat(float(settings[0]), 0.0, 255.0, 0.0, 10.0));
        hold_fraction = mapfloat(float(settings[1]), 0.0, 255.0, 0.0, 1.0);
        fade_fraction = 1.0 - hold_fraction;
      }


  if (fadeAnimation){
    animationTime = micros();
    if ( (animationTime - animationStart) > animationPeriod){ animationStart =  micros();} //Reset
    dt_ani = animationTime - animationStart;
    q1 = animationPeriod *hold_fraction / 2;
    q2 = animationPeriod / 2;
    q3 = (animationPeriod * (0.5 + hold_fraction));

    if ( dt_ani < q1){
      col_left_current = CHSV(colors[1].h,0,0);
      col_right_current = colors[0];
    }
    else if (dt_ani < q2){
      uint8_t fadefrac = (uint8_t) map(dt_ani, q1, q2, 0, 255);
      col_left_current = blend(CHSV(colors[1].h,0,0),colors[1], fadefrac);
      col_right_current = blend(colors[0],CHSV(colors[0].h,0,0), fadefrac);
    }
    else if ( dt_ani < q3){
      col_left_current = colors[1];
      col_right_current = CHSV(colors[0].h,0,0);
    }
    else if (dt_ani < animationPeriod){
      uint8_t fadefrac = (uint8_t) map(dt_ani, q3, animationPeriod, 0, 255);
      col_left_current = blend(colors[1],CHSV(colors[1].h,0,0), fadefrac);
      col_right_current = blend(CHSV(colors[0].h,0,0),colors[0], fadefrac);
    }
    else{
      col_left_current = CHSV(colors[1].h,0,0);
      col_right_current = colors[0];
    }
    //Writei Strips
    beem.lights.color_right( beem.lights.hsvColortoRgbw( col_right_current.h,
                                                           col_right_current.s,
                                                           col_right_current.v));
    beem.lights.color_left( beem.lights.hsvColortoRgbw(  col_left_current.h,
                                                           col_left_current.s,
                                                           col_left_current.v));
  }
  else{
    beem.lights.color_right( beem.lights.hsvColortoRgbw( colors[0].h,
                                                           colors[0].s,
                                                           colors[0].v));
    beem.lights.color_left( beem.lights.hsvColortoRgbw(  colors[1].h,
                                                           colors[1].s,
                                                           colors[1].v));
  }
}

void FlashColor::update()
{
  //log(  "Updating: " + name() );
}

String FlashColor::name(){
  return this -> mode_name;
}


//Fire Mode ////////////////////////////////////////////////////////////////////
void DynamicLighting::initialize()
{
  colors[0]= CHSV(25,255,255);
  pallette = HeatColors_p;
}

void DynamicLighting::render()
{
  log(  "Rendering: " + name() );


  //Color Outside
  beem.lights.color_outside( beem.lights.hsvColortoRgbw( colors[0].h,
                                                         colors[0].s,
                                                         colors[0].v));
  animation();
}

void DynamicLighting::animation(){

  random16_add_entropy( random16());

// Array of temperature readings at each simulation cell

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < inner_leds; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((cooling * 10) / inner_leds) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= inner_leds - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2]  ) / 2;
    }
    //Wrap WIth Decay
    //heat[0] = (heat[inner_leds - 1] + heat[inner_leds - 2])/2;
    //heat[1] = (heat[0] + heat[inner_leds - 1])/2;

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < sparking ) {
      int y = random8(inner_leds-1);
      heat[y] = qadd8( heat[y], random8(150,240) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( uint8_t j = 0; j < inner_leds; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      uint8_t colorindex = scale8( heat[j], 240);
      //log("J: "+String(j)+" I:"+String(colorindex));
      leds[j] = ColorFromPalette( pallette, colorindex, uint8_t(255));
    }

    for (uint8_t i = LEDS_OUTSIDE_START; i < LEDS_OUTSIDE_END; i++){
      //uint8_t inx = i-LEDS_INSIDE_START;
      //log("I: "+String(inx)+" R:"+String(leds[inx].r)+" B:"+String(leds[inx].b)+" G:"+String(leds[inx].g));
      beem.lights.setPixelColorRGB(i, leds[i].r,leds[i].g,leds[i].b,0);
    }

}

void DynamicLighting::update()
{
  //log(  "Updating: " + name() );
}

String DynamicLighting::name(){
  return this -> mode_name;
}


//Gradient Lighting ////////////////////////////////////////////////////////////
void GradientLighting::initialize()
{
  colors[0] = CHSV(25,255,255);
  settings[0] = 180;
  pallette = HeatColors_p;
  start_index = 0;
  pos_index = halfWidth;
}

void GradientLighting::render()
{
  log(  "Rendering: " + name() );

  //Color Outside
  beem.lights.color_outside( beem.lights.hsvColortoRgbw( colors[0].h,
                                                         colors[0].s,
                                                         colors[0].v));
  animation();
}

void GradientLighting::animation(){


  NPS = (float)((( (settings[0] ) / float(127))-1)*RPS_MAX * maxWidth);
  V = NPS * ((float)(beem.thisTime - beem.lastTime) / float(1000000));
  // if (V > RPS_MAX * maxWidth){ V = RPS_MAX * maxWidth; }
  grad_pos += V;
  if (grad_pos > maxWidth){ grad_pos -= maxWidth;}
  else if (grad_pos < 0){ grad_pos += maxWidth;}
  pos_index = (uint8_t) grad_pos;

  fill_palette( leds, halfWidth, 0, incriment, pallette,
              (uint8_t) 255 * beem.lights.brightness_multiplier, LINEARBLEND);

  for (uint8_t i = halfWidth; i < maxWidth; i++){
    leds[i] = leds[maxWidth - i-1];
  }

  //Convert LEDS to RGBW for NeoPixelBus
  for (uint8_t i = 0; i < maxWidth; i++){
    index = pos_index + i;
    if ( index > maxWidth){ index -= maxWidth ; }
    else if ( index < 0){ index += maxWidth ; }
    //log("I: "+String(index + LEDS_INSIDE_START)+" R:"+String(leds[i].r)+" B:"+String(leds[i].b)+" G:"+String(leds[i].g),MODE_LOOP_DEBUG);
    beem.lights.setPixelColorRGB( index , leds[i].r, leds[i].g,leds[i].b,0);
  }
}

void GradientLighting::update()
{
  //log(  "Updating: " + name() );
}

String GradientLighting::name(){
  return this -> mode_name;
}

//Flicker Lighting ////////////////////////////////////////////////////////////
void Flicker::initialize()
{
  colors[0] = CHSV(25,255,255);
  settings[0] = 8;
  settings[1] = 120;
  settings[2] = 55;
  settings[3] = 55;
}

void Flicker::render()
{
  log(  "Rendering: " + name() );

  //Color Outside
  beem.lights.color_outside( beem.lights.hsvColortoRgbw( colors[0].h,
                                                         colors[0].s,
                                                         colors[0].v));
  animation();
}

void Flicker::animation(){

  random16_add_entropy( random16());

    // Step 1.  Randomly ignite new 'sparks'
    for( uint8_t i = 0; i < random8(num_sparks); i++) {
      uint8_t y = random8(NUM_LEDS-1); //random position
      if (sparking[y] == false && random8() > sparking_threshold){ //Start The spark at y if it isn't already
        sparking[y] = true;
        spark_val[y] = qadd8( spark_val[y], random8(heating)); //Heating
      }
    }

    //Step 2. Add Random Heat Until Max Value
    for( uint8_t i = 0; i < NUM_LEDS; i++) {
      if (sparking[i] == true){ //Add Heat Unitl Max, Then Set sparking= false
        spark_val[i] = qadd8( spark_val[i], random8(heating)); //Heating
        if ( spark_val[i] >= 255){
          sparking[i] = false; //Turn Off The Spark
        }
      }
      else{ // Its Cooling Down
        spark_val[i] = qsub8(spark_val[i] , random8(cooling));
      }
    }

    //Light LEDS
    for (uint8_t i = 0; i < NUM_LEDS; i++){
      beem.lights.setPixelColorHSV(i, colors[0].h,colors[0].s,spark_val[i]);
    }

}

void Flicker::update()
{
  num_sparks = settings[0];
  sparking_threshold = settings[1];
  heating  = settings[2];
  cooling  = settings[3];
}

String Flicker::name(){
  return this -> mode_name;
}












//FlickerPalette Lighting ////////////////////////////////////////////////////////////
void FlickerPalette::initialize()
{
  colors[0] = CHSV(25,255,255);
  settings[0] = 8;
  settings[1] = 120;
  settings[2] = 55;
  settings[3] = 55;
}

void FlickerPalette::render()
{
  log(  "Rendering: " + name() );

  //Color Outside
  beem.lights.color_outside( beem.lights.hsvColortoRgbw( colors[0].h,
                                                         colors[0].s,
                                                         colors[0].v));
  animation();
}

void FlickerPalette::animation(){

  random16_add_entropy( random16());

    // Step 1.  Randomly ignite new 'sparks'
    for( uint8_t i = 0; i < random8(num_sparks); i++) {
      uint8_t y = random8(NUM_LEDS-1); //random position
      if (sparking[y] == false && random8() > sparking_threshold){ //Start The spark at y if it isn't already
        sparking[y] = true;
        spark_val[y] = qadd8( spark_val[y], random8(heating)); //Heating
      }
    }

    //Step 2. Add Random Heat Until Max Value
    for( uint8_t i = 0; i < NUM_LEDS; i++) {
      if (sparking[i] == true){ //Add Heat Unitl Max, Then Set sparking= false
        spark_val[i] = qadd8( spark_val[i], random8(heating)); //Heating
        if ( spark_val[i] >= 255){
          sparking[i] = false; //Turn Off The Spark
        }
      }
      else{ // Its Cooling Down
        spark_val[i] = qsub8(spark_val[i] , random8(cooling));
      }
    }

    //Light LEDS
    for (uint8_t i = 0; i < NUM_LEDS; i++){
      CRGB color = ColorFromPalette( pallette, spark_val[i], uint8_t(255));
      beem.lights.setPixelColorRGB(i, color.r,color.g,color.b,0);
    }

}

void FlickerPalette::update()
{
  num_sparks = settings[0];
  sparking_threshold = settings[1];
  heating  = settings[2];
  cooling  = settings[3];
}

String FlickerPalette::name(){
  return this -> mode_name;
}





//Flash Lighting ////////////////////////////////////////////////////////////
void Flash::initialize()
{
  colors[0] = CHSV(25,255,255);
  settings[0] = 0;
}

void Flash::render()
{
  log(  "Rendering: " + name() );

  animation();
}

void Flash::animation(){


  if ( settings[0] == 1){
    beem.lights.color_all( beem.lights.hsvColortoRgbw(     colors[0].h,
                                                           colors[0].s,
                                                           colors[0].v));
  }
  else{
    beem.lights.allOff();
  }


}

void Flash::update()
{

}

String Flash::name(){
  return this -> mode_name;
}



//Flash Lighting ////////////////////////////////////////////////////////////
void DirectColor::initialize()
{
  //0 Blue
  settings[0] = 0;
  //1 Red
  settings[1] = 0;
  //2 Green
  settings[2] = 0;
  //3 White
  settings[3] = 255;
}

void DirectColor::render()
{
  log(  "Rendering: " + name() );

  animation();
}

void DirectColor::animation(){

    beem.lights.color_all( RgbwColor(settings[1],settings[2],settings[0],settings[3]) );

}

void DirectColor::update()
{

}

String DirectColor::name(){
  return this -> mode_name;
}












///////////////////////////////////////////////////////////////////////////////
//Mode Manager Defaults
///////////////////////////////////////////////////////////////////////////////
ModeManager::ModeManager(){
  // beem.com.log("Setting Up Modes");
  // setup_modes();
}

void ModeManager::switchToMode(unsigned int newModeIndex) {
  beem.com.log("GM "+String(modeIndex)+"->"+String(newModeIndex),true);
  beem.com.log("Leaving...",true);
  Serial.flush();
  leave();
  modeIndex = newModeIndex;
  beem.com.log("Initializing...",true);
  Serial.flush();
  initialize();
}

Mode* ModeManager::currentMode()
{
  return _modes.at(modeIndex);
};

void ModeManager::addMode( Mode *mode)
{
  _modes.push_back(mode);
}

void ModeManager::setup_modes(){
  addMode( &flicker );
  addMode( &solidcolor );
  addMode( &flashcolor );
  addMode( &dynamiclighting );
  addMode( &gradientlighting );
  addMode( &flickerpalette );
  addMode( &flash );
  addMode( &directcolor );

  //addMode( &pacman );
  //addMode( &firework );


  // addMode( &Test_Mode_2 );
  initialize(0);
}

void ModeManager::initialize() {
  beem.addObserver(_modes.at(modeIndex));
  currentMode() -> initialize();
}

void ModeManager::initialize( unsigned int customIndex) {
  modeIndex = customIndex;
  beem.addObserver(_modes.at(customIndex));
  beem.com.log( "Mode Type Init: " + String( _modes.at(customIndex) -> name()),true);
  currentMode() -> initialize();
}

void ModeManager::leave() {
  beem.removeObserver(_modes.at(modeIndex));
  currentMode() -> leave();
}

void ModeManager::update() {
  //beem.com.log("Update: " + name());
  currentMode() -> update();
}

void ModeManager::render() {
  if (beem.lights._on){
    //beem.com.log("Rendering Game");
    _modes.at(modeIndex) -> render();
    beem.lights.show();

  }
  else{
     //beem.com.log("Beem Off");
     beem.lights.allOff();
     beem.lights.show();
     //delay(SLEEP_INTERVAL); // Save Power Or Something
     esp_sleep_enable_timer_wakeup(SLEEP_INTERVAL * 1000000);
   }
   //Render Lights

}

String ModeManager::name(){
  return currentMode() -> name();
}

// const CRGBPalette16 myRedWhiteBluePalette_p  =
// {
//     CRGB::Red,
//     CRGB::Gray, // 'white' is too bright compared to red and blue
//     CRGB::Blue,
//     CRGB::Black,
//
//     CRGB::Red,
//     CRGB::Gray,
//     CRGB::Blue,
//     CRGB::Black,
//
//     CRGB::Red,
//     CRGB::Red,
//     CRGB::Gray,
//     CRGB::Gray,
//     CRGB::Blue,
//     CRGB::Blue,
//     CRGB::Black,
//     CRGB::Black
// };
