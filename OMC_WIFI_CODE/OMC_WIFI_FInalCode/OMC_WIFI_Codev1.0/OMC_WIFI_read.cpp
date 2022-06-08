#include "OMC_WIFI_read.hpp"


//***************************************************************************************************************************************************************
//*************************************************    VARIABLES, CONSTANTES Y ARREGLOS PARA LECTURA ANALÓGICA     **********************************************
//***************************************************************************************************************************************************************

uint32_t promedioVolt = 0;            // Promedio de las mediciones analógicas, usado para eliminar nivel DC
uint32_t promedioCorr = 0;

float lecturaVolt[2500];              // Arreglo con valores del ADC para promedio (Ventana)
float lecturaCorr[2500];              // Arreglo con valores del ADC para promedio (Ventana)
float cuadradoVolt[2500];             // Arreglo con valores cuadrados que se sumarán (Ventana)
float cuadradoCorr[2500];             // Arreglo con valores cuadrados que se sumarán (Ventana)

int   pos = 0;                        // Posición en la ventana de los valores cuadrados

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*************************************************    VARIABLES Y CONSTANTES PARA CONTROLAR EL RELAY     *******************************************************
//***************************************************************************************************************************************************************

boolean pasoElTiempo = false;   // Indica si transcurrió el tiempo de recuperación

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************





void readSetUp() {

  // Sacando promedio inicial (nivel DC) con 2500 muestras
  for (int i = 0; i <= 2499; i++) {
    promedioVolt += analogRead(39);
    vTaskDelay((1 / (4 * 60)) / portTICK_PERIOD_MS);      //Frecuencia de muestreo de 4 veces 60 Hz
  }

  promedioVolt = promedioVolt / 2500;

  // Sacando promedio inicial (nivel DC) con 25000 muestras
  for (int i = 0; i <= 2499; i++) {
    promedioCorr += analogRead(36);
    vTaskDelay((1 / (4 * 60)) / portTICK_PERIOD_MS);      //Frecuencia de muestreo de 4 veces 60 Hz
  }

  promedioCorr = promedioCorr / 2500;

}


void relaySetUp () {

  pinMode(relayPin, OUTPUT);        // El Relé es una salida
  digitalWrite(relayPin, LOW);      // Al reiniciar el dispositivo el relé está apagado
  relay = LOW;                // Variable del estado relay (para ser leída por software)

  pasoElTiempo = 0;           // La variable indica que transcurrieron los segundos necesarios para restaurar corriente

  // Creación del temporizador, se desborda y ejecuta pasoTiempoRecuperacion() luego de que trascurran "tiempoRecuperacion" segundos
  timerRecuperacion = xTimerCreate("TimerDeRecuperacion", pdMS_TO_TICKS(tiempoRecuperacion * 2500), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(pasoTiempoRecuperacion));
  xTimerStop(timerRecuperacion, 0); // Mantiene apagado el temporizador hasta que se vuelva a iniciar

  mqttClient.publish("esp32/estadoRelay", 0, true, "OFF");

  Serial.println();
  Serial.println("Dispositivo inicializado");
  Serial.println("*** Relay apagado ***");

}


void pasoTiempoRecuperacion() {   // Se ejecuta luego de que trascurran "tiempoRecuperacion" segundos
  
  pasoElTiempo = 1;               // La variable indica que transcurrieron los segundos
  
}


void readCode (void *readParameter) {
  float suma;                                     //Variable para guardar la suma de valores cuadrados

  Serial.println("Analog Read Task created");

  while (true) {

    suma = 0;

    lecturaVolt[pos] = analogRead(39);                      // Se lee ADC
    vTaskDelay((1 / (4 * 60)) / portTICK_PERIOD_MS);        // Frecuencia de muestreo de 4 veces 60 Hz (En realidad se obtiene una fs inferior, pero no tanto)
    lecturaCorr[pos] = analogRead(36);                      // Se lee ADC
    vTaskDelay((1 / (4 * 60)) / portTICK_PERIOD_MS);        // Frecuencia de muestreo de 4 veces 60 Hz (En realidad se obtiene una fs inferior, pero no tanto)

    cuadradoVolt[pos] = (lecturaVolt[pos] - promedioVolt) * (lecturaVolt[pos] - promedioVolt) / (multVolt);   // Se calcula el cuadrado de la lectura reescalada
    cuadradoCorr[pos] = (lecturaCorr[pos] - promedioCorr) * (lecturaCorr[pos] - promedioCorr) / (multCorr);   // Se calcula el cuadrado de la lectura reescalada
    pos++;

    if (pos == 2499) {                    // Si se llega a la última posición se vuelve a la primera
      pos = 0;

      // Sacando promedios nuevamente cada 2500 muestras
      for (int i = 0; i <= 2499; i++) {
        promedioVolt += lecturaVolt[i];
        promedioCorr += lecturaCorr[i];
      }

      promedioVolt = promedioVolt / 2500;
      promedioCorr = promedioCorr / 2500;
    }

    suma = 0;

    for (int i = 0; i <= 2499; i++) {
      suma += cuadradoVolt[i];            //Se suman todos los valores cuadráticos.
    }

    rmsVolt = sqrt(suma / 2500);          //Calcula valor RMS al sacar raiz de promedio de valores cuadráticos

    suma = 0;

    for (int i = 0; i <= 2499; i++) {
      suma += cuadradoCorr[i];            //Se suman todos los valores cuadráticos.
    }

    rmsCorr = sqrt(suma / 2500);        //Calcula valor RMS al sacar raiz de promedio de valores cuadráticos
    //rmsCorr = 0;


    //    vTaskDelay(1000 / portTICK_PERIOD_MS);
    //
    //    Serial.print("V RMS = ");
    //    Serial.println(rmsVolt);
    //    Serial.print("C RMS = ");
    //    Serial.println(rmsCorr);


    // Código de control del relay
    if (controlGlobalRelay == true) {                                                              // Si el relay está en modo automático entonces ejecuta el control automático:
      if ( (rmsVolt >= voltInf) && (rmsVolt <= voltSup) && (rmsCorr <= corrSup) && (relay == LOW) ) {      // Una vez esté la alimenteciión en los niveles correctos:
        if (xTimerIsTimerActive(timerRecuperacion) == pdFALSE ) {
          xTimerReset(timerRecuperacion, 0);                                                          // Se inicia el temporizador solo si no estaba activo antes

          Serial.println();
          Serial.println("*** Voltaje/corriente dentro de rango *** ");
          Serial.print("Temporizador inicializado - Esperando ");
          Serial.print(tiempoRecuperacion);
          Serial.println(" segundos para restaurar alimentación");
        }

        if (pasoElTiempo == 1) {                                                                    // Si pasaron los segundos necesarios entonces
          relay = HIGH;                                                                               // Enciende el relay
          digitalWrite(relayPin, HIGH);

          xTimerReset(timerRecuperacion, 0);  // Se detiene el temporizador
          xTimerStop(timerRecuperacion, 0);
          pasoElTiempo = 0;

          mqttClient.publish("esp32/estadoRelay", 0, true, "ON");

          Serial.println();
          Serial.println("*** Relay encendido ***");
          Serial.println("Temporizador detenido");


          xTimerStop(publishTimer, 0);
          pos = 0;
          vTaskDelay(500 / portTICK_PERIOD_MS);
          xTimerReset(publishTimer, 0);
        }

      }

      if ( (rmsVolt < voltInf) || (rmsVolt > voltSup) || (rmsCorr > corrSup) ) {     // Si la alimentación está mal entonces:
        if (relay == HIGH) {                                                          // Solo si el relay estaba encendido antes
          relay = LOW;                                                                  // Apaga el relay
          digitalWrite(relayPin, LOW);

          xTimerReset(timerRecuperacion, 0);  // Se detiene el temporizador
          xTimerStop(timerRecuperacion, 0);
          pasoElTiempo = 0;

          mqttClient.publish("esp32/estadoRelay", 0, true, "OFF");

          Serial.println();
          Serial.println("*** ¡Voltaje/corriente fuera de rango! ***");
          Serial.println("*** Relay apagado ***");
          Serial.println("Temporizador detenido");
        }
        else {                                                                // Si en cambio el relay estaba apagado entonces significa que se interrumpio la recuperación
          if (xTimerIsTimerActive(timerRecuperacion) != pdFALSE ) {             // Pero solo se muestra el mensaje una sola vez
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
    else if (relay == HIGH) {             // Si el relé se mandó a apagar de manera manual:
      relay = LOW;                          // Se apaga el relé
      digitalWrite(relayPin, LOW);

      xTimerReset(timerRecuperacion, 0);    // Se detiene el temporizador
      xTimerStop(timerRecuperacion, 0);
      pasoElTiempo = 0;

      mqttClient.publish("esp32/estadoRelay", 0, true, "OFF");

      Serial.println();
      Serial.println("*** Se ha forzado corte de alimentación ***");
      Serial.println("*** Relay apagado ***");
    }

  }

}
