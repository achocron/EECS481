
#include "Patch.h"
#include "NFC_scanner.h"
#include "Console_radio.h"
#include <SPI.h>
#include <Wire.h>

// Patch 1
#define NFC1_SCK  (53)
#define NFC1_MOSI (49)
#define NFC1_SS   (47)
#define NFC1_MISO (51)

#define PATCH1_R    (2)
#define PATCH1_G    (3)
#define PATCH1_B    (4)


// Patch 2
#define NFC2_SCK  (52)
#define NFC2_MOSI (48)
#define NFC2_SS   (46)
#define NFC2_MISO (50)

#define PATCH2_R    (5)
#define PATCH2_G    (6)
#define PATCH2_B    (7)


#define NUM_PATCHES 2


/*
	We decided that it makes sense for the ball and console to each have a dedicated game mode. 
	Console and ball both have a game mode. When ball is turned on, it asks the console what mode to be in.
	When the Console's mode is changed, it notifies all the balls to change modes.
	When a ball asks the console what mode it's in, and the console is off, the ball should timeout and just go
	into regular mode.
*/ 

Patch patch1(NFC1_SCK, NFC1_MISO, NFC1_MOSI, NFC1_SS, PATCH1_R, PATCH1_G, PATCH1_B);
Patch patch2(NFC2_SCK, NFC2_MISO, NFC2_MOSI, NFC2_SS, PATCH2_R, PATCH2_G, PATCH2_B);

Patch* patches[NUM_PATCHES] = { &patch1, &patch2 };

#define LED_R 3
#define LED_G 5
#define LED_B 6

#define RADIO_PIN1 7
#define RADIO_PIN2 8

Console_radio radio(RADIO_PIN1, RADIO_PIN2);

void setup() {

  Serial.begin(115200);

  radio.init();

  for (int i = 0; i < NUM_PATCHES; ++i) {
    patches[i]->init();
  }
}



void loop() {
  for (int i = 0; i < NUM_PATCHES; ++i) {
    patches[i]->loop(radio);
  }
} // Loop






