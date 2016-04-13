#ifndef PATCH_H
#define PATCH_H

#include "LED.h"
#include "NFC_scanner.h"
#include "Console_radio.h"
#include "Color.h"

class Patch {
public:
	//constructor
	Patch(int sck_, int miso_, int mosi_, int ss_, unsigned int (&LED_indices_)[9], unsigned int (&rgbPixel_)[40], unsigned int nled_, int clockpin_, int datapin_);

 	// call init functions of member variables
  	void init();

	bool loop(Console_radio& radio);
	void setColor(const Color&);
	void show();
	void latchLeds(int n);
	unsigned int make_color(int r, int g, int b);
	Color getColor()
	{ return curr_color; }

private:
	unsigned int (&LED_indices)[9];
	unsigned int (&rgbPixel)[40];
	unsigned int nled;
	int clockpin;
	int datapin;
	NFC_scanner scanner;
	Color curr_color;
};

#endif


