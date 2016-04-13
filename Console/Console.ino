
#include "Patch.h"
#include "NFC_scanner.h"
#include "LED_segment.h"
#include "Console_radio.h"
#include <SPI.h>
#include <Wire.h>

// Patch 1
#define NFC1_SS   (30)
#define NFC1_MOSI (32)
#define NFC1_MISO (34)
#define NFC1_SCK  (36)

// Patch 2
#define NFC2_SS   (38)
#define NFC2_MOSI (40)
#define NFC2_MISO (42)
#define NFC2_SCK  (44)

// Patch 3
#define NFC3_SS   (22)
#define NFC3_MOSI (24)
#define NFC3_MISO (26)
#define NFC3_SCK  (28)

//LED Strip things
/*
#define nled 17
unsigned int rgbPixel[nled]; // rgb values of whole led strip
int datapin = 4;  // Led Strip St Pin
int clockpin = 5; // Led Strip Ci Pin
unsigned int patch1LEDs[] = {0,1,2,3,4};
unsigned int patch2LEDs[] = {6,7,8,9,10};
unsigned int patch3LEDs[] = {12,13,14,15,16,17};
*/

#define NUM_PATCHES 3

LED_segment segment1(1, 4);
LED_segment segment3(6, 10);
LED_segment segment2(12, 18);

Patch patch1(NFC1_SCK, NFC1_MISO, NFC1_MOSI, NFC1_SS, &segment1);
Patch patch2(NFC2_SCK, NFC2_MISO, NFC2_MOSI, NFC2_SS, &segment2);
Patch patch3(NFC3_SCK, NFC3_MISO, NFC3_MOSI, NFC3_SS, &segment3);

Patch* patches[NUM_PATCHES] = { &patch1, &patch2, &patch3 };

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



Color wheel(int WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 32)
  {
  case 0:
    r = 31 - WheelPos % 32;   //Red down
    g = WheelPos % 32;      // Green up
    b = 0;                  //blue off
    break;
  case 1:
    g = 31 - WheelPos % 32;  //green down
    b = WheelPos % 32;      //blue up
    r = 0;                  //red off
    break;
  case 2:
    b = 31 - WheelPos % 32;  //blue down
    r = WheelPos % 32;      //red up
    g = 0;                  //green off
    break;
  }

  return(Color(r,g,b));
}

void setup() {

  pinMode(13, OUTPUT);

  for (int i = 0; i < 10; i++) {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
  }

  Serial.begin(115200);

  LED_segment::init_strip();

  pinMode(SWITCH_PIN, INPUT);
  
  last_mode = !is_game_mode();

  radio.init();

  for (int i = 0; i < NUM_PATCHES; ++i) {
    patches[i]->init();
  }

  /*
  for (int i = 0; i < 96; ++i) {
    patches[0]->setColor(wheel(i));
    delay(100);
  }
  */
}

void loop() {
  /*
  for (int i = 0; i < NUM_PATCHES; ++i) {
    patches[i]->loop(radio);
  }
  */
  if (!is_game_mode()) {
    if(last_mode)
    {
      for (int i = 0; i < NUM_PATCHES; ++i) {
        patches[i]->setColor({0,255,0});
      }
    }
    for (int i = 0; i < NUM_PATCHES; ++i) {
      patches[i]->loop(radio);
    }
    last_mode = false;
    return;
  }
  //In game mode

  if (!last_mode) {
    last_mode = true;
    for (int i = 0; i < NUM_PATCHES; ++i) {
      patches[i]->setColor(Color(0,0,0));
    }
    patches[current_patch]->setColor(game_color);
    update_current_patch();
    return;
  }

  last_mode = true;
  
  bool scanned = patches[current_patch]->loop(radio);
  if (scanned) {
    update_current_patch();
  }
  

} // Loop

void update_current_patch()
{
  Patch* old_patch = patches[current_patch];
    
  Serial.println("_");
  Serial.println("****");
  Serial.println(current_patch);

  if (micros() % 2) {
    current_patch += 1;
  }
  else {
    current_patch += 2;
  }
  current_patch %= NUM_PATCHES;

  Serial.println(current_patch);
  Serial.println("****");
  Serial.println("_");

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
  bool result = digitalRead(SWITCH_PIN) == HIGH;
  if (result) {
    Serial.println("Game mode");
  }
  else {
    Serial.println("Normal mode");
  }
  return (result);
}
