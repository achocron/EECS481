
#ifndef NFC_SCANNER_H
#define NFC_SCANNER_H

#include "Arduino.h"

#include <Adafruit_PN532.h>

class NFC_scanner {
public:

  // constructor
  NFC_scanner(int sck_, int miso_, int mosi_, int ss_) :
  nfc(sck_, miso_, mosi_, ss_)
  {

  }

  void init()
  {
    Serial.println("NFC_scanner constructor begins");
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

  
  bool scan_for_id(int* id_out)
  {
    uint8_t success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    //Serial.println("**calling readPassiveTargetId");
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50);
    //Serial.println("**returning readPassiveTargetId");

    if (success) {
      // Display some basic information about the card
      Serial.println("Found an ISO14443A card");
      Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
      Serial.print("  UID Value: ");
      nfc.PrintHex(uid, uidLength);
      Serial.println("");

      uint32_t uid_full;
      uid_full = combine_shorts(uid);
      *id_out = get_id_from_uid(uid_full);
      return true;
      /* I THINK WE CAN GET RID OF ALL OF THIS BUT IDK
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
      */
    } 
    else {
      return false;
    }
  }



private:

  static const uint32_t[] = {23, 24 };


  // combine four uint8_t's into a single uint32_t
  static uint32_t combine_shorts(uint8_t* uid_arr)
  {
    uint32_t ret = 0;

    for (int i = 0; i < 4; i++) {
      ret |= uid_arr[i];

      // dont shift after last uint8_t
      if (i != 4 - 1) {
        ret <<= 8;  
      }
    }
    Serial.print("shorts combined into: ");
    Serial.println(ret);
    return ret;
  }

  static int get_id_from_uid(uint32_t uid)
  {
    return 1;
  }

  // original PN532 driver library
  Adafruit_PN532 nfc;
};


#endif
