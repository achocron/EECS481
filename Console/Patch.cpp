
#include "Patch.h"
#include "Color.h"
#include "LED.h"
#include "NFC_scanner.h"
#include "Console_radio.h"
#include "Arduino.h"
#include "Radio_consts.h"

// constructor
Patch::Patch(int sck_, int miso_, int mosi_, int ss_, unsigned int (&LED_indices_)[9], unsigned int (&rgbPixel_)[40], unsigned int nled_, int clockpin_, int datapin_) : 
  scanner(sck_,miso_,mosi_,ss_), LED_indices(LED_indices_), rgbPixel(rgbPixel_), nled(nled_), clockpin(clockpin_), datapin(datapin_)
{
  
}

void Patch::init()
{
  scanner.init();
  pinMode(clockpin,OUTPUT);
  pinMode(datapin,OUTPUT);
  setColor(Color(255,0,0));
}

bool Patch::loop(Console_radio& radio)
{
  show();
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
  curr_color.set(color.r_val(), color.g_val(), color.b_val());
  for(unsigned int i=0;i<9;i++)
  {
    rgbPixel[LED_indices[i]] = make_color(color.r_val(), color.g_val(), color.b_val());
  }
}

void Patch::show() {
  unsigned int ii,b1;
  for (ii=0; ii < nled; ii++ ) {

    digitalWrite(datapin,HIGH);
    digitalWrite(clockpin, HIGH);
    digitalWrite(clockpin, LOW);

    for ( b1=0x4000; b1; b1 >>= 1) {

      if(rgbPixel[ii] & b1) digitalWrite(datapin, HIGH);
      else                digitalWrite(datapin, LOW);
      digitalWrite(clockpin, HIGH);
      digitalWrite(clockpin, LOW);
    }
  }
  latchLeds(nled);
}
//*********************************************************
// activate new color pattern in ledstrip
void Patch::latchLeds(int n) {

  digitalWrite(datapin, LOW);
  for(int i = 8 * n; i>0; i--) {
    digitalWrite(clockpin, HIGH);
    digitalWrite(clockpin, LOW);
  }

}

unsigned int Patch::make_color(int r, int g, int b) {
  // color are 3 groups of 5 bit (0..31)

  r=r & 0x1F;
  g=g & 0x1F;
  b=b & 0x1F;
  return  (b << 10) | (r << 5) | g;
}
