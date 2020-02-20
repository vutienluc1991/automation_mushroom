#include "data.h"


// Declare object for geting data from  eeprom
DATA eeprom;

void setup()
{
  // Do some debugging
  Serial.begin(115200);
  eeprom.ee_init();

  eeprom.ee_write(32, 64, "some thing is going on here");
  delay(3);
  char buf[64];
  eeprom.ee_read(32, 64, &buf[0]);
  Serial.println("below is out put!!!!!");
  for(int i = 0; i < 64; i++){
    Serial.print(int(buf[i]));
      
  }
  Serial.println();
  Serial.println(&buf[0]);
  
  
}

void loop()
{
  
}
