#ifndef PATCH_H
#define PATCH_H

#include "LED.h"
#include "NFC_scanner.h"
#include "LED_segment.h"
#include "Console_radio.h"
#include "Color.h"

class Patch {
public:
	//constructor
	Patch(int sck_, int miso_, int mosi_, int ss_, LED_segment* segment_);

 	// call init functions of member variables
  	void init();

	bool loop(Console_radio& radio);
	void setColor(const Color& color);
	
	Color getColor()
	{ return curr_color; }

private:
  
	LED_segment* segment;
	NFC_scanner scanner;
	Color curr_color;
};

#endif


