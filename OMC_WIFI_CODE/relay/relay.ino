#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

// Variables compartidas con todo el sistema
float voltaje = 0;      // Voltaje RMS actual
float corriente = 7;    // Corriente RMS actual

uint8_t voltSup = 140;  // Máximo voltaje permitido
uint8_t voltInf = 100;  // Mínimo voltaje permitido
uint8_t corrSup = 15;   // Máxima corriente permitida

uint8_t tiempoRecuperacion = 5; // Tiempo requerido permitir paso de corriente luego de una falla o un reinicio (segundos)

boolean relay = LOW;      // Estado del relay (software)
boolean pasoElTiempo = 0; // Indica si transcurrió el tiempo de recuperación 

boolean controlGlobalRelay = 1; // Control Global del Relé
                                // Si controlGlobalRelay = 0 entonces estamos forzando a que se mantenga apagado sin importar el voltaje o la corriente.
                                // Si controlGlobalRelay = 1 entonces estamos trabajando de manera normal con los márgenes de voltaje y corriente normales.

TimerHandle_t timerRecuperacion;  // Temporizador, se desborda y ejecuta pasoTiempoRecuperacion() luego de que trascurran "tiempoRecuperacion" segundos

void pasoTiempoRecuperacion() {   // Se ejecuta luego de que trascurran "tiempoRecuperacion" segundos
  pasoElTiempo = 1;               // La variable indica que transcurrieron los segundos
}

// El siguiente procedimiento sirve para leer por serial un valor de voltaje forzado (Omitir para implementación final)
void leerSerial() {
  char caracteres[6] = "";

  uint8_t i = 0;
  while (Serial.available() > 0) {
    delay(100);

    caracteres[i] = Serial.read();
    i++;

    if (Serial.available() == 0) {
      voltaje = atof(caracteres);

      Serial.println();
      Serial.print("Recibido: ");
      Serial.println(voltaje);
    }
  }
}

void setup() {
  Serial.begin(115200);       //Inicializa comunicación serial
  delay(100);

  pinMode(23, OUTPUT);        // El Relé es una salida
  digitalWrite(23, LOW);      // Al reiniciar el dispositivo el relé está apagado
  relay = LOW;                // Variable del estado relay (para ser leída por software)

  pasoElTiempo = 0;           // La variable indica que transcurrieron los segundos necesarios para restaurar corriente
  
  // Creación del temporizador, se desborda y ejecuta pasoTiempoRecuperacion() luego de que trascurran "tiempoRecuperacion" segundos
  timerRecuperacion = xTimerCreate("TimerDeRecuperacion", pdMS_TO_TICKS(tiempoRecuperacion * 1000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(pasoTiempoRecuperacion));
  xTimerStop(timerRecuperacion, 0); // Mantiene apagado el temporizador hasta que se vuelva a iniciar
  
  Serial.println();
  Serial.println("Dispositivo inicializado");
  Serial.println("*** Relay apagado ***");

  delay(100);
}

void loop() {

  leerSerial(); // Leer valores forzados de voltaje por serial (omitir para implementación final)

  if (controlGlobalRelay == 1) {  // Si el relay está en modo automático entonces ejecuta el control automático:
    
    if ( (voltaje >= voltInf) && (voltaje <= voltSup) && (corriente <= corrSup) && (relay == LOW) ) { // Una vez esté la alimenteciión en los niveles correctos:
      if (xTimerIsTimerActive(timerRecuperacion) == pdFALSE ){   
        xTimerReset(timerRecuperacion, 0);  // Se inicia el temporizador solo si no estaba activo antes

        Serial.println();
        Serial.println("*** Voltaje/corriente dentro de rango *** ");
        Serial.print("Temporizador inicializado - Esperando ");
        Serial.print(tiempoRecuperacion);
        Serial.println(" segundos para restaurar alimentación");
      }
      if (pasoElTiempo == 1) {  // Si pasaron los segundos necesarios entonces
        relay = HIGH;           // Enciende el relay
        digitalWrite(23, HIGH);

        xTimerReset(timerRecuperacion, 0);  // Se detiene el temporizador
        xTimerStop(timerRecuperacion, 0);
        pasoElTiempo = 0;

        Serial.println();
        Serial.println("*** Relay encendido ***");
        Serial.println("Temporizador detenido");
      }
    }
    if ( (voltaje < voltInf) || (voltaje > voltSup) || (corriente > corrSup) ) {  // Si la alimentación está mal entonces:
      if (relay == HIGH) {  // Solo si el relay estaba encendido antes
        relay = LOW;        // Apaga el relay
        digitalWrite(23, LOW);

        xTimerReset(timerRecuperacion, 0);  // Se detiene el temporizador
        xTimerStop(timerRecuperacion, 0);
        pasoElTiempo = 0;

        Serial.println();
        Serial.println("*** ¡Voltaje/corriente fuera de rango! ***");
        Serial.println("*** Relay apagado ***");
        Serial.println("Temporizador detenido");
      }
      else {  // Si en cambio el relay estaba apagado entonces significa que se interrumpio la recuperación
        if (xTimerIsTimerActive(timerRecuperacion) != pdFALSE ){  // Pero solo se muestra el mensaje una sola vez
          xTimerStop(timerRecuperacion, 0);
          pasoElTiempo = 0;
          Serial.println();
          Serial.println("*** ¡Voltaje/corriente fuera de rango! ***");
          Serial.println("Recuperación fallida");
          Serial.println("Temporizador detenido");
        }
      }
    }
  } 

  else if (relay == HIGH) { // Si el relé se mandó a apagar de manera manual:
    relay = LOW;            // Se apaga el relé
    digitalWrite(23, LOW);
    
    xTimerReset(timerRecuperacion, 0); // Se detiene el temporizador
    xTimerStop(timerRecuperacion, 0);
    pasoElTiempo = 0;

    Serial.println();
    Serial.println("*** Se ha forzado corte de alimentación ***");
    Serial.println("*** Relay apagado ***");
  }
}
