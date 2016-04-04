#ifndef PATCH_H
#define PATCH_H

#include "LED.h"
#include "NFC_scanner.h"
#include "Console_radio.h"

class Patch {

public:
	//constructor
	Patch(int patch_id_, int sck_, int miso_, int mosi_, int ss_, int LED_R_, int LED_G_, int LED_B_) : 
	patch_id(patch_id_),patch_scanner(sck_,miso_,mosi_,ss_),patch_lights(LED_R_,LED_G_,LED_B_)
	{

	}

	void loop(Console_radio& radio)
	{
		//do patch things
	}

private:
	const int patch_id;
	LED patch_lights;
	NFC_scanner patch_scanner;

};

#endif


