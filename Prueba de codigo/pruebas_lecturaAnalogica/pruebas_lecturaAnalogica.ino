void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print(String("VN = ") + analogRead(39));  //SensorVN
  Serial.print(" ");
  Serial.println(String("VP = ") + analogRead(36));  //SensorVP
  
  delay(1/(4*60));
}
