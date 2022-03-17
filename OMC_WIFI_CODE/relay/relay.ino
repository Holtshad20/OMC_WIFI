#include <WiFi.h>
#include <PubSubClient.h> 

// SSID y Contraseña de la red (Hard coded)
const char* ssid = "dos_desktop";
const char* password = "8RlGCKQL";
// Dirección IP del broker MQTT (Hard coded)
const char* mqtt_server = "192.168.7.217";

WiFiClient espClient;
PubSubClient client(espClient)
long lastMsg = 0;
char msg[50];
int value = 0;

volatile int interruptCounter;
int totalInterruptCounter;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);

}

int entero = 0;

void setup() {
  Serial.begin(115200);       //Inicializa comunicación serial
  delay(100);

  pinMode(23, OUTPUT);
  digitalWrite(23, HIGH);
  delay(100);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

void loop() {
  boolean relay = LOW;

  uint8_t volt_sup = 140;
  uint8_t volt_inf = 100;

  char caracteres[6] = "";

  boolean recuperacion = 0;

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
    recuperacion = 0;
    relay = HIGH;
    digitalWrite(23, HIGH);
    totalInterruptCounter = 0;
  }
  else {
    recuperacion = 1; 
    if (totalInterruptCounter >= 10) {
      relay = LOW;
      digitalWrite(23, LOW);
    }
  }

  if (interruptCounter > 0) {
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);

    totalInterruptCounter++;
    if (recuperacion == 1){
      Serial.print("Segundos transcurridos desde recuperación: ");
      Serial.println(totalInterruptCounter);
    }
  }
}
