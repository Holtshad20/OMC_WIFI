#include "OMC_WIFI_Leds.hpp"


void ledSetUp() {                           // Rutina de configuración incial par los leds

  ledcSetup(greenChannel, 1000, 8);           // Se configura el canal para el led verde (Canal 0, 1 KHz, 8 bits de resolución)
  ledcAttachPin(greenLed, greenChannel);      // Se enlaza el pin 23 con el canal del led verde

  ledcSetup(redChannel, 1000, 8);             // Se configura el canal para el led rojo (Canal 1, 1 KHz, 8 bits de resolución)
  ledcAttachPin(redLed, redChannel);          // Se enlaza el pin 22 con el canal del led rojo
}

void greenLedTask(void *greenLedParameter) {      // Rutina de funcionamiento del led verde

  wifi_mode_t esp_mode;                             // Se guarda el modo (AP o APSTA) del OMC

//  Serial.println("Green Led Task created");

  while (true) {

    esp_wifi_get_mode(&esp_mode);

    if ((esp_mode == WIFI_MODE_APSTA) and WiFi.isConnected()) {   // Si el OMC está en modo APSTA y está conectado a una red
      // Se aplican 2 parpadeos en el led
      ledcWrite(greenChannel, 255);
      vTaskDelay(200 / portTICK_PERIOD_MS);

      ledcWrite(greenChannel, 0);
      vTaskDelay(200 / portTICK_PERIOD_MS);

      ledcWrite(greenChannel, 255);
      vTaskDelay(200 / portTICK_PERIOD_MS);

      ledcWrite(greenChannel, 0);
      vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
    else {

      if (connServer) {                                   // Si el OMC está conectado al servidor
        // Se mantiene el led encendido
        ledcWrite(greenChannel, 255);
        vTaskDelay(200 / portTICK_PERIOD_MS);

      }
      else if (WiFi.isConnected() and !connServer) {      // Si el OMC está conectado a una red pero NO está conectado al servidor
        // El led parpadea cada segundo
        ledcWrite(greenChannel, 255);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        ledcWrite(greenChannel, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);


      }
      else if (!WiFi.isConnected()) {                                             // Si el OMC no está conectado a una red
        // El led hace fading
        for (int dutyCycle = 0; dutyCycle <= 254; dutyCycle = dutyCycle + 2) {
          // changing the LED brightness with PWM
          ledcWrite(greenChannel, dutyCycle);
          vTaskDelay(10 / portTICK_PERIOD_MS);
        }

        // decrease the LED brightness
        for (int dutyCycle = 254; dutyCycle >= 0; dutyCycle = dutyCycle - 2) {
          // changing the LED brightness with PWM
          ledcWrite(greenChannel, dutyCycle);
          vTaskDelay(10 / portTICK_PERIOD_MS);
        }

      }

    }


  }

}


void redLedTask(void *redLedParameter) {                // Rutina de funcionamiento del led rojo

//  Serial.println("Red Led Task created");

  while (true) {

    //    if ((corrFail >= 3) and (!controlGlobalRelay)) {      // Se el límite de corriente ha sido superado más de 3 veces y el relé está bloqueado manualmente
    //                                                            // El led se mantiene encendido
    //      estadoOMC = 6;                                        // Bloqueo manual por exceso de sobrepaso de límite de corriente
    //
    //      ledcWrite(redChannel, 255);
    //      vTaskDelay(200 / portTICK_PERIOD_MS);
    //
    //    }
    if (!controlGlobalRelay) {                      // Si el relé está bloqueado manualmente
                                                      // El led se mantiene encendido  
      if (corrFail >= 3) {                            // Se el límite de corriente ha sido superado más de 3 veces

        estadoOMC = 6;                                  // Bloqueo manual por exceso de sobrepaso de límite de corriente

      }
      else {

        estadoOMC = 2;                                  // Bloqueo manual

      }


      ledcWrite(redChannel, 255);
      vTaskDelay(200 / portTICK_PERIOD_MS);

    }
    else if ((xTimerIsTimerActive(timerRecuperacion) != pdFALSE) or (xTimerIsTimerActive(timerSecundario) != pdFALSE)) {    // Si el OMC está en tiempo de espera para habilitar el suministro
                                                                                                                              // El led hace fading
      estadoOMC = 1;                                                                                                          // Tiempo de espera

      for (int dutyCycle = 0; dutyCycle <= 254; dutyCycle = dutyCycle + 2) {
        // changing the LED brightness with PWM
        ledcWrite(redChannel, dutyCycle);
        vTaskDelay(10 / portTICK_PERIOD_MS);

        if ((rmsVolt <= voltInf) or (rmsVolt >= voltSup) or (rmsCorr >= corrSup)) {                                           // Si excede alguno de los límites 

          break;                                                                                                              //Se rompe el cliclo de fading

        }

      }

      // decrease the LED brightness
      for (int dutyCycle = 254; dutyCycle >= 0; dutyCycle = dutyCycle - 2) {
        // changing the LED brightness with PWM
        ledcWrite(redChannel, dutyCycle);
        vTaskDelay(10 / portTICK_PERIOD_MS);

        if ((rmsVolt <= voltInf) or (rmsVolt >= voltSup) or (rmsCorr >= corrSup)) {

          break;

        }

      }

    }
    else if (rmsCorr > 0.9 * corrSup) {                           // Si se supera el límite de corriente
                                                                    // El led parpadea rápidamente
      estadoOMC = 5;                                                // Alta corriente

      for (int dutyCycle = 0; dutyCycle <= 40; dutyCycle++) {       // Se mantiene el parpadeo durante unos segundos luego de superar el límite de corriente

        //ledcSetup(greenChannel, 1, 8);
        ledcWrite(redChannel, 255);
        vTaskDelay(200 / portTICK_PERIOD_MS);

        ledcWrite(redChannel, 0);
        vTaskDelay(200 / portTICK_PERIOD_MS);

      }

    }
    else if (rmsVolt > voltSup) {               // Si el voltaje supera el límite superior
                                                  // El led parpadea 2 veces cada segundo
      estadoOMC = 4;                              // Alto voltaje

      ledcWrite(redChannel, 255);
      vTaskDelay(200 / portTICK_PERIOD_MS);

      ledcWrite(redChannel, 0);
      vTaskDelay(200 / portTICK_PERIOD_MS);

      ledcWrite(redChannel, 255);
      vTaskDelay(200 / portTICK_PERIOD_MS);

      ledcWrite(redChannel, 0);
      vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
    else if (rmsVolt < voltInf) {           // Si el voltaje supera el límite superior
                                              // El led parpadea cada segundo
      estadoOMC = 3;                          // Bajo voltaje

      ledcWrite(redChannel, 255);
      vTaskDelay(1000 / portTICK_PERIOD_MS);

      ledcWrite(redChannel, 0);
      vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
    else {                                  // Si el suministro se encuentra en rangos aceptables y no está bloqueado
                                              // Se apaga el led
      estadoOMC = 0;                          // Operación normal

      ledcWrite(redChannel, 0);
      vTaskDelay(200 / portTICK_PERIOD_MS);

    }

  }

}
