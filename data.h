#ifndef DATA_H
#endif DATA_H

#include "EEPROM.h"

#define EEPROM_SIZE 512

//Configuration
#define EE_CONFIG 0
//The first 32 bytes is used only for configuration 
// - such as wifi mode or bluetooth mode
// - Which pin to use... etc...


//wifi ssid - the next 64 bytes
#define EE_WIFI_SSID 32
//wifi password - the next 32 bytes
#define EE_WIFI_PASS 96

//what left is for data - Remember there is only 512 Bytes of EEPROOM so use it wisely
// Should contains - schedule by time of the day, by threshold, default pump time
#define EE_DATA 128

// There are 512B of eeprom so use it wisely
class DATA{
  public:
    bool ee_write(int addr, int size, char* data);
    bool ee_read(int addr, int size, char* buf);
    bool ee_init();
  private:
    // Something need to keep private then lol
    bool status = 0;
};
