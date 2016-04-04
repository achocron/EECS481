#ifndef PATCH_H
#define PATCH_H

#include "LED.h"
#include "NFC_scanner.h"
#include "Console_radio.h"

class Patch {
public:
	//constructor
	Patch(int sck_, int miso_, int mosi_, int ss_, int LED_R_, int LED_G_, int LED_B_);

 	// call init functions of member variables
  	void init();

	void loop(Console_radio& radio);

  	void setColor(int red, int green, int blue);

private:
	LED leds;
	NFC_scanner scanner;
};

#endif


