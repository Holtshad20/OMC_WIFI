int entero = 0;

void setup() {
  Serial.begin(115200);       //Inicializa comunicación serial
  delay(100);

  pinMode(23, OUTPUT);
  digitalWrite(23, LOW);
  delay(100);
}

void loop() {
  boolean relay = LOW;

  uint8_t volt_sup = 140;
  uint8_t volt_inf = 100;

  char caracteres[6] = "";


  uint8_t i = 0;
  while (Serial.available() > 0) {
    delay(100);

    caracteres[i] = Serial.read();
    i++;

    if (Serial.available() == 0) {
      entero = atoi(caracteres);
      Serial.print("Recibido: ");
      Serial.println(entero);
    }
  }

  if (entero <= volt_inf || entero >= volt_sup) {
    relay = LOW;
    digitalWrite(23, LOW);
  }
  else {
    relay = HIGH;
    digitalWrite(23, HIGH);
  }
}
