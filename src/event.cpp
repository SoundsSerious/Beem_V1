#include "event.h"
#include "globals.h"
#include "system_info.h"

// void Event::visit(State *s) { s -> handleInput(*this); }; //State
void Event::visit(Subject &sj) { beem.com.log("Event -> Subject");
                                 sj.notify(*this); }; //Subject
void Event::visit(Observer &o) { beem.com.log("Event -> Observer");
                                 o.onNotify(this); }; //Observer
void Event::visit(Mode     &g) { beem.com.log("Event -> Mode");
                                      g.onNotify(this); }; //Mode
// void Event::visit(StateSwitch *s){ s -> handleInput(*this); }; //State
EventTypes Event::type() {return this -> type_;};


// void MotionEvent::visit(State *s) { s -> handleInput(*this); }; //State
void MotionEvent::visit(Subject &sj) { beem.com.log("Motion -> Subject");
                                       sj.notify(*this); }; //Subject
void MotionEvent::visit(Observer &o) { beem.com.log("Motion -> Observer");
                                       o.onNotify(this); }; //Observer
void MotionEvent::visit(Mode     &g) { beem.com.log("Motion -> Mode");
                                       g.onNotify(this); }; //Mode
// void MotionEvent::visit(StateSwitch *s){ s -> handleInput(*this); }; //State
EventTypes MotionEvent::type() {return this -> type_;};
EventTypes PeakHeightEv::type() {return this -> type_;};
EventTypes FrameTickEv::type() {return this -> type_;};
EventTypes LaunchEv::type() {return this -> type_;};
EventTypes LandedEv::type() {return this -> type_;};
EventTypes CatchEv::type() {return this -> type_;};

// void COMEvent::visit(State *s) { s -> handleInput(*this); }; //State
void COMEvent::visit(Subject &sj) { beem.com.log("COM -> Subject");
                                    sj.notify(*this); }; //Subject
void COMEvent::visit(Observer &o) { beem.com.log("COM -> Observer");
                                    o.onNotify(this); }; //Observer
void COMEvent::visit(Mode     &g) { beem.com.log("COM -> Mode");
                                    g.onNotify(this); }; //Mode
EventTypes COMEvent::type() {return this -> type_;};
EventTypes ModeColorEv::type() {return this -> type_;};
EventTypes ModePalletteEv::type() {return this -> type_;};
EventTypes ModeSettingEv::type() {return this -> type_;};














void EventQueue::processEntries(Subject &subject)
{ if (!_events.empty()){
    //beem.com.log("We Have Events: "+String(_events.size()));
    while (!_events.empty()){
       Event * event= nextEvent();
       String msg = String(event -> type());
       event -> visit( subject );

       //Save The Memory, Man. Fuck Alzheimers
       _events.pop(); // Dont Stop
       delete event; //Needs A Virtual Destructor
    }
  }
  else{
    //No Events
  }
}







Event * EventQueue::nextEvent()
{
  return _events.front();
}

void EventQueue::addEvent(Event * event)
{
  _events.push(event);
}
