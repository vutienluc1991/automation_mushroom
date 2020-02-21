#include "data.h"

bool DATA::ee_init(){
  if (!EEPROM.begin(EEPROM_SIZE))
  { 
    this->status = 0;
    return 0;
  }
  else{
    this->status = 1;
    return 1;
  }
}

bool DATA::ee_write(int addr, int size, char* data){

  if(!this->status) return 0; // means failed to write
  int len = strlen(data);
  if(len >= size)
    len = size; // Avoid buffer-overflow
  Serial.println(len);
  // write data
  for(int i = 0; i < len; i++){
    EEPROM.write(addr + i, data[i]);
    //Serial.println(data[i]);
  }
  // clear old data afterward
  for(int i = len; i < size; i++){
    EEPROM.write(addr + i, 0);
  }
  
  EEPROM.commit(); // Commit after done writing
  return 1; // means writing successfully
}

bool DATA::ee_read(int addr, int size, char* buf){
  if(!this->status) return 0; // means failed to read
  for(int i = 0; i < size; i++){
    buf[i] = EEPROM.read(addr + i);
  }
  return 1; // means reading successfully 
}
