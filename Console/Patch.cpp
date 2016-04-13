
#include "Patch.h"
#include "Color.h"
#include "LED.h"
#include "NFC_scanner.h"
#include "LED_segment.h"
#include "Console_radio.h"
#include "Arduino.h"
#include "Radio_consts.h"

// constructor
Patch::Patch(int sck_, int miso_, int mosi_, int ss_, LED_segment* segment_) : 
  scanner(sck_,miso_,mosi_,ss_), segment(segment_)
{
  
}

void Patch::init()
{
  scanner.init();
}

bool Patch::loop(Console_radio& radio)
{
  int id_scanned;
  bool success = scanner.scan_for_id(&id_scanned);

  if (success) {
  	Serial.println("successfully read NFC tag");

  	bool sent = radio.send_color(getColor(), id_scanned);

  	if (sent) {
  		Serial.println("successfully sent color");
  		Color received_color;
  		bool received = radio.receive_color(received_color, id_scanned);
  		if (received) {
        Serial.print(received_color.c_str());
  			Serial.println("successfully received color");
  			setColor(received_color);
  		}
  	}
   return true;
  }
  return false;
}

void Patch::setColor(const Color& color)
{

  curr_color = color;

  Serial.print("Setting color: ");
  Serial.print(color.r_val());Serial.print(" ");
  Serial.print(color.g_val());Serial.print(" ");
  Serial.print(color.b_val());Serial.println(" ");

  segment->set_color(curr_color);
}


