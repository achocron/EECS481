
#include "Patch.h"
#include "NFC_scanner.h"
#include "Console_radio.h"
#include <SPI.h>
#include <Wire.h>

#define NFC1_SS   (30)
#define NFC1_MOSI (32)
#define NFC1_MISO (34)
#define NFC1_SCK  (36)

#define PATCH1_R    (2)
#define PATCH1_G    (3)
#define PATCH1_B    (4)


// Patch 2
#define NFC2_SS   (38)
#define NFC2_MOSI (40)
#define NFC2_MISO (42)
#define NFC2_SCK  (44)

#define PATCH2_R    (7)
#define PATCH2_G    (6)
#define PATCH2_B    (5)

// Patch 3
#define NFC3_SS   (22)
#define NFC3_MOSI (24)
#define NFC3_MISO (26)
#define NFC3_SCK  (28)

#define PATCH3_R    (7)
#define PATCH3_G    (6)
#define PATCH3_B    (5)


#define NUM_PATCHES 3

Patch patch1(NFC1_SCK, NFC1_MISO, NFC1_MOSI, NFC1_SS, PATCH1_R, PATCH1_G, PATCH1_B);
Patch patch2(NFC2_SCK, NFC2_MISO, NFC2_MOSI, NFC2_SS, PATCH2_R, PATCH2_G, PATCH2_B);
Patch patch3(NFC3_SCK, NFC3_MISO, NFC3_MOSI, NFC3_SS, PATCH3_R, PATCH3_G, PATCH3_B);

Patch* patches[NUM_PATCHES] = { &patch1, &patch2, &patch3 };

#define LED_R 3
#define LED_G 5
#define LED_B 6

#define RADIO_CE 9
#define RADIO_CS 53

Console_radio radio(RADIO_CE, RADIO_CS);

#define SWITCH_PIN 8

bool last_mode;
bool is_game_mode();
void broadcast_mode();
void update_current_patch();

Color game_color(100,0,0);
int current_patch = 0;

void setup() {

  last_mode = !is_game_mode();

  Serial.begin(115200);

  pinMode(SWITCH_PIN, INPUT);

  radio.init();

  for (int i = 0; i < NUM_PATCHES; ++i) {
    patches[i]->init();
  }
}

void loop() {

  if (!is_game_mode()) {
    for (int i = 0; i < NUM_PATCHES; ++i) {
      patches[i]->loop(radio);
    }
    return;
  }
  //In game mode

  if (is_game_mode() != last_mode) {
    last_mode = is_game_mode();
    for (int i = 0; i < NUM_PATCHES; ++i) {
      patches[i]->setColor(Color(0,0,0));
    }
    patches[current_patch]->setColor(game_color);
    update_current_patch();
    return;
  }

  bool scanned = patches[current_patch]->loop(radio);
  if (scanned) {
    update_current_patch();
  }

} // Loop

void update_current_patch()
{
  Patch* old_patch = patches[current_patch];

  if (current_patch == 1) {
    current_patch = 0;
  }
  else {
    current_patch = 1;
  }

patches[current_patch]->setColor(old_patch->getColor());
  old_patch->setColor(Color(0,0,0));
//  
 // current_patch %= NUM_PATCHES;
  Serial.println("update current patch");
  Serial.print(current_patch);
  Serial.println("");
}

bool is_game_mode()
{
  if (digitalRead(SWITCH_PIN) == HIGH) {
    Serial.println("Game mode");
  }
  else {
        Serial.println("Normal mode");
  }
  return (digitalRead(SWITCH_PIN) == HIGH);
}
