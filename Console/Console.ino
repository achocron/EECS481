

#include <SPI.h>
#include "RF24.h"

#include <Wire.h>
#include <Adafruit_PN532.h>


// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (4)  // Not connected by default on the NFC Shield


// Or use this line for a breakout or shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
// also change #define in Adafruit_PN532.cpp library file
   #define Serial SerialUSB
#endif


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

void setup_nfc() {
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  Serial.println("Waiting for an ISO14443A Card ...");
}



void setup() {

  Serial.begin(115200);

  setup_leds();

  setup_radio();

  setup_nfc();
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

      /*
      if(strcmp(rec_msg,"hi") == 0)
      {
        setColor(255,255,255);
      }
      else
      {
        setColor(0,0,0);
      }
      */
      
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
      //Serial.print(F(", Round-trip delay "));
      //Serial.print(end_time-start_time);
      //Serial.println(F(" microseconds"));
  }
}

void loop_nfc() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    
    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ... 
      Serial.println("Seems to be a Mifare Classic card (4 byte UID)");
    
      // Now we need to try to authenticate it for read/write access
      // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
      Serial.println("Trying to authenticate block 4 with default KEYA value");
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    
    // Start with block 4 (the first block of sector 1) since sector 0
    // contains the manufacturer data and it's probably better just
    // to leave it alone unless you know what you're doing
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
    
      if (success)
      {
        Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
        uint8_t data[16];
    
        // If you want to write something to block 4 to test with, uncomment
    // the following line and this text should be read back in a minute
        //memcpy(data, (const uint8_t[]){ 'a', 'd', 'a', 'f', 'r', 'u', 'i', 't', '.', 'c', 'o', 'm', 0, 0, 0, 0 }, sizeof data);
        // success = nfc.mifareclassic_WriteDataBlock (4, data);

        // Try to read the contents of block 4
        success = nfc.mifareclassic_ReadDataBlock(4, data);
    
        if (success)
        {
          // Data seems to have been read ... spit it out
          Serial.println("Reading Block 4:");
          nfc.PrintHexChar(data, 16);
          Serial.println("");
      
          // Wait a bit before reading the card again

          send_message();
          
          delay(1000);
        }
        else
        {
          Serial.println("Ooops ... unable to read the requested block.  Try another key?");
        }
      }
      else
      {
        Serial.println("Ooops ... authentication failed: Try another key?");
      }
    }
    
    if (uidLength == 7)
    {
      // We probably have a Mifare Ultralight card ...
      Serial.println("Seems to be a Mifare Ultralight tag (7 byte UID)");
    
      // Try to read the first general-purpose user page (#4)
      Serial.println("Reading page 4");
      uint8_t data[32];
      success = nfc.mifareultralight_ReadPage (4, data);
      if (success)
      {
        // Data seems to have been read ... spit it out
        nfc.PrintHexChar(data, 4);
        Serial.println("");
    
        // Wait a bit before reading the card again
        delay(1000);
      }
      else
      {
        Serial.println("Ooops ... unable to read the requested page!?");
      }
    }
  }
}



void loop() {

  loop_nfc();
  
} // Loop

