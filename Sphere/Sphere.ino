#include "Motor.h"
#include "Accelerometer.h"
#include "LED.h"
#include "Sphere_radio.h"
#include "Color.h"
#include <SPI.h>

#define MOTOR 3
#define LED_R 10
#define LED_G 9
#define LED_B 6
#define BUFFER_SIZE 16

#define SHAKE_THRESHOLD 5000

/* globals */
double last_Y;
double last_P;
double last_R;

const double tol = 0.1;

#define RADIO_CE 7
#define RADIO_CS 8

Sphere_radio radio(RADIO_CE,RADIO_CS);
Motor motor(MOTOR);
Accelerometer accelerometer;
LED ballLights(LED_R,LED_G,LED_B);

void dmpDataReady() {
  accelerometer.mpuInterrupt = true;
}

void setup() {

    radio.init();
    accelerometer.init1();
    attachInterrupt(0,dmpDataReady,RISING);
    accelerometer.init2();
 
    if(accelerometer.constructed_successfully) {
      Color green_color(0,255,0);
      //blink led to signal successful initialization
      ballLights.blink(green_color, 3);
    }
}

void loop() {
    motor.update();

    // if programming failed, don't try to do anything
    if (!accelerometer.getDmpReady()) return;

    // wait for MPU interrupt or extra packet(s) available
    while (accelerometer.isFree()) {
        motor.update();
          
        Color color_received;
        bool received_color = radio.receive_color(color_received);
        if (received_color) {
          radio.send_color(ballLights.get_current_color());
          ballLights.setColor(color_received);
          motor.start_vibrating();
        }
    }
    accelerometer.operate();

    static bool changed_once = false;
    if (!changed_once) {
      last_Y = accelerometer.getYaw();
      last_P = accelerometer.getPitch();
      last_R = accelerometer.getRoll();
      changed_once = true;
      return;
    }
    
    double diff_y = last_Y - accelerometer.getYaw();
    double diff_p = last_P - accelerometer.getPitch();
    double diff_r = last_R - accelerometer.getRoll();

    last_Y = accelerometer.getYaw();
    last_P = accelerometer.getPitch();
    last_R = accelerometer.getRoll();

    Color next_color(abs(round(ballLights.get_current_color().r_val() + diff_y) % 255),
                     abs(round(ballLights.get_current_color().g_val() + diff_p) % 255),
                     abs(round(ballLights.get_current_color().b_val() + diff_r) % 255));

    ballLights.setColor(next_color);
    
    double shake_magnitude = accelerometer.getShakeMagnitude();

  if (!motor.is_vibrating() && shake_magnitude > SHAKE_THRESHOLD) {
      motor.start_vibrating();
  }
} // Loop
