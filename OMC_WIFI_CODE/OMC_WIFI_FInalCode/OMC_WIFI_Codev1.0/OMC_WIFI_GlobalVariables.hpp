#ifndef OMC_WIFI_GLOBALVARIABLES_HPP
#define OMC_WIFI_GLOBALVARIABLES_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <nvs_flash.h>
#include <AsyncMqttClient.h>
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"


//***************************************************************************************************************************************************************
//******************************************************    VARIABLES Y CONSTANTES PARA CLIENTE MQTT     ********************************************************
//***************************************************************************************************************************************************************

//#define WIFI_SSID "dos-desktop"
//#define WIFI_PASSWORD "8RlGCKQL"

//#define WIFI_SSID "Thorondor"
//#define WIFI_PASSWORD "n140862m041260n200689m170697gizmokingdom"
//#define MQTT_HOST IPAddress(192, 168, 0, 50)
//#define MQTT_PORT 1883

#define                MQTT_PORT 1883
extern IPAddress       MQTT_HOST;

extern AsyncMqttClient mqttClient;

extern TimerHandle_t   mqttReconnectTimer;
extern TimerHandle_t   wifiReconnectTimer;
extern TimerHandle_t   publishTimer;

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************


//***************************************************************************************************************************************************************
//*************************************************    VARIABLES, CONSTANTES Y ARREGLOS PARA LECTURA ANALÓGICA     **********************************************
//***************************************************************************************************************************************************************

extern float rmsVolt;      // Valor RMS Voltaje
extern float rmsCorr;      // Valor RMS Corriente

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*************************************************    VARIABLES Y CONSTANTES PARA CONTROLAR EL RELAY     *******************************************************
//***************************************************************************************************************************************************************

extern int voltMode;

extern uint8_t voltSup;                   // Máximo voltaje permitido
extern uint8_t voltInf;                   // Mínimo voltaje permitido
extern uint8_t corrSup;                   // Máxima corriente permitida

extern uint8_t tiempoRecuperacion;        // Tiempo requerido permitir paso de corriente luego de una falla o un reinicio (segundos)

extern boolean relay;                     // Estado del relay (software)
extern boolean controlGlobalRelay;        // Control Global del Relé
                                            // Si controlGlobalRelay = 0 entonces estamos forzando a que se mantenga apagado sin importar el voltaje o la corriente.
                                            // Si controlGlobalRelay = 1 entonces estamos trabajando de manera normal con los márgenes de voltaje y corriente normales.

extern TimerHandle_t timerRecuperacion;   // Temporizador, se desborda y ejecuta pasoTiempoRecuperacion() luego de que trascurran "tiempoRecuperacion" segundos


//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*******************************************************    CONSTANTES Y CONSTRUCTORES PARA AUTOCONNECT     ****************************************************
//***************************************************************************************************************************************************************

extern Preferences storage;         // Espacio en memoria para guardar los datos necesarios

extern String      hostname;        // Variable donde se guardan los últimos 3 bytes de la dirección MAC (ESP.getEfuseMAC extrae los bytes deordenados)

extern boolean     connServer;      // Variable para indicar si el ESP32 está conectado al servidor

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************

void credReset();

#endif
