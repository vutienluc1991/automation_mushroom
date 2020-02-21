
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

class BLE{
  public:
    bool ble_init(char* name);
    
    // reading 
    bool ble_read_data(char* buf, int size);
    bool ble_write_data(char* data);
    
  private:
    BluetoothSerial ble;
};

//void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
//  if(event == ESP_SPP_SRV_OPEN_EVT){
//    Serial.println("Client Connected");
//  }
// 
//  if(event == ESP_SPP_CLOSE_EVT ){
//    Serial.println("Client disconnected");
//  }
//}

