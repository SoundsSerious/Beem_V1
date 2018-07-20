#include "beem.h"


void Beem::initlaize(){
   //Initalize communication
   com.initialize();
   com.log("Communication Started...");

  // //Update MPU & Physics
   com.log("Go For Brains");
   //mpu.initialize();
   //mpu.update();
   com.log("Go For World");
   //physics.initialize();

   com.log("Go For Fun");
   games_manager.setup_modes();
  // //Update Strip
   com.log("Go For Lights");
   lights.initlaize();
   com.log("Go For Loop");


   com.start_cycle();
}


void Beem::update(){
  //Handle Loop Dynamics
  lastTime = thisTime;
  thisTime = micros();
  //Open COM to end client if conditions are correct
  if (PRINT_MAINLOOP) {com.log("Handshakes, Formalities, Ect...");}
  com.open();
  //Tick The Log So It Can Output Periodically

  com.tick();
  if (PRINT_MAINLOOP) {com.log("Beeming Into Space...");}
  //Update COM layer
  com.update();

  //Handle Other Stuff
  if (PRINT_MAINLOOP) {com.log("Updating...");}
  //Update MPU
  start = micros();
  //Physics Update Inner Loop
  while ( micros() - start < renderInterval) {

    if (PRINT_MAINLOOP) {com.log("Do Da Queue");}
    yield();
    event_queue.processEntries(*this);
    yield();

    games_manager.update();
  }
  //create newEvent and process events (circular buffer)
  if (PRINT_MAINLOOP) {com.log("Puttin On The High Beems!");}
  games_manager.render();
  //processMotion();
  //Notify Observers Of new Event
  // com.log("Hollerin'");
  //Send Event To Current Motion State
  // com.log("Making Decisions");

  //Close COM to end client
  com.close();
  yield();

}


//DOn't Use These...
// void Beem::notify( Event &event)
// {
//   com.log("Event -> Beem",true);
// }

// void Beem::notify( COMEvent &event)
// {
//   com.log("COM -> Beem",true);
// }

// void Beem::notify( COM_ModeSelect &event)
// {
//   //Select Mode
//   com.log("Switching Modes: "+ String(event.selectedMode),true);
//   this -> games_manager.switchToMode( (unsigned int) event.selectedMode );
// }
// void Beem::updateThetaOffset()
// {
//   thisTime = micros();
//
//   //Integrate For Theta
//   if ( abs(mpu.G.z) > 0.1){
//     thetaOffset -= mpu.G.z * (thisTime - lastTime) / 1000000;
//   }
//   //Catch & Adjust For Theta Over Limit
//   if (thetaOffset > 360){
//     thetaOffset -= 360;
//   }
//   else if (thetaOffset < 0){
//     thetaOffset += 360;
//   }
//
//   lightOffset = thetaOffset / degPerPixel;
//
//   //Preserve Time Calculations
//   lastTime = thisTime;
// }

// void Beem::processMotion()
// { //Generate A New Event & Add to circular buffer after deleting current item
//   //Serial.print("Getting New Event #");Serial.println(_eventCount);
//   currentMotionEvent = genNextEvent();
//   currentMotionEvent.visit( *this );//Subject Call notify()
//   //currentMotionEvent.visit( &motionState );//StateSwitchCall
//   //_motionState._states.back() -> handleInput( currentMotionEvent ); //Hack For Motion Direct
//   //currentMotionEvent.visit( _powerState);//StateSwitchCall
//
// }
//
// MotionEvent Beem::genNextEvent()
// { //For Now Look At Omegea (w)
//   return MotionEvent( mpu.G.z );
// }
