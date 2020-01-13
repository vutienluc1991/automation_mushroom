/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"
/************************* DHT Configuration *********************************/
#define DHTTYPE  DHT21
#define DHTPIN   D3

/*timmer*/
#define COUNTDOWN 120000
uint16_t g_begin = 0;
bool g_turnflag = false;
bool g_pump_on = false;
/************************* WiFi Access Point *********************************/
#define WLAN_SSID       "Dam Thanh Cong"
#define WLAN_PASS       "damnghilon"

/************************* Adafruit.io Setup *********************************/
#define DEBUGGING
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883 // use 8883 for SSL
#define AIO_USERNAME "vutienluc1991"
#define AIO_KEY "aio_WGss68c16p5ate1EhY05eMQBPYsO"
#define MQTT_SUBSCRIBE "/feeds/onoff" //"control"
#define MQTT_PUBLISH "/feeds/photocell" //"data"
/************ Global State (you don't need to change 
/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();


/**** DHT Functions ****/
DHT dht(DHTPIN, DHTTYPE);

void setup() {


  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  delay(10);
  dht.begin();
  #ifdef DEBUGGING
    Serial.begin(115200);
    Serial.println(F("Adafruit MQTT demo"));
  
    // Connect to WiFi access point.
    Serial.println(); Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WLAN_SSID);
  #endif
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #ifdef DEBUGGING
      Serial.print(".");
    #endif
  }

  #ifdef DEBUGGING
    Serial.println();
  
    Serial.println("WiFi connected");
    Serial.println("IP address: "); Serial.println(WiFi.localIP());
  #endif
  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoffbutton);
}



void loop() {
  char* beta = reading_dht();
  char data[64];
  memcpy(data, beta, 64);
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();
  
  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here
  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      #ifdef DEBUGGING
        Serial.print(F("Got: "));
        Serial.println((char *)onoffbutton.lastread);
      #endif
      g_turnflag = true;
    }
  }
  turnon();
  // Now we can publish stuff!
  #ifdef DEBUGGING
    Serial.print(F("\nSending photocell val "));
    Serial.print(data);
    Serial.print("...");
  #endif
  if (! photocell.publish(data)) {
    #ifdef DEBUGGING
      Serial.println(F("Failed"));
    #endif
  } else {
    #ifdef DEBUGGING
      Serial.println(F("OK!"));
    #endif
  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
  //free(data); 
  delay(2000);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  #ifdef DEBUGGING
    Serial.print("Connecting to MQTT... ");
  #endif
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       #ifdef DEBUGGING
         Serial.println(mqtt.connectErrorString(ret));
         Serial.println("Retrying MQTT connection in 5 seconds...");
       #endif
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  #ifdef DEBUGGING
    Serial.println("MQTT Connected!");
  #endif
}

char* reading_dht()
{   
  char result[64];
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));

    sprintf(result, "Sensor Error!!!!");
    return result;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  sprintf(result, "Humidity:%0.2f, temperature:%0.2f", h, t);
  return result;
}

void turnon(){
  if(g_turnflag == true && g_pump_on == false){
    g_begin = millis();
    g_pump_on = true;
    digitalWrite(D1, HIGH);
    Serial.println("Turn on Pump");
    
  }
  else{
    if(millis() - g_begin > COUNTDOWN && g_pump_on == true){
      digitalWrite(D1, LOW);
      g_turnflag = false;
      g_pump_on = false;
      Serial.write("Turn off Pump");
    }
    else if(millis() - g_begin > COUNTDOWN && g_pump_on == false){
      digitalWrite(D1, LOW);
      g_turnflag = false;
      g_pump_on = false;
    }
  }
}
