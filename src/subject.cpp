#include "subject.h"
#include "globals.h"

void Subject::addObserver(Observer  *observer)
{
  if ( this -> observers.size() < this -> observers.max_size()){
    uint8_t inx = this -> observers.size();//numObservers_+1;
    beem.com.log("Adding Observer: "+String(inx),true);
    this -> observers.push_back(observer); //-> observer;
    observer ->_index = this -> observers.size()-1;
//    numObservers_ += 1;
  }
  else{ beem.com.log("Too Many Observers");}
}

void Subject::removeObserver(Observer *observer)
{     //Shift Down Other Observers
  beem.com.log("Removing Observer OF " + String(observers.size()),true);
  if (observers.size() > 0){
     uint8_t inx = observer -> _index;
     beem.com.log("Removing Observer: "+String(inx),true);
     this -> observers.erase(this -> observers.begin()+inx);
     //for( int i = inx; i < MAX_OBSERVERS - 1; i++) observers_[i] = observers_[i + 1];
     //numObservers_ -= 1;
  }
}

void Subject::notify(Event &event)
{
  beem.com.log("Notifying "+String( this -> observers.size()) + " Observers");
  if (this -> observers.size()){
    for (unsigned int i = 0; i < this -> observers.size(); i++)
    {
      //We Cast TO Types As This Gives Us Greater Control Over Event Flow...
      ObserverType thisType = observers.at(i) -> type();
      switch(thisType){
        case OBSERVER:
          //Use Visitor Method
          beem.com.log("Notifying Observer: #"+String(i));
          event.visit( *observers.at(i) );
          break;
        case GAME:
          //Cast Class To Mode
          beem.com.log("Notifying Mode: #"+String(i));
          Mode* game = static_cast<Mode*>(observers.at(i));
          //Use Visitor Method
          event.visit( *game );
      }
    }
  }
}
