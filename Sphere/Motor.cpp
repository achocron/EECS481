#include "Motor.h"
#include "Arduino.h"

Motor::Motor(int pin_) : pin(pin_), 
vibrating(false), start_time(0), on(150), off(0), duration(1000)
{ 
  pinMode(pin, OUTPUT); 
}

void Motor::update() {
  if (vibrating && (millis() - start_time) > duration) {
      analogWrite(pin, off);
      vibrating = false;
  }     
}

void Motor::start_vibrating() { 
  start_time = millis(); 
  vibrating = true;
  analogWrite(pin, on);
}
