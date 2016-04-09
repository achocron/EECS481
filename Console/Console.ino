
#include "Patch.h"
#include "NFC_scanner.h"
#include "Console_radio.h"
#include <SPI.h>
#include <Wire.h>

// Patch 1
#define NFC1_SCK  (45)
#define NFC1_MOSI (41)
#define NFC1_SS   (39)
#define NFC1_MISO (43)

#define PATCH1_R    (2)
#define PATCH1_G    (3)
#define PATCH1_B    (4)


// Patch 2
#define NFC2_SCK  (44)
#define NFC2_MOSI (40)
#define NFC2_SS   (38)
#define NFC2_MISO (42)

#define PATCH2_R    (5)
#define PATCH2_G    (6)
#define PATCH2_B    (7)


#define NUM_PATCHES 2

Patch patch1(NFC1_SCK, NFC1_MISO, NFC1_MOSI, NFC1_SS, PATCH1_R, PATCH1_G, PATCH1_B);
Patch patch2(NFC2_SCK, NFC2_MISO, NFC2_MOSI, NFC2_SS, PATCH2_R, PATCH2_G, PATCH2_B);

Patch* pathes[NUM_PATCHES] = { &patch1, &patch2 };

#define LED_R 3
#define LED_G 5
#define LED_B 6

#define RADIO_CE 9
#define RADIO_CS 8

Console_radio radio(RADIO_CE, RADIO_CS);

void setup() {

  Serial.begin(115200);

  radio.init();

  for (int i = 0; i < NUM_PATCHES; ++i) {
    pathes[i]->init();
  }
}



void loop() {
  for (int i = 0; i < NUM_PATCHES; ++i) {
    pathes[i]->loop(radio);
  }
} // Loop


