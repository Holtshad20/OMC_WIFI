boolean relay = LOW;

uint8_t volt_sup = 140;
uint8_t volt_inf = 100;

boolean recuperacion = 0;
boolean controlGlobalRelay = 1;

uint8_t tiempoRecuperacion = 30;

// Si controlGlobalRelay = 0 entonces estamos forzando a que se mantenga apagado sin importar el voltaje o la corriente.
// Si controlGlobalRelay = 1 entonces estamos trabajando de manera normal con los márgenes de voltaje y corriente normales.

//volatile int interruptCounter;
//int totalInterruptCounter;
//hw_timer_t * timer = NULL;
//portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
//
//void IRAM_ATTR onTimer() {
//  portENTER_CRITICAL_ISR(&timerMux);
//  interruptCounter++;
//  portEXIT_CRITICAL_ISR(&timerMux);
//
//}

void setup() {
  Serial.begin(115200);       //Inicializa comunicación serial
  delay(100);

  pinMode(23, OUTPUT);
  digitalWrite(23, HIGH);
  delay(100);

  //  timer = timerBegin(0, 80, true);
  //  timerAttachInterrupt(timer, &onTimer, true);
  //  timerAlarmWrite(timer, 1000000, true);
  //  timerAlarmEnable(timer);
}

void loop() {
  int entero = 0;
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


  if (controlGlobalRelay == 1) {
    if ((entero <= volt_inf || entero >= volt_sup ) && relay == LOW) {
      recuperacion = 0;
      relay = HIGH;
      digitalWrite(23, HIGH);
      tiempoActual = 0;
    }
    else if (relay == HIGH) {
      recuperacion = 1;
      if (tiempoActual >= tiempoRecuperacion) {
        relay = LOW;
        digitalWrite(23, LOW);
        recuperacion = 0;
      }
    }

    if (recuperacion == 1) {
      Serial.print("Segundos transcurridos desde recuperación: ");
      Serial.println(tiempoActual);
    }
  }
  
  else if (relay == LOW) {
    relay = HIGH;
    digitalWrite(23, HIGH);
  }
}
