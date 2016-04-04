#ifndef CONSOLE_RADIO_H
#define CONSOLE_RADIO_H

#include "RF24.h"

class Color;

class Console_radio {
public:
	Console_radio(int pin1, int pin2) : radio(pin1, pin2)
	{};

	void init();
	bool send_color(const Color&, int);
	bool receive_color(Color&, int);
private:
	RF24 radio;
};

#endif
