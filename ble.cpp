
#include "ble.h"

bool BLE::ble_init(char* name){
  this->ble.begin("mushroom");
  
  // if it has nothing to check always return 1 then
  return 1;
}

bool BLE::ble_read_data(char* buf, int size){
  if(this->ble->available()){
    while(this->ble->available()){
      
      
    }
  
}
bool BLE::ble_write_data(char* data){
  
  }

  // if has nothing to check always return 1 then
  return 1;
}
