#include "OMC_WIFI_GlobalVariables.hpp"


//***************************************************************************************************************************************************************
//******************************************************    VARIABLES Y CONSTANTES PARA CLIENTE MQTT     ********************************************************
//***************************************************************************************************************************************************************

//#define WIFI_SSID "dos-desktop"
//#define WIFI_PASSWORD "8RlGCKQL"

//#define WIFI_SSID "Thorondor"
//#define WIFI_PASSWORD "n140862m041260n200689m170697gizmokingdom"
//#define MQTT_HOST IPAddress(192, 168, 0, 50)
//#define MQTT_PORT 1883

#define         MQTT_PORT 1883            // Puerto al que se debe conectar el OMC para acceder a los servicios del servidor
IPAddress       MQTT_HOST;                // Dirección IP del servidor

AsyncMqttClient mqttClient;               // Cliente MQTT del OMC

int    numberID = 0;                      // ID que le asigna el servidor al OMC
int    uptime   = 0;                      // Tiempo operativo del OMC

TimerHandle_t   mqttReconnectTimer;       // Temporizador para reconectar al servidor
TimerHandle_t   publishTimer;             // Temporizador para publicar al servidor

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************


//***************************************************************************************************************************************************************
//*************************************************    VARIABLES, CONSTANTES Y ARREGLOS PARA LECTURA     ********************************************************
//***************************************************************************************************************************************************************

float rmsVolt    = 0;      // Valor RMS Voltaje
float rmsCorr    = 0;      // Valor RMS Corriente
float rmsPower   = 0;      // Valor RMS Potencia
float powFactor  = 0;      // Valor Factor de Potencia
float _energy    = 0;      // Valor Energía

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*************************************************    VARIABLES Y CONSTANTES PARA CONTROLAR EL RELAY     *******************************************************
//***************************************************************************************************************************************************************


int     voltMode;                   // Modo voltaje: 120 o 220
int     corrFail = 0;               // Contador de fallas por corriente      

float   corrSup;                    // Límite superior de corriente

uint8_t estadoOMC;                  // Estado de OMC (alto voltaje, bajo voltaje, ...) 
uint8_t tiempoRecuperacion = 20;    // Tiempo requerido permitir paso de corriente luego de una falla o un reinicio (segundos)
uint8_t voltSup;                    // Máximo voltaje permitido
uint8_t voltInf;                    // Mínimo voltaje permitido



boolean relay              = LOW;   // Estado del relay (software)
boolean controlGlobalRelay;         // Control Global del Relé
                                      // Si controlGlobalRelay = 0 entonces estamos forzando a que se mantenga apagado sin importar el voltaje o la corriente.
                                      // Si controlGlobalRelay = 1 entonces estamos trabajando de manera normal con los márgenes de voltaje y corriente normales.

TimerHandle_t timerRecuperacion;    // Temporizador, se desborda y ejecuta pasoTiempoRecuperacion() luego de que trascurran "tiempoRecuperacion" segundos
TimerHandle_t timerSecundario;      // Temporizador, se desborda y ejecuta pasoTiempoSecundario() luego de que trascurra el tiempo estimado para cada OMC
TimerHandle_t timerCorrFail;        // Temporizador, se desborda y ejecuta blockCurrent() y reinicia la cuenta de corrFail

TaskHandle_t  xReadHandle;          // Manejador de tareas de la rutina de lectura


//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//****************************************    VARIABLES Y CONSTANTES PARA CONTROLAR LEDS Y TOUCH SENSOR     *******************************************************
//***************************************************************************************************************************************************************

TaskHandle_t  xGreenLedHandle;    //Manejador de tareas de la rutina del led verde
TaskHandle_t  xRedLedHandle;      //Manejador de tareas de la rutina del led rojo

TaskHandle_t  xTouchHandle;       //Manejador de tareas de la rutina del Touch Sensor

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*******************************************************    CONSTANTES Y CONSTRUCTORES PARA AUTOCONNECT     ****************************************************
//***************************************************************************************************************************************************************

Preferences storage;                 // Espacio en memoria para guardar los datos necesarios

String      omcID;                   // Variable donde se guardan los últimos 3 bytes de la dirección MAC (ESP.getEfuseMAC extrae los bytes deordenados)

boolean     connServer = false;      // Variable para indicar si el ESP32 está conectado al servidor

TaskHandle_t  xAutoConnectHandle;    //Manejador de tareas de la rutina de AutoConnect

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************


void credReset() {      // Rutina para reiniciar credenciales

  nvs_flash_deinit();     // Se desinicializa la partición NVS (necesario para poder borrarla)
  nvs_flash_erase();      // Se borra la partición NVS
  nvs_flash_init();       // Se inicializa la partición NVS

}
