#include <Arduino.h>
#include <FastLED.h>
#include "globals.h"


//using namespace std;
//WARNING: I SUCK AT SPELLING + FEEL FREE TO FIX ANY ERRORS

Beem beem;

void on_off_buttonaction() {
	beem.com.log("BUTTON 0 PRESSED",true);
	beem.lights._on = !beem.lights._on;

	if (beem.lights._on){
		if (!beem.com.is_wifi_on){ beem.com.wifi_on();}
	}
	else{
		beem.lights.allOff();
		beem.lights.show();
		if (beem.com.is_wifi_on){ }//beem.com.wifi_off();}
	}
}

void setup() {

delay(100);
beem.initlaize();
//beem._com.log("Finish Setup");

pinMode(BUTTON, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(BUTTON),on_off_buttonaction, FALLING);

delay(1000);

}

void loop() {
	beem.update();
}
