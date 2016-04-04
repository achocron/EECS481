

#include <SPI.h>
#include "RF24.h"

#include <Wire.h>

#include "NFC_scanner.h"

#define NFC1_SCK  (53)
#define NFC1_MOSI (49)
#define NFC1_SS   (47)
#define NFC1_MISO (51)

#define NFC2_SCK  (52)
#define NFC2_MOSI (48)
#define NFC2_SS   (46)
#define NFC2_MISO (50)


NFC_scanner nfc1(NFC1_SCK, NFC1_MISO, NFC1_MOSI, NFC1_SS);
NFC_scanner nfc2(NFC2_SCK, NFC2_MISO, NFC2_MOSI, NFC2_SS);

#define BUFFER_SIZE 16
#define LED_R 3
#define LED_G 5
#define LED_B 6

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

int curr_red;
int curr_green;
int curr_blue;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);

byte addresses[][6] = {"1Node","2Node"};

void setColor(int red, int green, int blue)
{
    curr_red = red;
    curr_green = green;
    curr_blue = blue;
    analogWrite(LED_R, 255-red);
    analogWrite(LED_G, 255-green);
    analogWrite(LED_B, 255-blue);
}

void setup_leds() {
  // configure LED for output
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  setColor(5, 5, 5);
}

void setup_radio() {
  //Serial.println(F("RF24/examples/GettingStarted"));
  //Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
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


void setup() {

  Serial.begin(115200);

  nfc1.init();
  nfc2.init();
  //setup_leds();

  //setup_radio();

  //setup_nfc();
}

void send_message() {

  radio.stopListening();                                    // First, stop listening so we can talk.
    
  Serial.println(F("Now sending"));

  //unsigned long start_time = micros();                             // Take the time, and send it.  This will block until complete
  char msg[BUFFER_SIZE];
  snprintf(msg, sizeof(char) * BUFFER_SIZE, "%i,%i,%i", curr_red, curr_green, curr_blue);
   if (!radio.write( msg , sizeof(char)*BUFFER_SIZE )){
     Serial.println(F("failed"));
   }
      
  radio.startListening();                                    // Now, continue listening
  
  unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
  boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
  
  while ( ! radio.available() ){                             // While nothing is received
    if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
        timeout = true;
        break;
    }      
  }
  
  char rec_msg[BUFFER_SIZE] = "";
  if ( timeout ){                                             // Describe the results
      Serial.println(F("Failed, response timed out."));
      Serial.println(rec_msg);
      //Serial.println(strcmp(rec_msg,"hi"));
      for(unsigned int i = 0;i < BUFFER_SIZE; i++)
      {
        rec_msg[i] = 0;
      }
      //setColor(0,0,0);
  }else{
                                       // Grab the response, compare, and send to debugging spew
      radio.read( rec_msg, sizeof(char)*BUFFER_SIZE);

      // Spew it
      Serial.print(F("Sent "));
      Serial.print(msg);
      Serial.print(F(", Got response "));
      Serial.println(rec_msg);

      int r,g,b;
      sscanf(rec_msg,"%i,%i,%i",&r,&g,&b);
      snprintf(msg, sizeof(char) * BUFFER_SIZE, "%i,%i,%i", r, g, b);

      Serial.println(msg);

      setColor(r,g,b);
  }
}

void loop() {

  //loop_nfc();
  int id_scanned1;
  bool success1 = nfc1.scan_for_id(&id_scanned1);
  if (success1) {
    Serial.println("SUCCESS 1");
  }

  int id_scanned2;
  bool success2 = nfc2.scan_for_id(&id_scanned2);
  if (success2) {
    Serial.println("SUCCESS 2");
  }


  
} // Loop


