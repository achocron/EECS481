#ifndef LED_H
#define LED_H

#include "Arduino.h"

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

    curr_R = red;
    curr_G = green;
    curr_B = blue;
  }

  int get_curr_R() {
    return curr_R;
  }

  int get_curr_G() {
    return curr_G;
  }

  int get_curr_B() {
    return curr_B;
  }

  

private:
  const int LED_R;
  const int LED_G;
  const int LED_B;

  int curr_R;
  int curr_G;
  int curr_B;
};

#endif
