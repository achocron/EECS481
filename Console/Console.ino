
#include "Patch.h"
#include "NFC_scanner.h"
#include "Console_radio.h"
#include <SPI.h>
#include <Wire.h>

// Patch 1
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

#define PATCH2_R    (7)
#define PATCH2_G    (6)
#define PATCH2_B    (5)


#define NUM_PATCHES 2

Patch patch1(NFC1_SCK, NFC1_MISO, NFC1_MOSI, NFC1_SS, PATCH1_R, PATCH1_G, PATCH1_B);
Patch patch2(NFC2_SCK, NFC2_MISO, NFC2_MOSI, NFC2_SS, PATCH2_R, PATCH2_G, PATCH2_B);

Patch* patches[NUM_PATCHES] = { &patch1, &patch2 };

#define LED_R 3
#define LED_G 5
#define LED_B 6

#define RADIO_PIN1 9
#define RADIO_PIN2 8

Console_radio radio(RADIO_PIN1, RADIO_PIN2);

bool last_mode;
bool is_game_mode();
void broadcast_mode();
void update_current_patch();

Color game_color(100,0,0);
int current_patch = 0;

void setup() {

  last_mode = !is_game_mode();

  Serial.begin(115200);

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
  return false;
}
