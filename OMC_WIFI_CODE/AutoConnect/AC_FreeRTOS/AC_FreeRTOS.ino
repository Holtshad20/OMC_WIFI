#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <AutoConnectCredential.h>
#include <Preferences.h>
#include <AsyncMqttClient.h>
//#include "freertos/FreeRTOS.h"
//#include "freertos/timers.h"




//***************************************************************************************************************************************************************
//******************************************************    VARIABLES Y CONSTANTES PARA CLIENTE MQTT     ********************************************************
//***************************************************************************************************************************************************************
//
//#define WIFI_SSID "dos-desktop"
//#define WIFI_PASSWORD "8RlGCKQL"

//#define WIFI_SSID "Thorondor"
//#define WIFI_PASSWORD "n140862m041260n200689m170697gizmokingdom"
//#define MQTT_HOST IPAddress(192, 168, 0, 50)
//#define MQTT_PORT 1883

#define   MQTT_PORT 1883
IPAddress MQTT_HOST;

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;
TimerHandle_t publishTimer;

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*************************************************    VARIABLES, CONSTANTES Y ARREGLOS PARA LECTURA ANALÓGICA     **********************************************
//***************************************************************************************************************************************************************

uint32_t promedioVolt = 0;            // Promedio de las mediciones analógicas, usado para eliminar nivel DC
uint32_t promedioCorr = 0;

float lecturaVolt[2500];              // Arreglo con valores del ADC para promedio (Ventana)
float lecturaCorr[2500];              // Arreglo con valores del ADC para promedio (Ventana)
float cuadradoVolt[2500];             // Arreglo con valores cuadrados que se sumarán (Ventana)
float cuadradoCorr[2500];             // Arreglo con valores cuadrados que se sumarán (Ventana)
int   pos = 0;                          // Posición en la ventana de los valores cuadrados

const float multVolt = 1.28 * 1.28;   // Factor de escala para medir voltaje 1.23 1.21
const float multCorr = 108 * 108;     // Factor de escala para medir corriente 108

float rmsVolt = 0;                    // Valor RMS Voltaje
float rmsCorr = 0;                    // Valor RMS Corriente

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*************************************************    VARIABLES Y CONSTANTES PARA CONTROLAR EL RELAY     *******************************************************
//***************************************************************************************************************************************************************

uint8_t voltSup = 100;                // Máximo voltaje permitido
uint8_t voltInf = 0;                // Mínimo voltaje permitido
uint8_t corrSup = 15;                 // Máxima corriente permitida

uint8_t tiempoRecuperacion = 10;      // Tiempo requerido permitir paso de corriente luego de una falla o un reinicio (segundos)

boolean relay = LOW;                  // Estado del relay (software)
boolean pasoElTiempo = 0;             // Indica si transcurrió el tiempo de recuperación

boolean controlGlobalRelay = true;    // Control Global del Relé
// Si controlGlobalRelay = 0 entonces estamos forzando a que se mantenga apagado sin importar el voltaje o la corriente.
// Si controlGlobalRelay = 1 entonces estamos trabajando de manera normal con los márgenes de voltaje y corriente normales.

TimerHandle_t timerRecuperacion;      // Temporizador, se desborda y ejecuta pasoTiempoRecuperacion() luego de que trascurran "tiempoRecuperacion" segundos

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*******************************************************    CONSTANTES Y CONSTRUCTORES PARA AUTOCONNECT     ****************************************************
//***************************************************************************************************************************************************************

WebServer         Server;             // Constructor del servidor web del ESP32
AutoConnect       Portal(Server);     // Constructor del portal captivo del ESP32
AutoConnectConfig config;             // Constructor de las configuraciones del AutoConnect

Preferences       storage;            // Espacio en memoria para guardar los datos necesarios

String chipID;                        // Variable donde se guardan los últimos 3 bytes de la dirección MAC (ESP.getEfuseMAC extrae los bytes deordenados)

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*************************************************    ELEMENTOS Y CONSTRUCTORES PARA PÁGINA WEB DE AUTOCONNECT     *********************************************
//***************************************************************************************************************************************************************

// Declaración de elementos AutoConnect para la página web de configuración del AP del ESP32
ACText(caption01, "Desde este portal podrá configurar algunas características del dispositivo OMC-WIFI", "text-align:justify;font-family:serif;color:#000000;");
ACText(header01, "<h2>Cambiar Nombre del Dispositivo</h2>", "text-align:center;color:2f4f4f;");
ACText(txt01, "<p>El <b>nombre</b> debe tener entre 2 y 32 caracteres y solo acepta el guión como caracter especial (NO puede ser el último).</p>", "text-align:justify");
ACInput(ssid, "", "Nuevo Nombre", "^[a-zA-ZñÑ\\d\\-]{1,31}[a-zA-ZñÑ\\d\\s]$", "Introduzca el nuevo nombre");
ACSubmit(save01, "Guardar Nombre", "/ap_ssid");
ACText(header02, "<h2>Cambiar Clave del Dispositivo</h2>", "text-align:center;color:2f4f4f;");
ACText(txt02, "<p>La <b>clave</b> debe tener entre 8 y 16 caracateres</p>", "text-align:justify");
ACInput(pass1, "", "Nueva Clave", "^.{8,16}$", "Introduzca la nueva clave");
ACInput(pass2, "", "Confirme Clave", "^.{8,16}$", "Introduzca la clave de nuevo");
ACSubmit(save02, "Guardar Clave", "/ap_pass");
ACText(header03, "<h2>Restablecer Credenciales</h2>", "text-align:center;color:2f4f4f;");
ACText(txt03, "Introduzca la <b>clave actual del dispositivo</b> para restablecer las credenciales a las de fábrica", "text-align:justify");
ACInput(pass3, "", "Clave Actual", "^.{8,16}$", "Introduzca la clave actual");
ACSubmit(save03, "Restablecer Credenciales", "/cred_reset");
ACText(header04, "<h2>Configurar Dirección IP del Servidor</h2>", "text-align:center;color:2f4f4f;");
ACText(txt04, "A continuación deberá introducir la <b>dirección IP</b> del servidor al que se desea conectar", "text-align:justify");
ACInput(server, "", "IP del Servidor", "\\b(?:(?:2(?:[0-4][0-9]|5[0-5])|[0-1]?[0-9]?[0-9])\\.){3}(?:(?:2([0-4][0-9]|5[0-5])|[0-1]?[0-9]?[0-9]))\\b", "Introduzca la IP del servidor");
ACSubmit(save04, "Guardar Dirección IP", "/server_ip");
ACSubmit(backMenu, "Volver al menú", "/_ac");


// Declaración de elementos AutoConnect para la página web para guardado de SSID
ACText(txt11, "", "text-align:center");
ACText(note11, "<p><b>ADVERTENCIA:</b> para aplicar los cambios el dispositivo debe ser <b>reiniciado</b>, por lo que el dispositivo conectado al OMC-WIFI será desconectado del suministro eléctrico momentáneamente.</p>", "text-align:justify");
ACSubmit(reset, "Reiniciar equipo", "/_ac#rdlg");
ACSubmit(backConfig, "Volver", "/ap_config");


// Declaración de elementos AutoConnect para la página web para guardado de contraseña
ACText(txt21, "", "text-align:center");


// Declaración de elementos AutoConnect para la página web para restablecer las credenciales
ACText(txt31, "", "text-align:center");


// Declaración de elementos AutoConnect para la página web para configurar la dirección IP del servidor
ACText(txt41, "", "text-align:center");


// Declaración de elementos AutoConnect para la página web de corte del suministro eléctrico
ACText(caption02, "Desde este portal podrá <b>cortar</b> o <b>reestablecer</b> el suministro eléctrico a su dispositivo", "text-align:justify;font-family:serif;color:#000000;");
ACText(switchState, "", "text-align:center;");
ACSubmit(cutRestore, "Cortar/Reestablecer suministro", "/switch_relay");
//ACButton(cut, "Cortar suministro", "document.getElementById('switchState').innerHTML = '0'");
//ACButton(restore, "Reestablecer suministro", "document.getElementById('switchState').innerHTML = '1'");
//ACElement(SwitchSupply, "<script type='text/javascript'>function switchSupply(switchState) {if (switchState){controlGlobalRelay  = true;}else{controlGlobalRelay  = false;}}</script>");
ACSubmit(backSupply, "Volver", "/cut_supply");



// Declaración de la página web para la página web de configuración del AP del ESP32
AutoConnectAux ap_config("/ap_config", "Configuración del Dispositivo", true, {

  caption01,
  header01,
  txt01,
  ssid,
  save01,
  header02,
  txt02,
  pass1,
  pass2,
  save02,
  header03,
  txt03,
  pass3,
  save03,
  header04,
  txt04,
  server,
  save04,
  backMenu,

});


// Declaración de la página web para indicar el cambio de SSID
AutoConnectAux ap_ssid("/ap_ssid", "Configuración del Dispositivo", false, {

  header01,
  txt11,
  note11,
  reset,
  backConfig,

});


// Declaración de la página web para la página web para indicar el cambio de clave
AutoConnectAux ap_pass("/ap_pass", "Configuración del Dispositivo", false, {

  header02,
  txt21,
  note11,
  reset,
  backConfig,

});


// Declaración de la página web para restablecer las credenciales a las de fábrica
AutoConnectAux cred_reset("/cred_reset", "Configuración del Dispositivo", false, {

  header03,
  txt31,
  note11,
  reset,
  backConfig,

});


// Declaración de la página web para configurar la dirección IP del servidor
AutoConnectAux server_ip("/server_ip", "Configuración del Dispositivo", false, {

  header04,
  txt41,
  backConfig,

});


AutoConnectAux cut_supply("/cut_supply", "Suministro Eléctrico", true, {

  caption02,
  cutRestore,
  backMenu,

});


AutoConnectAux switch_relay("/switch_relay", "Suministro Eléctrico", false, {

  switchState,
  backSupply,

});


//AutoConnectText& switchRelay = switch_relay["switchState"].as<AutoConnectText>();

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************


//***************************************************************************************************************************************************************
//*************************************************    FUNCIONES DE LAS PÁGINAS WEB DE AUTOCONNECT     **********************************************************
//***************************************************************************************************************************************************************

// Función para reiniciar los datos en la página de configuración del AP
String onConfig(AutoConnectAux& aux, PageArgument& args) {

  //Se limpian las entradas de datos al acceder al directorio
  aux["ssid"].as<AutoConnectInput>().value   = "";
  aux["pass1"].as<AutoConnectInput>().value  = "";
  aux["pass2"].as<AutoConnectInput>().value  = "";
  aux["pass3"].as<AutoConnectInput>().value  = "";
  aux["server"].as<AutoConnectInput>().value = "";

  return String();

}


//Función para validar el cambio de SSID
String onChangeSSID(AutoConnectAux& aux, PageArgument& args) {
  AutoConnectInput& ssid = ap_config["ssid"].as<AutoConnectInput>();                                                                      //Se guarda el elemento AutoConnectInput denominado "ssid" de la página web ap_config, que guarda los datos del nuevo SSID introducidos por el usuario

  if (args.arg("ssid") == "") {                                                                                                           //Si NO se introdujo un SSID
    aux["txt11"].as<AutoConnectText>().value = "NO se detectó ningún cambio en el <b>SSID</b>.\n";                                          //Aparecerá este mensaje en la página web
  }
  else if (ssid.isValid()) {                                                                                                              //Si se introdujo un SSID y se cumplen las condiciones establecidas
    storage.begin("config", false);                                                                                                         //Se apertura el espacio en memoria flash denominado "storage" para leer y escribir (false)
    storage.putString("ssid", args.arg("ssid"));                                                                                            //Se guarda el SSID introducido por el usuario en la memoria flash
    Serial.println("El <b>SSID</b> ha cambiado a:\n" + storage.getString("ssid", ""));
    storage.end();                                                                                                                          //Se cierra el espacio en memoria flash denominado "storage"

    aux["txt11"].as<AutoConnectText>().value = "El <b>SSID</b> ha cambiado a:\n" + args.arg("ssid");                                        //Aparecerá este mensaje en la página web
  }
  else {                                                                                                                                  //Si se introdujo un SSID pero NO se cumplen las condiciones establecidas
    aux["txt11"].as<AutoConnectText>().value = "El <b>SSID</b> introducido NO cumple las condiciones.\nPor favor intente de nuevo.\n";      //Aparecerá este mensaje en la página web
  }

  return String();

}


// Función para validar el cambio de Contraseña
String onChangePass(AutoConnectAux& aux, PageArgument& args) {

  if (args.arg("pass1") == args.arg("pass2")) {                                                                                             //Si las claves introducidas coinciden
    AutoConnectInput& pass = ap_config["pass1"].as<AutoConnectInput>();                                                                       //Se guarda el elemento AutoConnectInput denominado "pass1" de la página web ap_config, que guarda los datos de la nueva clave introducidos por el usuario
    //Verificando que la clave introducida cumple con las condiciones
    if (args.arg("pass1") == "") {                                                                                                            //Si NO se introdujo una clave
      aux["txt21"].as<AutoConnectText>().value = "NO se detectó ningún cambio en la <b>clave</b>.\n";                                           //Aparecerá este mensaje en la página web
    }
    else if (pass.isValid()) {                                                                                                                //Si se introdujo una clave y se cumplen las condiciones establecidas
      storage.begin("config", false);                                                                                                           //Se apertura el espacio en memoria flash denominado "storage" para leer y escribir (false)
      storage.putString("pass", args.arg("pass1"));                                                                                             //Se guarda la clave introducida por el usuario en la memoria flash
      Serial.println("La <b>clave</b> ha cambiado a:\n" + storage.getString("pass", ""));
      storage.end();                                                                                                                            //Se cierra el espacio en memoria flash denominado "storage"

      aux["txt21"].as<AutoConnectText>().value = "La <b>clave</b> ha sido cambiada exitosamente.\n";                                            //Aparecerá este mensaje en la página web
    }
    else {                                                                                                                                    //Si se introdujo una clave pero NO se cumplen las condiciones establecidas
      aux["txt21"].as<AutoConnectText>().value = "La <b>clave</b> introducida NO cumple las condiciones.\nPor favor intente de nuevo.\n";       //Aparecerá este mensaje en la página web
    }
  }
  else {                                                                                                                                    //Si las claves introducidas coinciden
    aux["txt21"].as<AutoConnectText>().value = "Las <b>claves</b> introducidas NO coinciden.\nPor favor intente de nuevo.\n";                 //Aparecerá este mensaje en la página web
  }

  return String();

}


// Función para validar el reinicio de credenciales
String onCredentialReset(AutoConnectAux& aux, PageArgument& args) {

  if (args.arg("pass3") == storage.getString("pass", "12345678")) {                                               //Si la clave introducida por el usuario coincide con la actual del ESP32
    storage.begin("config", false);                                                                                 //Se apertura el espacio en memoria flash denominado "storage" para leer y escribir (false)
    storage.putString("ssid", "OMC-WIFI-" + chipID);                                                   //Se guarda el SSID de fábrica en la memoria flash
    storage.putString("pass", "12345678");                                                                          //Se guarda la clave de fábrica en la memoria flash
    Serial.println("La <b>clave</b> ha cambiado a:\n" + storage.getString("pass", ""));
    Serial.println("La <b>clave</b> ha cambiado a:\n" + storage.getString("pass", ""));
    storage.end();                                                                                                  //Se cierra el espacio en memoria flash denominado "storage"

    aux["txt31"].as<AutoConnectText>().value = "Las <b>credenciales</b> han sido restablecidas exitosamente.\n";    //Aparecerá este mensaje en la página web
  }
  else {                                                                                                          //Si la clave introducida por el usuario coincide con la actual del ESP32
    aux["txt31"].as<AutoConnectText>().value = "La <b>clave</b> introducida es inválida.\nIntente de nuevo.";       //Aparecerá este mensaje en la página web
  }

  return String();

}


// Función para validar la dirección IP del servidor
String onServerIP(AutoConnectAux& aux, PageArgument& args) {
  AutoConnectInput& server = ap_config["server"].as<AutoConnectInput>();                                                            //Se guarda el elemento AutoConnectInput denominado "server" de la página web ap_config, que guarda los datos de la dirección IP introducidos por el usuario

  if (args.arg("server") == "") {                                                                                                   //Si NO se introdujo una dirección IP
    aux["txt41"].as<AutoConnectText>().value = "NO introdujo ninguna <b>dirección IP</b>.\n";                                         //Aparecerá este mensaje en la página web
  }
  else if (server.isValid()) {                                                                                                      //Si se introdujo una dirección IP y se cumplen las condiciones establecidas
    storage.begin("config", false);                                                                                                   //Se apertura el espacio en memoria flash denominado "storage" para leer y escribir (false)
    storage.putString("server_ip", args.arg("server"));                                                                               //Se guarda la dirección IP en la memoria flash
    Serial.println("La <b>dirección IP del servidor</b> ha cambiado a:\n" + storage.getString("server_ip", ""));
    storage.end();                                                                                                                    //Se cierra el espacio en memoria flash denominado "storage"

    mqttClient.setServer(MQTT_HOST.fromString(args.arg("server")), MQTT_PORT);
    
    aux["txt41"].as<AutoConnectText>().value = "La <b>dirección IP del servidor</b> ha cambiado a:\n" + args.arg("server");           //Aparecerá este mensaje en la página web
  }
  else {                                                                                                                            //Si se introdujo una dirección IP pero NO se cumplen las condiciones establecidas
    aux["txt41"].as<AutoConnectText>().value = "La <b>dirección IP</b> introducida es inválida.\nPor favor intente de nuevo.\n";      //Aparecerá este mensaje en la página web
  }

  return String();

}


String onCutSupply(AutoConnectAux& aux, PageArgument& args) {

  //if (args.arg("switchState") == "0"){
  //controlGlobalRelay  = true;
  //  }
  //  else if (args.arg("switchState") == "1"){
  //    controlGlobalRelay  = true;
  //  }

}

//void switchSupply (boolean switchState){
//
//  if (switchState == false){
//    controlGlobalRelay  = false;
//  }
//  else{
//    controlGlobalRelay  = true;
//  }
//
//}


String onSwitchRelay(AutoConnectAux& aux, PageArgument& args) {

  //AutoConnectText& switchRelay = cut_supply["switchState"].as<AutoConnectText>();

  if (aux["switchState"].as<AutoConnectText>().value == "Suministro reestablecido.") {
    controlGlobalRelay  = false;
    aux["switchState"].as<AutoConnectText>().value = "Suministro cortado.";
  }
  else if (aux["switchState"].as<AutoConnectText>().value == "Suministro cortado.") {
    controlGlobalRelay  = true;
    aux["switchState"].as<AutoConnectText>().value = "Suministro reestablecido.";
  }

  //  if (aux["switchState"].as<AutoConnectText>().value == "Suministro reestablecido.") {
  //    aux["switchState"].as<AutoConnectText>().value = "Suministro cortado.";
  //  }
  //  else if (aux["switchState"].as<AutoConnectText>().value == "Suministro cortado.") {
  //    aux["switchState"].as<AutoConnectText>().value = "Suministro reestablecido.";
  //  }

  return String();
}

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//Función para borrar las credenciales (sólo para la etapa de desarrollo)
//void deleteAllCredentials(void) {
//  //Se guardan las credenciales que se encuentran en la flash
//  AutoConnectCredential credential;
//  station_config_t      config;
//  uint8_t qty           = credential.entries();                                  //Cantidad de credenciales guardadas
//
//  Serial.println("Borrando credenciales");
//  if (qty)
//    Serial.printf("Hay %d credenciales guardadas.\n", qty);
//  else {
//    Serial.println("No hay credenciales guardadas.");
//    return;
//  }
//
//  //Ciclo para borrar las credenciales
//   while (qty--) {
//    credential.load((int8_t)0, &config);                                        //Se cargan las credenciales de la flash
//    if (credential.del((const char*)&config.ssid[0]))                           //Se borran las credenciales cargadas anteriormente
//      Serial.printf("%s fue borrada.\n", (const char*)config.ssid);
//    else
//      Serial.printf("%s NO pudo ser borrada.\n", (const char*)config.ssid);
//  }
//}



//***************************************************************************************************************************************************************
//*************************************************    FUNCIONES DE CONFIGURACIÓN INICIALES (SETUP)     *********************************************************
//***************************************************************************************************************************************************************

//Funciones necesarias para configurar el cliente MQTT
//void connectToWifi() {
//  Serial.println();
//  Serial.println("Connecting to Wi-Fi...");
//  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//}

void connectToMqtt() {
  Serial.println();
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  Serial.println();
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
    
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
      //xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println();
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);

  uint16_t packetIdSub = mqttClient.subscribe("esp32/estadoRelay", 0);
  Serial.println();
  Serial.print("Suscrito a esp32/estadoRelay con QoS 0. Packet ID: ");
  Serial.println(packetIdSub);

  packetIdSub = mqttClient.subscribe("esp32/controlRelay", 1);
  Serial.println();
  Serial.print("Suscrito a esp32/controlRelay con QoS 1. Packet ID: ");
  Serial.println(packetIdSub);

  packetIdSub = mqttClient.subscribe("esp32/volt", 0);
  Serial.println();
  Serial.print("Suscrito a esp32/volt con QoS 0. Packet ID: ");
  Serial.println(packetIdSub);

  packetIdSub = mqttClient.subscribe("esp32/corr", 1);
  Serial.println();
  Serial.print("Suscrito a esp32/corr con QoS 0. Packet ID: ");
  Serial.println(packetIdSub);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println();
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println();
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println();
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  //Serial.println();
  //Serial.println("Se ha enviado/recibido un mensaje MQTT");
  //Serial.println("Topic: " + String(topic));
  //Serial.println("Payload: " + String(payload));

  if (String(topic) == "esp32/controlRelay") {
    Serial.println("Cambio de control");
    controlGlobalRelay = atoi(String(payload).c_str());
  }
}

void onMqttPublish(uint16_t packetId) {
  //Serial.println();
  //Serial.println("Publish acknowledged.");
  //Serial.print("  packetId: ");
  //Serial.println(packetId);
}

void publicarValores() {

  mqttClient.publish("esp32/volt", 0, true, String(rmsVolt).c_str());
  //mqttClient.publish("esp32/corr", 0, true, String(rmsCorr).c_str());
  mqttClient.publish("esp32/corr", 0, true, "0");
  Serial.println();
  Serial.print("V RMS = ");
  Serial.println(rmsVolt);
  Serial.print("C RMS = ");
  Serial.println(rmsCorr);
  xTimerReset(publishTimer, 0);
}

void mqttSetUp() {

  storage.begin("config", true);                                          //Se apertura el espacio en memoria flash denominado "storage" para leer y escribir (false)
  MQTT_HOST.fromString(storage.getString("server_ip", "0.0.0.0"));
  storage.end();

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  //wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
  publishTimer = xTimerCreate("publishTimer", pdMS_TO_TICKS(1000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(publicarValores));
  xTimerStop(publishTimer, 0);

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);

  if (MQTT_HOST != IPAddress(0, 0, 0, 0)) {

    mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  }

  //  connectToWifi();
  xTimerReset(publishTimer, 0);
}







void analogReadSetUp(void) {

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


void pasoTiempoRecuperacion() {   // Se ejecuta luego de que trascurran "tiempoRecuperacion" segundos
  pasoElTiempo = 1;               // La variable indica que transcurrieron los segundos
}


void relaySetUp () {

  pinMode(23, OUTPUT);        // El Relé es una salida
  digitalWrite(23, LOW);      // Al reiniciar el dispositivo el relé está apagado
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



// Función para generar la página de inicio
void rootPage() {

  //Declaración de página web alojada en el directorio "/"
  String content =
    "<html>"
    "<head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "<title>OMC-WIFI</title>"
    "</head>"
    "<body>"
    "<h1 align=\"center\" style=\"color:purple;margin:10px;\">Bienvenido al Menú</h1>"
    "<p></p>"
    "<p style=\"padding-top:5px;text-align:center\">"AUTOCONNECT_LINK(COG_24)"</p>"
    "</body>"
    "</html>";

  //Se envía al usuario el contenido de la página web
  Server.send(200, "text/html", content);

}


//Función para configurar inicialmente los parámetros de AutoConnect
void acSetUp(void) {
  byte mac[6];
  AutoConnectText& switchRelay = switch_relay["switchState"].as<AutoConnectText>();

  //  //Se borra la configuración Wi-Fi
  //  deleteAllCredentials();
  //  WiFi.disconnect(true, true);

  //Se toman los 3 últimos bytes de la MAC del ESP32
  WiFi.macAddress(mac);

  for (int i = 3; i < 6; i++) {
    if (mac[i] < 0x10) {
      chipID += '0';
    }
    chipID += String(mac[i], HEX);
  }


  //Se hace la configuración inicial del AP
  storage.begin("config", true);                          //Se apertura el espacio de las credenciales para leer y sin posibilidad de escribir (true)

  //Configuración Hostname y SSID
  if (storage.getString("ssid", "") != "") {              //Si hay un SSID guardado en memoria, se cambia
    config.hostName = storage.getString("ssid", "");        //Se extrae el hostname guardado en el espacio de memoria "storage"
    config.apid     = storage.getString("ssid", "");        //Se extrae el SSID guardado en el espacio de memoria "storage"
  }
  else {                                                  //Si NO hay un SSID guardado en memoria, se coloca el default
    config.hostName = "OMC-WIFI-" + chipID;    //Se coloca el hostname por defecto
    config.apid     = "OMC-WIFI-" + chipID;    //Se coloca el hostname por defecto
  }

  //Configuración Contraseña
  if (storage.getString("pass", "") != "") {              //Si hay una clave guardada en memoria, se cambia
    config.psk = storage.getString("pass", "");             //Se extrae la contraseña guardada en el espacio de memoria "storage"
  }
  else {                                                  //Si NO hay una clave guardada en memoria, se cambia
    config.psk = "12345678";                                //Se coloca la contraseña por defecto
  }

  storage.end();

  if (controlGlobalRelay == true) {
    switchRelay.value = "Suministro reestablecido.";
  }
  else {
    switchRelay.value = "Suministro cortado.";
  }

  config.apip    = IPAddress(172, 22, 174, 254);                        //Se configura la dirección IPv4 del AP ESP32
  config.title   = "OMC-WIFI-" + chipID;                                //Título de la página web
  config.homeUri = "/_ac";                                           //Directorio HOME de la página web
  Portal.config(config);                                                //Se añaden las configuraciones al portal web
  Portal.join({ap_config, ap_ssid, ap_pass, cred_reset, server_ip, cut_supply, switch_relay});    //Se cargan las páginas web diseñadas en el portal web
  Portal.on("/ap_config", onConfig);                                    //Se enlaza la función "onConfig" con la página en el directorio "/ap_config" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/ap_ssid", onChangeSSID);                                  //Se enlaza la función "onChangeSSID" con la página en el directorio "/ap_ssid" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/ap_pass", onChangePass);                                  //Se enlaza la función "onChangePass" con la página en el directorio "/ap_pass" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/cred_reset", onCredentialReset);                          //Se enlaza la función "onCredentialReset" con la página en el directorio "/cred_reset" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/server_ip", onServerIP);                                  //Se enlaza la función "onServerIP" con la página en el directorio "/server_ip" (la función se ejecutará cada vez que se acceda al directorio)
  //Portal.on("/cut_supply", onCutSupply);                                //Se enlaza la función "onCutSupply" con la página en el directorio "/cut_supply" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/switch_relay", onSwitchRelay);
  //  Portal.begin();                                                       //Se inicializa el portal una vez ha sido configurado
  //
  //  Server.on("/", rootPage);                                             //Se inicializa el servidor web
  //  if (Portal.begin()) {                                                 //Si se configura una conexión del ESP32 a un punto de acceso
  //    Serial.println("WiFi connected: " + WiFi.localIP().toString());       //Se imprime la dirección IP del ESP32 en esa red en la pantalla serial
  //  }

}


//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//********************************************************    TAREAS A MANEJAR CON FreeRTOS     *****************************************************************
//***************************************************************************************************************************************************************

//Código para el portal captivo de AutoConnect
void acCode (void *acParameter) {
  Serial.println("AutoConnect Task created");

  Portal.begin();                                                       //Se inicializa el portal una vez ha sido configurado
  Server.on("/", rootPage);                                             //Se inicializa el servidor web


  while (true) {
    Portal.handleClient();
  }

}


void analogReadCode (void *analogReadParameter) {
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

    //rmsCorr = sqrt(suma / 2500);        //Calcula valor RMS al sacar raiz de promedio de valores cuadráticos
    rmsCorr = 0;


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
          digitalWrite(23, HIGH);

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
          digitalWrite(23, LOW);

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
      digitalWrite(23, LOW);

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


//void printCode (void *analogReadParameter) {
//
//  AutoConnectText& switchRelay = switch_relay["switchState"].as<AutoConnectText>();
//
//  Serial.println("Print Task created");
//
//  while (true) {
//    vTaskDelay(3000 / portTICK_PERIOD_MS);
//
//    Serial.println(switchRelay.value);
//
//    //    if (switchRelay.value == "Suministro cortado.") {
//    //      controlGlobalRelay  = false;
//    //    }
//    //    else if (switchRelay.value == "Suministro reestablecido.") {
//    //      controlGlobalRelay  = true;
//    //    }
//    //
//    //    if (controlGlobalRelay  == true){
//    //      switchRelay.value = "Suministro reestablecido.";
//    //    }
//    //    else{
//    //      switchRelay.value = "Suministro cortado.";
//    //    }
//
//
//    Serial.println(controlGlobalRelay);
//
//
//  }
//
//}


//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************


//Función de configuración inicial del microcontrolador
void setup() {

  vTaskDelay(500 / portTICK_PERIOD_MS);

  Serial.begin(115200);
  Serial.println("Inicializando OMC-WIFI-" + chipID);

  mqttSetUp();
  analogReadSetUp();
  relaySetUp();
  acSetUp();


  //Tarea para ejecutar el código de AutoConnect
  xTaskCreatePinnedToCore(
    acCode,                 //Función que se ejecutará en la tarea
    "AutoConnectCode",      //Nombre descriptivo
    15000,                  //Tamaño del Stack para esta tarea
    NULL,                   //Parámetro para guardar la función
    1,                      //Prioridad de la tarea (de 0 a 25)
    NULL,                   //Manejador de tareas
    0);                     //Núcleo en el que se ejecutará


  //Tarea para ejecutar el código de lectura analógica de voltaje y corriente y control del relay
  xTaskCreatePinnedToCore(
    analogReadCode,         //Función que se ejecutará en la tarea
    "AnalogReadCode",       //Nombre descriptivo
    10000,                 //Tamaño del Stack para esta tarea
    NULL,                   //Parámetro para guardar la función
    1,                      //Prioridad de la tarea (de 0 a 25)
    NULL,                   //Manejador de tareas
    1);                     //Núcleo en el que se ejecutará


  //  //Tarea para ejecutar el código de lectura analógica de voltaje y correinte
  //  xTaskCreatePinnedToCore(
  //    printCode,              //Función que se ejecutará en la tarea
  //    "PrintCode",            //Nombre descriptivo
  //    1024,                   //Tamaño del Stack para esta tarea
  //    NULL,                   //Parámetro para guardar la función
  //    2,                      //Prioridad de la tarea (de 0 a 25)
  //    NULL,                   //Manejador de tareas
  //    1);                     //Núcleo en el que se ejecutará

  vTaskDelay(500 / portTICK_PERIOD_MS);

  //vTaskStartScheduler();    //Se inicializa el organizador de tareas

}


void loop() {

}
