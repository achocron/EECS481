
#include "LED_segment.h"

#include "Arduino.h"
#include "Color.h"



#define LED_DATA_PIN 4
#define LED_CLOCK_PIN 5

int LED_segment::datapin  = LED_DATA_PIN;
int LED_segment::clockpin = LED_CLOCK_PIN;

unsigned int LED_segment::rgbPixel[nled] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


LED_segment::LED_segment(const unsigned int min_index_, const unsigned int max_index_) :
  min_index(min_index_), max_index(max_index_)
{

}

void LED_segment::init_strip()
{
  pinMode(clockpin, OUTPUT);
  pinMode(datapin, OUTPUT);
  set_all_color(Color(0,0,0));
}


// static
void LED_segment::set_all_color(const Color& color)
{
  for (unsigned int i = 0; i <= nled; ++i) {
    rgbPixel[i] = make_color(color);
  }
  show();
}


void LED_segment::set_color(const Color& color)
{
  for (unsigned int i = min_index; i <= max_index; ++i) {
    rgbPixel[i] = make_color(color);
  }
  show();
}





void LED_segment::show()
{
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

void LED_segment::latchLeds(int n)
{
  digitalWrite(datapin, LOW);
  for(int i = 8 * n; i>0; i--) {
    digitalWrite(clockpin, HIGH);
    digitalWrite(clockpin, LOW);
  }
}

unsigned int LED_segment::make_color(const Color& color)
{
  // color are 3 groups of 5 bit (0..31)
  unsigned int r = color.r_val();
  unsigned int g = color.g_val();
  unsigned int b = color.b_val();
  r=r & 0x1F;
  g=g & 0x1F;
  b=b & 0x1F;
  return  (b << 10) | (r << 5) | g;
}

