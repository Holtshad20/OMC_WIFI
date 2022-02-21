void setup() {
  Serial.begin(115200);
}

void loop() {
  int x1 = analogRead(39);

  Serial.println(x1);

  delay(10);
}
