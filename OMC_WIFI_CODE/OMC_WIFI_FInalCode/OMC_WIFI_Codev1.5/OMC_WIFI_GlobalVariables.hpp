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

#define         MQTT_PORT 1883                   // Puerto al que se debe conectar el OMC para acceder a los servicios del servidor
extern IPAddress       MQTT_HOST;                // Dirección IP del servidor

extern AsyncMqttClient mqttClient;               // Cliente MQTT del OMC

extern int    numberID;                      // ID que le asigna el servidor al OMC
extern int    uptime;                      // Tiempo operativo del OMC

extern TimerHandle_t   mqttReconnectTimer;       // Temporizador para reconectar al servidor
extern TimerHandle_t   publishTimer;             // Temporizador para publicar al servidor

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************


//***************************************************************************************************************************************************************
//*************************************************    VARIABLES, CONSTANTES Y ARREGLOS PARA LECTURA ANALÓGICA     **********************************************
//***************************************************************************************************************************************************************

extern float rmsVolt;      // Valor RMS Voltaje
extern float rmsCorr;      // Valor RMS Corriente
extern float rmsPower;     // Valor RMS Potencia
extern float powFactor;    // Valor Factor de Potencia
extern float _energy;      // Valor Energía

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*************************************************    VARIABLES Y CONSTANTES PARA CONTROLAR EL RELAY     *******************************************************
//***************************************************************************************************************************************************************

#define lowFactor 0.85
#define highFactor 1.15


extern int     voltMode;                   // Modo voltaje: 120 o 220
extern int     corrFail;                   // Contador de fallas por corriente      

extern float   corrSup;                    // Límite superior de corriente

extern uint8_t estadoOMC;                  // Estado de OMC (alto voltaje, bajo voltaje, ...) 
extern uint8_t tiempoRecuperacion;         // Tiempo requerido permitir paso de corriente luego de una falla o un reinicio (segundos)
extern uint8_t voltSup;                    // Máximo voltaje permitido
extern uint8_t voltInf;                    // Mínimo voltaje permitido

extern boolean relay;                      // Estado del relay (software)
extern boolean controlGlobalRelay;         // Control Global del Relé
                                             // Si controlGlobalRelay = 0 entonces estamos forzando a que se mantenga apagado sin importar el voltaje o la corriente.
                                             // Si controlGlobalRelay = 1 entonces estamos trabajando de manera normal con los márgenes de voltaje y corriente normales.

extern TimerHandle_t timerRecuperacion;    // Temporizador, se desborda y ejecuta pasoTiempoRecuperacion() luego de que trascurran "tiempoRecuperacion" segundos
extern TimerHandle_t timerSecundario;      // Temporizador, se desborda y ejecuta pasoTiempoSecundario() luego de que trascurra el tiempo estimado para cada OMC
extern TimerHandle_t timerCorrFail;        // Temporizador, se desborda y ejecuta blockCurrent() y reinicia la cuenta de corrFail

extern TaskHandle_t  xReadHandle;          // Manejador de tareas de la rutina de lectura

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*************************************************    VARIABLES Y CONSTANTES PARA CONTROLAR LEDS     *******************************************************
//***************************************************************************************************************************************************************

extern TaskHandle_t  xGreenLedHandle;    //Manejador de tareas de la rutina del led verde
extern TaskHandle_t  xRedLedHandle;      //Manejador de tareas de la rutina del led rojo

extern TaskHandle_t  xTouchHandle;       //Manejador de tareas de la rutina del Touch Sensor

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*******************************************************    CONSTANTES Y CONSTRUCTORES PARA AUTOCONNECT     ****************************************************
//***************************************************************************************************************************************************************

extern Preferences storage;               // Espacio en memoria para guardar los datos necesarios

extern String      omcID;                 // Variable donde se guardan los últimos 3 bytes de la dirección MAC (ESP.getEfuseMAC extrae los bytes deordenados)

extern boolean     connServer;            // Variable para indicar si el ESP32 está conectado al servidor

extern TaskHandle_t  xAutoConnectHandle;  //Manejador de tareas de la rutina de AutoConnect

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************

void credReset();     // Rutina para reiniciar credenciales

#endif
