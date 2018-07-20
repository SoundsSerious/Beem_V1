#ifndef EVENT_H
#define EVENT_H

#include <Arduino.h>
#include "state.h"
#include "subject.h"
#include "modes.h"
#include <queue>
#include <FastLED.h>
#include <NeoPixelBus.h>
//#import "observer.h"
//#include "globals.h"

//Event Guidelines:
//Never send an event from a place where events are handled.
//Memory Managment Is Up To You Guy (unique_ptr may help)

//The Visitor pattern defines which events go where (Modes, Audio, Image ect)

class Mode; //Gotta Predeclare
class Subject;

enum EventTypes {EVENT,
                MOTION_EVENT,
                        PeakHeightEvt,LandedEvt,FrameTickEvt,CatchEvt,LaunchEvt,
                COM_EVENT,
                        ModeColorEvt,ModeSelectEvt, ModePalletteEvt, ModeSettingEvt};

class IEvent
{
  // virtual void visit(State *s) =0;
  virtual void visit(Subject &sj) = 0;
  virtual void visit(Observer &o) =0;
  virtual void visit(Mode &g) = 0;//Doesn't Work... I think this is due to storage of observer pointers
  // virtual void visit(StateSwitch *s) =0; //State

  EventTypes type_  = EVENT;
  virtual EventTypes type()=0;
};

class Event: public IEvent{
public:
  Event(){created = micros();};
  virtual ~Event() {}; //Gotta Have A Virtual Distructor otherwise memory leak
  unsigned long created;
  EventTypes type_  = EVENT;
  //Should Return The Type Of Event
  // virtual void visit(State *s); //State
  virtual void visit(Subject &sj); //Subject
  virtual void visit(Observer &o); //Observer
  virtual void visit(Mode &g);
  // virtual void visit(StateSwitch *s); //State

  virtual EventTypes type();
};



//Moition Events
class MotionEvent: public Event
{
public:
  MotionEvent(){};
  MotionEvent( float omega ){ omega = omega;
                             created = micros();};
  virtual ~MotionEvent(){};
  EventTypes type_ = MOTION_EVENT;

  //Should Return The Type Of Event
  // virtual void visit(State *s); //State
  virtual void visit(Subject &sj); //Subject
  virtual void visit(Observer &o); //Observer
  virtual void visit(Mode &g);
  // virtual void visit(StateSwitch *s); //State

  virtual EventTypes type();

  float omega;
};

class PeakHeightEv: public MotionEvent {
public:
    PeakHeightEv( float peakHeightIn ){ peakHeight = peakHeightIn;};
    float peakHeight;
    EventTypes type_ = PeakHeightEvt;
    virtual EventTypes type();
};

class FrameTickEv: public MotionEvent {
public:
    EventTypes type_ = FrameTickEvt;
    virtual EventTypes type();
};

class LaunchEv: public MotionEvent {
public:
    EventTypes type_ = LaunchEvt;
    virtual EventTypes type();
};

class LandedEv: public MotionEvent {
public:
    LandedEv( float dX, float dY, float H ){
       flight_dx = dX;
       flight_dy = dY;
       flight_H = H;
     };
    float flight_dx, flight_dy, flight_H;
    EventTypes type_ = LandedEvt;
    virtual EventTypes type();
};

class CatchEv: public MotionEvent {
public:
    EventTypes type_ = CatchEvt;
    virtual EventTypes type();
};



//Communiication events
class COMEvent: public Event {
public:
  COMEvent(String pk, String sk, String arg)
  {
      primary_key = pk;
      secondary_key = sk;
      message = arg;
  };
  virtual ~COMEvent(){};
  EventTypes type_ = COM_EVENT;
  String primary_key = "";
  String secondary_key = "";
  String message = "";
  //Should Return The Type Of Event
  // virtual void visit(State *s); //State
  virtual void visit(Subject &sj); //Subject
  virtual void visit(Observer &o); //Observer
  virtual void visit(Mode &g); //Observer
  // virtual void visit(StateSwitch *s); //State

  virtual EventTypes type();
};

class ModeColorEv: public COMEvent {
public:
  ModeColorEv(uint8_t iI, uint8_t iH,uint8_t iS,uint8_t iV): COMEvent("GAM","COL","0,0,0")
  {
    H = iH;
    S = iS;
    V = iV;
    primary_key = "GAM";
    secondary_key = "COL";
    inx = iI;
    //message = String(R)+","String(G)+","String(B)+";";
  };
  uint8_t H,S,V,inx;
  EventTypes type_ = ModeColorEvt;
  CHSV color;
  //Should Return The Type Of Event

  virtual EventTypes type();
};


class ModePalletteEv: public COMEvent {
public:
  ModePalletteEv(uint8_t inx): COMEvent("GAM","COL","")
  {
    index = inx;
  };

  EventTypes type_ = ModePalletteEvt;
  int index;
  //Should Return The Type Of Event

  virtual EventTypes type();
};


class ModeSettingEv: public COMEvent{
  public:
  ModeSettingEv(uint8_t inx, uint8_t input): COMEvent("GAM","SET","")
  {
    index = inx;
    setting = input;
  }
  EventTypes type_ = ModeSettingEvt;
  uint8_t index;
  uint8_t setting;

  virtual EventTypes type();
};












class EventQueue{
  public:
    EventQueue(){};
    ~EventQueue() {};
  private:
    std::queue <Event*> _events;
  public:
    void processEntries(Subject &subject);
    void addEvent(Event * event);
    Event * nextEvent();

};

#endif //EVENT_H
