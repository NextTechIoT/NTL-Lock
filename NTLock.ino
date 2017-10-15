#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <String.h>

#define WLAN_SSID       "[WIFI_SSID]"
#define WLAN_PASS       "[WIFI_PASS]"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  8883                
#define AIO_USERNAME    "[USERNAME]"
#define AIO_KEY         "[AIO_KEY]"

String code = "1";

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/[FEEDNAME]");

int lpin = [PinNo];

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  pinMode(lpin, OUTPUT);
  delay(10);
  Serial.println(F("Adafruit MQTT demo"));
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  mqtt.subscribe(&onoffbutton);
}
uint32_t x=0;
void unlock(){
  digitalWrite(lpin, HIGH); 
  delay(3000);
  digitalWrite(lpin, LOW); 
}
void loop() {
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      String val = String((char *)onoffbutton.lastread);
      Serial.println(val);      
      if (val == code){
        Serial.println("Opening Doors");
        unlock();
      } 
    }
  }
  
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000); 
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
