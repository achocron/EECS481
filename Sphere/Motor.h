#ifndef MOTOR_H
#define MOTOR_H

class Motor {
public:
  Motor(int pin_);

  void update();
  void start_vibrating();
  
  bool is_vibrating() 
  { return vibrating; }

private:
  unsigned int start_time;
  bool vibrating;

  const unsigned int duration;
  const int pin;
  const int on;
  const int off;
};

#endif
