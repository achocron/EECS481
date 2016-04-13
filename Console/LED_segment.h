#ifndef LED_SEGMENT_H
#define LED_SEGMENT_H

#include "Arduino.h"

#include "Color.h"


#define nled 18


class LED_segment {
public:
  // constructor
  LED_segment(const unsigned int min_index, 
              const unsigned int max_index);

  static void init_strip();
  static void set_all_color(const Color& color);

  void set_color(const Color& color);



private:

  const unsigned int min_index;
  const unsigned int max_index;

  static void show();
  static void latchLeds(int n);
  static unsigned int make_color(const Color& color);

  static unsigned int rgbPixel[nled];

  static int clockpin;
  static int datapin;

};
#endif
