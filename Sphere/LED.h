#ifndef LED_H
#define LED_H

#include "Arduino.h"
#include "Color.h"

class LED {
public:
  LED(int LED_R_, int LED_G_, int LED_B_) : LED_R(LED_R_), LED_G(LED_G_), LED_B(LED_B_){
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
  }

  void setColor(int red, int green, int blue) {
    analogWrite(LED_R, 255-red);
    analogWrite(LED_G, 255-green);
    analogWrite(LED_B, 255-blue);

    current_color.set(red, green, blue);
  }

  void setColor(const Color& color) {
    analogWrite(LED_R, 255-color.r_val());
    analogWrite(LED_G, 255-color.g_val());
    analogWrite(LED_B, 255-color.b_val());

    current_color = color;
  }


  Color get_current_color() 
  { return current_color; }

  void blink(Color& blink_color, int num_times) {
    int delay_time = 200;
    Color off_color(0,0,0);
    for (int i = 0; i < num_times; ++i) {
      setColor(blink_color);
      delay(delay_time);
      setColor(off_color);
      delay(delay_time);
    }
  }

private:
  const int LED_R;
  const int LED_G;
  const int LED_B;

  Color current_color;
};

#endif
