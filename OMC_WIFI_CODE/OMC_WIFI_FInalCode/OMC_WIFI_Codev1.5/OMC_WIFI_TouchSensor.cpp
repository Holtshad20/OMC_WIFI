#include "OMC_WIFI_TouchSensor.hpp"

uint32_t ticks = 0;

PZEM004Tv30 pzem2(Serial2, 16, 17);

void touchInterrupt() {

  if ((eTaskGetState(xTouchHandle) == eSuspended)) {

//    Serial.println("Interrupted");

    vTaskDelay(10 / portTICK_PERIOD_MS);
    touch_pad_intr_disable();

    vTaskDelay(10 / portTICK_PERIOD_MS);
    vTaskResume(xTouchHandle);

  }

}


void touchSetUp() {

  touchAttachInterrupt(TOUCH_SENSOR, touchInterrupt, THRESHOLD);

}

//Tarea p
void touchTask(void *touchParameter) {

//  Serial.println("Touch Task created");

  while (true) {

    ticks = xTaskGetTickCount();

    while ((touchRead(TOUCH_SENSOR) < THRESHOLD) and (xTaskGetTickCount() - ticks <= 10000)) {

    }

    ticks = xTaskGetTickCount() - ticks;

//    Serial.println(ticks);


    if ((ticks >= AP_MODE_THRESHOLD) and (ticks < CRED_RESET_THRESHOLD)) {

      if (WiFi.softAPdisconnect()) {                          // Si el ESP32 está en modo AP/Estación

        esp_wifi_set_mode(WIFI_MODE_STA);                       // Se cambia el modo del ESP32 a Estación

//        Serial.println("Cambiado a modo Estación");

      }
      else {                                                  // Si el ESP32 está en modo Estación

        String ssid = "OMC-WIFI-" + omcID;

        storage.begin("config", true);                          // Se apertura el espacio en memoria flash denominado "storage" para leer (true)

        esp_wifi_set_mode(WIFI_MODE_APSTA);                     //Se cambia el modo del ESP32 a AP/Estación

        WiFi.softAPConfig(IPAddress(172, 16, 16, 1), IPAddress(172, 16, 16, 1), IPAddress(255, 255, 255, 0));
        WiFi.softAP(storage.getString("ssid", ssid.c_str()).c_str(), storage.getString("pass", "12345678").c_str());    //Se inicializa el AP con las credenciales guardadas
//        Serial.println("Cambiado a modo AP/Estación");

        storage.end();

      }

    }
    //    else if ((ticks >= REBOOT_THRESHOLD) and (ticks < CRED_RESET_THRESHOLD)) {
    //
    //      //Serial.println("Rebooting OMC-WIFI in 5 seconds...");
    //
    //      Serial.end();
    //      vTaskDelay(100 / portTICK_PERIOD_MS);
    //
    //      xTimerDelete(publishTimer, 0);
    //      xTimerDelete(mqttReconnectTimer, 0);
    //      xTimerDelete(timerRecuperacion, 0);
    //      xTimerDelete(timerSecundario, 0);
    //      vTaskDelay(100 / portTICK_PERIOD_MS);
    //
    //      vTaskDelete(xAutoConnectHandle);
    //      vTaskDelete(xGreenLedHandle);
    //      vTaskDelete(xRedLedHandle);
    //      vTaskDelete(xTouchHandle);
    //      vTaskDelay(100 / portTICK_PERIOD_MS);
    //
    //      ESP.restart();
    //
    //    }
    else if (ticks >= CRED_RESET_THRESHOLD) {

      //Serial.println("Clearing credentials and rebooting OMC-WIFI in 5 seconds...");
      pzem2.resetEnergy();
      credReset();
//      Serial.println("Credenciales borradas");
      Serial.end();
      Serial2.end();
      vTaskDelay(100 / portTICK_PERIOD_MS);

      xTimerDelete(publishTimer, 0);
      xTimerDelete(mqttReconnectTimer, 0);
      xTimerDelete(timerRecuperacion, 0);
      xTimerDelete(timerSecundario, 0);
      vTaskDelay(100 / portTICK_PERIOD_MS);

      vTaskDelete(xAutoConnectHandle);
      vTaskDelete(xReadHandle);
      vTaskDelete(xGreenLedHandle);
      vTaskDelete(xRedLedHandle);
      vTaskDelay(100 / portTICK_PERIOD_MS);

      esp_task_wdt_init(1, true);
      esp_task_wdt_add(NULL);
      vTaskDelay(100 / portTICK_PERIOD_MS);

      ESP.restart();

    }
    else {

//      Serial.println("Se presionó el botón por menos de 5 segundos");

    }


    touch_pad_intr_enable();          //Se habilita nuevamente la interrupción
    vTaskSuspend(NULL);               //Se suspende la tarea del Touch Sensor

  }

}
