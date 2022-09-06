#include "OMC_WIFI_read.hpp"


//***************************************************************************************************************************************************************
//*************************************************    VARIABLES, CONSTANTES Y ARREGLOS PARA LECTURA DE PARÁMETROS    *******************************************
//***************************************************************************************************************************************************************

PZEM004Tv30 pzem(Serial2, 16, 17);

float voltage;
float current;
float power;
float energy;
float pf;
//float frequency;

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

  //ESP.restart();

}


void relaySetUp () {

  pinMode(relayPin, OUTPUT);        // El Relé es una salida
  digitalWrite(relayPin, LOW);      // Al reiniciar el dispositivo el relé está apagado
  relay = LOW;                // Variable del estado relay (para ser leída por software)

  pasoElTiempo = 0;           // La variable indica que transcurrieron los segundos necesarios para restaurar corriente

  // Creación del temporizador, se desborda y ejecuta pasoTiempoRecuperacion() luego de que trascurran "tiempoRecuperacion" segundos
  timerRecuperacion = xTimerCreate("TimerDeRecuperacion", pdMS_TO_TICKS(tiempoRecuperacion * 1000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(pasoTiempoRecuperacion));
  xTimerStop(timerRecuperacion, 0); // Mantiene apagado el temporizador hasta que se vuelva a iniciar

  // Creación del temporizador, se desborda y ejecuta pasoTiempoSecundario() luego de que trascurra el tiempo estimado para cada OMC
  timerSecundario   = xTimerCreate("TimerSecundario", pdMS_TO_TICKS(3000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(pasoTiempoSecundario));
  xTimerStop(timerSecundario, 0); // Mantiene apagado el temporizador hasta que se vuelva a iniciar

  //mqttClient.publish("esp32/estadoRelay", 0, true, "OFF");

  Serial.println();
  Serial.println("Dispositivo inicializado");
  Serial.println("*** Relay apagado ***");

}


void pasoTiempoRecuperacion() {   // Se ejecuta luego de que trascurran "tiempoRecuperacion" segundos

  pasoElTiempo = 0;

  if ((numberID != 0) and (numberID != 99)) {

    xTimerChangePeriod(timerSecundario, pdMS_TO_TICKS((numberID - 1) * 5000 + 5), 0);
    Serial.println("Rutina conectado a servidor completada: " + String((numberID - 1) * 5 + 5) + "segs");

  }
  else {

    xTimerChangePeriod(timerSecundario, pdMS_TO_TICKS(random(5, 60000)), 0);
    Serial.println("Rutina random completada.");

  }

  xTimerReset(timerRecuperacion, 0);  // Se detiene el temporizador
  xTimerStop(timerRecuperacion, 0);



}


void pasoTiempoSecundario() {   // Se ejecuta luego de que trascurran los segundos calculados para este OMC

  if ( (rmsVolt >= voltInf) && (rmsVolt <= voltSup) && (rmsCorr <= corrSup) && (relay == LOW) ) {

    pasoElTiempo = 1;               // La variable indica que transcurrieron los segundos

  }

  xTimerReset(timerSecundario, 0);  // Se detiene el temporizador
  xTimerStop(timerSecundario, 0);

  Serial.println("Rutina secundaria completada.");

}


void readCode (void *readParameter) {

  Serial.println("Read Task created");

  while (true) {

    // Lectura de parámetros
    voltage = pzem.voltage();
    current = pzem.current();
    power = pzem.power();
    energy = pzem.energy();
    pf = pzem.pf();
    //    float frequency = pzem.frequency();

    // Igualación de Variables Globales
    if (!isnan(voltage)) {

      rmsVolt = voltage;

    }

    if (!isnan(current)) {

      rmsCorr = current;

    }

    if (!isnan(power)) {

      rmsPower = power;

    }

    if (!isnan(energy)) {

      _energy = energy;

    }

    if (!isnan(pf)) {

      powFactor = pf;

    }




    // Código de control del relay
    if (controlGlobalRelay == true) {                                                              // Si el relay está en modo automático entonces ejecuta el control automático:
      if ( (rmsVolt >= voltInf) && (rmsVolt <= voltSup) && (rmsCorr <= corrSup) && (relay == LOW) ) {      // Una vez esté la alimenteciión en los niveles correctos:
        if ((xTimerIsTimerActive(timerRecuperacion) == pdFALSE) and (xTimerIsTimerActive(timerSecundario) == pdFALSE)) {
          xTimerReset(timerRecuperacion, 0);                                                          // Se inicia el temporizador solo si no estaba activo antes

          Serial.println();
          Serial.println("*** Voltaje/corriente dentro de rango *** ");
          Serial.print("Temporizador inicializado - Esperando ");
          Serial.print(tiempoRecuperacion);
          Serial.println(" segundos para restaurar alimentación");
        }

        if (pasoElTiempo == 1) {                                                                    // Si pasaron los segundos necesarios entonces

          pasoElTiempo = 0;
          relay = HIGH;                                                                               // Enciende el relay
          digitalWrite(relayPin, HIGH);

          xTimerReset(timerRecuperacion, 0);  // Se detiene el temporizador
          xTimerStop(timerRecuperacion, 0);


          //mqttClient.publish("esp32/estadoRelay", 0, true, "ON");

          Serial.println();
          Serial.println("*** Relay encendido ***");
          Serial.println("Temporizador detenido");


          xTimerStop(publishTimer, 0);
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

          //mqttClient.publish("esp32/estadoRelay", 0, true, "OFF");

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

      //mqttClient.publish("esp32/estadoRelay", 0, true, "OFF");

      Serial.println();
      Serial.println("*** Se ha forzado corte de alimentación ***");
      Serial.println("*** Relay apagado ***");
    }

  }

}
