
#include "Patch.h"
#include "Color.h"
#include "LED.h"
#include "NFC_scanner.h"
#include "Console_radio.h"
#include "Arduino.h"


// constructor
Patch::Patch(int sck_, int miso_, int mosi_, int ss_, int LED_R_, int LED_G_, int LED_B_) : 
  scanner(sck_,miso_,mosi_,ss_), leds(LED_R_,LED_G_,LED_B_)
{

}

void Patch::init()
{
  scanner.init();
  setColor(Color(5,5,5));
}

void Patch::loop(Console_radio& radio)
{
  int id_scanned;
  bool success = scanner.scan_for_id(&id_scanned);

  if (success) {
  	Serial.println("successfully read NFC tag");

  	bool sent = radio.send_color(leds.get_current_color(), id_scanned);

  	if (sent) {
  		Serial.println("successfully sent color");
  		Color received_color;
  		bool received = radio.receive_color(received_color, id_scanned);
  		if (received) {
  			Serial.println("successfully received color");
  			setColor(received_color);
  		}
  	}
  }
}

void Patch::setColor(const Color& color)
{
  leds.setColor(color);
}

