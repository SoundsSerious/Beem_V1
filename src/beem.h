

#include <Arduino.h>
#include "lights.h"
#include "communication.h"
#include "state.h"
#include "entity.h"
#include "subject.h"
#include "event.h"
#include "modes.h"
//#include "physics.h"
#include "system_info.h"

#ifndef BEEM_H
#define BEEM_H

using namespace std;

//In which we combine Lights + Motion + Modes
class Beem: public Subject, public Entity
{
public:
  //Constructor
  Beem():  lights(), com(){}; //mpu(MPU_SPI_CLOCK,MPU_CS),
  virtual ~Beem() {}; //Destructor (of doooom)

  //Physical parameters

  //States
  // StateSwitch powerState;
  // StateSwitch motionState;
  // StateSwitch lightMode;
  // StateSwitch gameMode;

  //Hardware
  //MPU9250 mpu;
  Lights lights;

  //Physics
  //Physics physics;

  //Connection
  COM com;

  //Modes
  ModeManager modes_manager;

  //Events
  EventQueue event_queue;

  //Important Entity Functions
  virtual void initlaize();
  virtual void update();

  //Parameters For Interloop
  int start;
  int targetFPS = 50;
  uint32_t budgetRefreshTime = 2500;
  uint32_t renderInterval = (1000000 / targetFPS) - budgetRefreshTime; //Microseconds aka 60fps
  //Sandbox Functions
  unsigned long lastTime,thisTime;
  int lightOffset = 0;
  float thetaOffset = 0;
  float degPerPixel = 360/ NUM_LEDS ;
  void updateThetaOffset();


};

#endif //FRISBEEM_H
