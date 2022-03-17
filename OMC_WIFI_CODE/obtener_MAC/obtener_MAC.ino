#include <WiFi.h>

String mac_str;
 
void setup() {
  Serial.begin(115200);
  byte mac[6];
  WiFi.macAddress(mac);
  
  for(int i = 3; i < 6; i++) {
    if(mac[i] < 0x10) {
      mac_str += '0';
    }
    mac_str += String(mac[i], HEX);
  }
}

void loop() {
  Serial.println(mac_str);
  delay(10000);
}
