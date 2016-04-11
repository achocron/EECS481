
#ifndef NFC_SCANNER_H
#define NFC_SCANNER_H

#include "Arduino.h"

#include <Adafruit_PN532.h>

#define REPEAT_DELAY 2000 // 2 seconds
#define SCAN_DELAY 30 // 30 ms

class NFC_scanner {
public:

  // constructor
  NFC_scanner(int sck_, int miso_, int mosi_, int ss_);

  // initialize hardware components, call in setup()
  void init();

  // If id is found in SCAN_DELAY ms, set id_out to corresponding
  // sphere and return true. Otherwise return false.
  bool scan_for_id(int* id_out);



private:

  static constexpr uint32_t sphere_ids[2] = {23, 24 };

  // convert uint32_t from nfc tag uid into sphere id
  static int get_id_from_uid(uint32_t uid);

  // combine four uint8_t's into a single uint32_t
  static uint32_t combine_shorts(uint8_t* uid_arr);

  // original PN532 driver library
  Adafruit_PN532 nfc;
  unsigned long last_read_time;
};


#endif
