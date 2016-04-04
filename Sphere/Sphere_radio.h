#ifndef SPHERE_RADIO_H
#define SPHERE_RADIO_H

#include "RF24.h"

class Color;

class Sphere_radio {
public:
	Sphere_radio(int pin1, int pin2) :  radio(pin1, pin2)
	{};
	void init();
	bool send_color(const Color&);
	bool receive_color(Color&);
private:
	RF24 radio;
};

#endif
