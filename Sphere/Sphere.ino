/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include "Motor.h"
#include "Accelerometer.h"

#include <SPI.h>
#include "RF24.h"

#define MOTOR 3
#define LED_R 10
#define LED_G 9
#define LED_B 6
#define BUFFER_SIZE 16

#define SHAKE_THRESHOLD 5000

/* helper functions */

/* globals */
double last_Y;
double last_P;
double last_R;
double tol;
int curr_R, curr_G, curr_B;

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

Motor motor(MOTOR);
Accelerometer accelerometer;

void dmpDataReady() {
  accelerometer.mpuInterrupt = true;
}

void setColor(int red, int green, int blue)
{
    analogWrite(LED_R, 255-red);
    analogWrite(LED_G, 255-green);
    analogWrite(LED_B, 255-blue);
}

byte addresses[][6] = {"1Node","2Node"};

void setup() {

    accelerometer.init1();
    attachInterrupt(0,dmpDataReady,RISING);
    accelerometer.init2();
    tol = 0.1;
    // configure LED for output
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);

    
    if(accelerometer.constructed_successfully)
    {
      //blink led to signal successful initialization
      setColor(0,255,0);
      delay(100);
      setColor(0,0,0);
      delay(100);
      setColor(0,255,0);
      delay(100);
      setColor(0,0,0);
      delay(100);
      setColor(0,255,0);
      delay(100);
      setColor(0,0,0);
      delay(100);
    }
    

    radio.begin();
    
    // Set the PA Level low to prevent power supply related issues since this is a
    // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
    radio.setPALevel(RF24_PA_LOW);
    
    // Open a writing and reading pipe on each radio, with opposite addresses
    if(radioNumber){
      radio.openWritingPipe(addresses[1]);
      radio.openReadingPipe(1,addresses[0]);
    }else{
      radio.openWritingPipe(addresses[0]);
      radio.openReadingPipe(1,addresses[1]);
    }
    
    // Start the radio listening for data
    radio.startListening();
}

void loop() {

    // if programming failed, don't try to do anything
    if (!accelerometer.getDmpReady()) return;

    // wait for MPU interrupt or extra packet(s) available
    while (accelerometer.isFree()) {
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .vib
        // .
        // .
        /****************** Pong Back Role ***************************/

        motor.update();
        
        char message[BUFFER_SIZE];
        
        if( radio.available()){
                                                                       // Variable for the received timestamp
         while (radio.available()) {                                   // While there is data ready
           radio.read(message, sizeof(char) * BUFFER_SIZE );             // Get the payload
         }
        
         radio.stopListening();                                        // First, stop listening so we can talk
         char message_back[BUFFER_SIZE];
         snprintf(message_back, BUFFER_SIZE*sizeof(char), "%i,%i,%i", curr_R, curr_G, curr_B);
         
         radio.write(message_back, sizeof(char) * BUFFER_SIZE );              // Send the final one back.      
         radio.startListening();                                       // Now, resume listening so we catch the next packets.     
         
         Serial.print(F("Received: "));
         Serial.println((message));
         Serial.print(F("Sent response "));
         Serial.println((message_back));

         int msg_R;
         int msg_G;
         int msg_B;
         sscanf(message,"%i,%i,%i",&msg_R,&msg_G,&msg_B);
         char string[BUFFER_SIZE];
         sprintf(string,"R: %i,G: %i,B: %i", msg_R,msg_G,msg_B);
         Serial.println(string);

         setColor(msg_R,msg_G,msg_B);
         curr_R = msg_R;
         curr_G = msg_G;
         curr_B = msg_B;
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
     
    curr_R = abs(round(curr_R + diff_y) % 255);
    curr_G = abs(round(curr_G + diff_p) % 255);
    curr_B = abs(round(curr_B + diff_r) % 255);

    setColor(curr_R, curr_G, curr_B);
    
    double shake_magnitude = accelerometer.getShakeMagnitude();

  if (!motor.is_vibrating() && shake_magnitude > SHAKE_THRESHOLD) {
      motor.start_vibrating();
  }
} // Loop

