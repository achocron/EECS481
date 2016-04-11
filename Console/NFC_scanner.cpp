#include "NFC_scanner.h"

#include <Adafruit_PN532.h>


// constructor
NFC_scanner::NFC_scanner(int sck_, int miso_, int mosi_, int ss_) :
  nfc(sck_, miso_, mosi_, ss_), last_read_time(0)
{

}

void NFC_scanner::init()
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

bool NFC_scanner::scan_for_id(int* id_out)
  {
    unsigned long curr_time = millis();

    // last_read_time initialized to 0
    if (last_read_time != 0 && curr_time - last_read_time < REPEAT_DELAY) {
      return false;
    }

    uint8_t success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    //Serial.println("**calling readPassiveTargetId");
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, SCAN_DELAY);
    //Serial.println("**returning readPassiveTargetId");

    if (success) {

      last_read_time = millis();

      // Display some basic information about the card
      Serial.println("Found an ISO14443A card");
      Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
      Serial.print("  UID Value: ");
      nfc.PrintHex(uid, uidLength);
      Serial.println("");

      uint32_t uid_full;
      uid_full = combine_shorts(uid);
      *id_out = get_id_from_uid(uid_full);
            Serial.println("***");

      Serial.println(*id_out);
                  Serial.println("***");

      return true;
    } 
    else {
      return false;
    }
  }

  // static
  uint32_t NFC_scanner::combine_shorts(uint8_t* uid_arr)
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

  // static
  int NFC_scanner::get_id_from_uid(uint32_t uid)
  {
    return 1;
  }






