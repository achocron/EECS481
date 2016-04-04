
#include "Patch.h"

#include "Arduino.h"

#include "LED.h"
#include "NFC_scanner.h"
#include "Console_radio.h"


// constructor
Patch::Patch(int sck_, int miso_, int mosi_, int ss_, int LED_R_, int LED_G_, int LED_B_) : 
  scanner(sck_,miso_,mosi_,ss_), leds(LED_R_,LED_G_,LED_B_)
{

}

void Patch::init()
{
  scanner.init();
  setColor(5, 5, 5);
}

void Patch::loop(Console_radio& radio)
{
  int id_scanned;
  bool success = scanner.scan_for_id(&id_scanned1);
  if (success) {
    Serial.println("SUCCESS");
  }
}

void Patch::setColor(int red, int green, int blue)
{
  leds.setColor(red, green, blue);
}
