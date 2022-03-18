#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

boolean relay = LOW;

float voltaje = 0;
float corriente = 7;

uint8_t voltSup = 140;
uint8_t voltInf = 100;
uint8_t corrSup = 15;


boolean controlGlobalRelay = 1;
// Si controlGlobalRelay = 0 entonces estamos forzando a que se mantenga apagado sin importar el voltaje o la corriente.
// Si controlGlobalRelay = 1 entonces estamos trabajando de manera normal con los márgenes de voltaje y corriente normales.

boolean pasoElTiempo = 0;
uint8_t tiempoRecuperacion = 5;

TimerHandle_t timerRecuperacion;

void pasoTiempoRecuperacion() {
  pasoElTiempo = 1;
}

void setup() {
  Serial.begin(115200);       //Inicializa comunicación serial
  delay(100);

  pinMode(23, OUTPUT);
  digitalWrite(23, LOW);

  timerRecuperacion = xTimerCreate("TimerDeRecuperacion", pdMS_TO_TICKS(tiempoRecuperacion * 1000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(pasoTiempoRecuperacion));
  xTimerStop(timerRecuperacion, 0);
  
  delay(100);
}

void loop() {
  char caracteres[6] = "";

  uint8_t i = 0;
  while (Serial.available() > 0) {
    delay(100);

    caracteres[i] = Serial.read();
    i++;

    if (Serial.available() == 0) {
      voltaje = atof(caracteres);
      Serial.print("Recibido: ");
      Serial.println(voltaje);
    }
  }

  if (controlGlobalRelay == 1) {
    if ( ( (voltaje <= voltInf) || (voltaje >= voltSup) || (corriente >= corrSup) ) && relay == HIGH) {
      xTimerStop(timerRecuperacion, 0);
      relay = LOW;
      digitalWrite(23, LOW);

      Serial.println("Relay apagado");
      Serial.println("Temporizador detenido");
    }
    else if (relay == LOW) {
      if (xTimerIsTimerActive(timerRecuperacion) == pdFALSE ){   
        xTimerReset(timerRecuperacion, 0);
        Serial.println();
        Serial.print("Temporizador inicializado - Esperando ");
        Serial.print(tiempoRecuperacion);
        Serial.println(" segundos");
      }
      if (pasoElTiempo == 1) {
        xTimerStop(timerRecuperacion, 0); 
        relay = HIGH;
        digitalWrite(23, HIGH);
        pasoElTiempo = 0;
        Serial.println("Relay encendido");
        Serial.println("Temporizador detenido");
      }
    }
  } 
  else if (relay == HIGH) {
    relay = LOW;
    digitalWrite(23, LOW);
  }
}
