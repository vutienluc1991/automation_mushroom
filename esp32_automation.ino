#include "data.h"


// Declare object for geting data from  eeprom
DATA eeprom;

void setup()
{
  // Do some debugging
  Serial.begin(115200);
  eeprom.ee_init();

  eeprom.ee_write(32, 64, "123456789asdfghjklqwertyuiopozxcvbnm12345678904234345435435435435435345435435435435435435435435435345");
  Serial.println("done writing!!!");
  delay(3);
  char buf[66];
  eeprom.ee_read(32, 64, &buf[0]);
  Serial.println("below is out put!!!!!");
  Serial.println(&buf[0]);
  
  
}

void loop()
{
  
}
