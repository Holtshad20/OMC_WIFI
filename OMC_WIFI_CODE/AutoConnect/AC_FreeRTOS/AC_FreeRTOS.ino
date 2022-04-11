#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <AutoConnectCredential.h>
#include <Preferences.h>
#include <nvs_flash.h>
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

const float multVolt = 2.75 * 2.75;   // Factor de escala para medir voltaje 2.75
const float multCorr = 108 * 108;     // Factor de escala para medir corriente 108

float rmsVolt = 0;                    // Valor RMS Voltaje
float rmsCorr = 0;                    // Valor RMS Corriente

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*************************************************    VARIABLES Y CONSTANTES PARA CONTROLAR EL RELAY     *******************************************************
//***************************************************************************************************************************************************************

uint8_t voltSup = 130;                // Máximo voltaje permitido
uint8_t voltInf = 100;                // Mínimo voltaje permitido
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

boolean inside = false;
//
//const char* scUpdateData = R"(
//<script language="javascript">
//setInterval(function(){
//  document.getElementById('txtCenter01').innerHTML = rmsVolt.toString() + "Volts";
//  document.getElementById('txtCenter02').innerHTML = rmsCorr.toString() + "Amps";
//  },1000);
//</script>
//)";


//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*************************************************    ELEMENTOS Y CONSTRUCTORES PARA PÁGINA WEB DE AUTOCONNECT     *********************************************
//***************************************************************************************************************************************************************

//Declaración de elementos para títulos
ACText(header01, "", "text-align:center;color:2f4f4f;");
ACText(header02, "", "text-align:center;color:2f4f4f;");
ACText(header03, "", "text-align:center;color:2f4f4f;");
ACText(header04, "", "text-align:center;color:2f4f4f;");

//Declaración de elementos para texto
ACText(txt01, "", "text-align:justify");
ACText(txt02, "", "text-align:justify");
ACText(txt03, "", "text-align:justify");
ACText(txt04, "", "text-align:justify");
ACText(txt05, "", "text-align:justify");
ACText(txtCenter01, "", "text-align:center");
ACText(txtCenter02, "", "text-align:center");
ACText(txtCenter03, "", "text-align:center");


// Declaración de elementos AutoConnect para la página web de verificación de contraseña antes de configurar el dispositivo
ACInput(passVer, "", "Clave Actual", "^.{8,16}$", "Introduzca la clave actual", AC_Tag_None, AC_Input_Password);
ACSubmit(ver, "Configurar Dispositivo", "/ap-config");
ACSubmit(backMenu, "Volver", "/_ac");


// Declaración de elementos AutoConnect para la página web de configuración del AP del ESP32
ACInput(ssid, "", "Nuevo Nombre", "^[a-zA-ZñÑ\\d\\-]{1,31}[a-zA-ZñÑ\\d\\s]$", "Introduzca el nuevo nombre");
ACSubmit(save01, "Guardar Nombre", "/ap-ssid");
ACInput(pass1, "", "Nueva Clave", "^.{8,16}$", "Introduzca la nueva clave", AC_Tag_None, AC_Input_Password);
ACInput(pass2, "", "Confirme Clave", "^.{8,16}$", "Introduzca la clave de nuevo", AC_Tag_None, AC_Input_Password);
ACSubmit(save02, "Guardar Clave", "/ap-pass");
ACInput(server, "", "IP del Servidor", "\\b(?:(?:2(?:[0-4][0-9]|5[0-5])|[0-1]?[0-9]?[0-9])\\.){3}(?:(?:2([0-4][0-9]|5[0-5])|[0-1]?[0-9]?[0-9]))\\b", "Introduzca la IP del servidor");
ACSubmit(save03, "Guardar Dirección IP", "/server-ip");
ACRadio(resetCred, {"Sí", "No"}, "", AC_Horizontal, 2);
ACSubmit(save04, "Restablecer Credenciales", "/cred-reset");




// Declaración de elementos AutoConnect para la página web para guardado de configuraciones
ACSubmit(reset, "Reiniciar equipo", "/_ac#rdlg");
ACSubmit(backConfig, "Volver", "/ap-config");


// Declaración de elementos AutoConnect para la página web de corte del suministro eléctrico
ACText(switchState, "", "text-align:center;");
ACSubmit(cutRestore, "Cortar/Reestablecer suministro", "/switch-relay");
//ACButton(cut, "Cortar suministro", "document.getElementById('switchState').innerHTML = '0'");
//ACButton(restore, "Reestablecer suministro", "document.getElementById('switchState').innerHTML = '1'");
//ACElement(SwitchSupply, "<script type='text/javascript'>function switchSupply(switchState) {if (switchState){controlGlobalRelay  = true;}else{controlGlobalRelay  = false;}}</script>");
ACSubmit(backSupply, "Volver", "/supply");
ACButton(updateData, "Actualizar Datos", "window.location.reload()");
//ACElement(reloadPage, scUpdateData);
//ACElement(reloadPage,"<script type='text/javascript'>setInterval(function(){window.location.reload();},3000);</script>");


// Declaración de la página web para la página web de verificación de contraseña antes de configurar el dispositivo
AutoConnectAux pre_config("/pre-config", "Configuración del Dispositivo", true, {

  txt01,
  passVer,
  ver,
  backMenu,

});


// Declaración de la página web para la página web de configuración del AP del ESP32
AutoConnectAux ap_config("/ap-config", "Configuración del Dispositivo", false, {

  txt01,
  header01,
  txt02,
  ssid,
  save01,
  header02,
  txt03,
  pass1,
  pass2,
  save02,
  header03,
  txt04,
  server,
  save03,
  header04,
  txt05,
  resetCred,
  save04,
  backMenu,

});


// Declaración de la página web para indicar el cambio de SSID
AutoConnectAux ap_ssid("/ap-ssid", "Configuración del Dispositivo", false, {

  header01,
  txtCenter01,
  txt01,
  reset,
  backConfig,

});


// Declaración de la página web para la página web para indicar el cambio de clave
AutoConnectAux ap_pass("/ap-pass", "Configuración del Dispositivo", false, {

  header02,
  txtCenter01,
  txt01,
  reset,
  backConfig,

});


// Declaración de la página web para configurar la dirección IP del servidor
AutoConnectAux server_ip("/server-ip", "Configuración del Dispositivo", false, {

  header03,
  txtCenter01,
  backConfig,

});


// Declaración de la página web para restablecer las credenciales a las de fábrica
AutoConnectAux cred_reset("/cred-reset", "Configuración del Dispositivo", false, {

  header04,
  txtCenter01,
  txt01,
  reset,
  backConfig,

});


AutoConnectAux supply("/supply", "Suministro Eléctrico", true, {

  header01,
  txtCenter01,
  header02,
  txtCenter02,
  header03,
  txtCenter03,
  updateData,
  header04,
  txt04,
  passVer,
  cutRestore,
  backMenu,
  //reloadPage,

});


AutoConnectAux switch_relay("/switch-relay", "Suministro Eléctrico", false, {

  header04,
  txtCenter01,
  txtCenter02,
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
String onPreConfig(AutoConnectAux& aux, PageArgument& args) {

  inside = false;

  //Se habilita y limpia el campo para introducir la clave
  //aux["passVer"].as<AutoConnectInput>().enable = true;
  aux["passVer"].as<AutoConnectInput>().value  = "";

  //Textos que aparecerán es esta página
  aux["txt01"].as<AutoConnectText>().value = "Para configurar el dispositivo OMC-WIFI, por favor introduzca la clave actual.";


  return String();

}


// Función para indicar los datos iniciales en la página de configuración del AP
String onConfig(AutoConnectAux& aux, PageArgument& args) {

  storage.begin("config", true);       // Se apertura el espacio en memoria flash denominado "storage" para leer y escribir (false)

  if ((args.arg("passVer") == storage.getString("pass", "12345678")) or (inside == true)) {

    inside = true;

    //Textos que aparecerán es esta página
    aux["txt01"].as<AutoConnectText>().value = "Desde este portal podrá configurar algunas características del dispositivo OMC-WIFI.";
    aux["txt02"].as<AutoConnectText>().value = "<p>El <b>nombre</b> debe tener entre 2 y 32 caracteres y solo acepta el guión como caracter especial (NO puede ser el último).</p>";
    aux["txt03"].as<AutoConnectText>().value = "<p>La <b>clave</b> debe tener entre 8 y 16 caracateres.</p>";
    aux["txt04"].as<AutoConnectText>().value = "A continuación deberá introducir la <b>dirección IP</b> del servidor al que se desea conectar (la dirección IP actual es " + storage.getString("server_ip", "0.0.0.0") + ").";
    aux["txt05"].as<AutoConnectText>().value = "Elija <b>\"Sí\"</b> para restablecer los datos del dispositivo a los de fábrica (APs conectados, credenciales, dirección del servidor IP). <b>ESTE PROCESO ES IRREVERSIBLE</b>";

    aux["header01"].as<AutoConnectText>().value = "<h2>Cambiar Nombre del Dispositivo</h2>";
    aux["header02"].as<AutoConnectText>().value = "<h2>Cambiar Clave del Dispositivo</h2>";
    aux["header03"].as<AutoConnectText>().value = "<h2>Configurar Dirección IP del Servidor</h2>";
    aux["header04"].as<AutoConnectText>().value = "<h2>Restablecer Credenciales</h2>";


    //Se habilitan los botones y las entradas de texto
    aux["ssid"].as<AutoConnectInput>().enable    = true;
    aux["pass1"].as<AutoConnectInput>().enable   = true;
    aux["pass2"].as<AutoConnectInput>().enable   = true;
    aux["server"].as<AutoConnectInput>().enable  = true;

    aux["save01"].as<AutoConnectSubmit>().enable = true;
    aux["save02"].as<AutoConnectSubmit>().enable = true;
    aux["save03"].as<AutoConnectSubmit>().enable = true;
    aux["save04"].as<AutoConnectSubmit>().enable = true;

    aux["resetCred"].as<AutoConnectRadio>().enable = true;


    //Se limpian las entradas de datos al acceder al directorio
    aux["ssid"].as<AutoConnectInput>().value     = "";
    aux["pass1"].as<AutoConnectInput>().value    = "";
    aux["pass2"].as<AutoConnectInput>().value    = "";
    aux["server"].as<AutoConnectInput>().value   = "";

  }
  else {

    //Textos que aparecerán es esta página
    aux["txt01"].as<AutoConnectText>().value = "La <b>clave introducida</b> es <b>inválida</b>, no podrá configurar el dispositivo.";
    aux["txt02"].as<AutoConnectText>().value = "";
    aux["txt03"].as<AutoConnectText>().value = "";
    aux["txt04"].as<AutoConnectText>().value = "";
    aux["txt05"].as<AutoConnectText>().value = "";

    aux["header01"].as<AutoConnectText>().value = "";
    aux["header02"].as<AutoConnectText>().value = "";
    aux["header03"].as<AutoConnectText>().value = "";
    aux["header04"].as<AutoConnectText>().value = "";

    //Se deshabilitan los botones y las entradas de texto
    aux["ssid"].as<AutoConnectInput>().enable    = false;
    aux["pass1"].as<AutoConnectInput>().enable   = false;
    aux["pass2"].as<AutoConnectInput>().enable   = false;
    aux["server"].as<AutoConnectInput>().enable  = false;

    aux["save01"].as<AutoConnectSubmit>().enable = false;
    aux["save02"].as<AutoConnectSubmit>().enable = false;
    aux["save03"].as<AutoConnectSubmit>().enable = false;
    aux["save04"].as<AutoConnectSubmit>().enable = false;

    aux["resetCred"].as<AutoConnectRadio>().enable = false;

  }

  storage.end();                       // Se cierra el espacio en memoria flash denominado "storage"

  return String();

}


//Función para validar el cambio de SSID
String onChangeSSID(AutoConnectAux& aux, PageArgument& args) {
  AutoConnectInput& ssid = ap_config["ssid"].as<AutoConnectInput>();                                                                          //Se guarda el elemento AutoConnectInput denominado "ssid" de la página web ap_config, que guarda los datos del nuevo SSID introducidos por el usuario

  aux["header01"].as<AutoConnectText>().value = "<h2>Cambiar Nombre del Dispositivo</h2>";

  storage.begin("config", false);

  if (args.arg("ssid") == "") {                                                                                                               //Si NO se introdujo un SSID
    aux["txtCenter01"].as<AutoConnectText>().value = "NO se detectó ningún cambio en el <b>SSID</b>.\n";                                          //Aparecerá este mensaje en la página web
  }
  else if (ssid.isValid()) {                                                                                                                  //Si se introdujo un SSID y se cumplen las condiciones establecidas
    storage.begin("config", false);                                                                                                             //Se apertura el espacio en memoria flash denominado "storage" para leer y escribir (false)
    storage.putString("ssid", args.arg("ssid"));                                                                                                //Se guarda el SSID introducido por el usuario en la memoria flash
    Serial.println("El <b>SSID</b> ha cambiado a:\n" + storage.getString("ssid", ""));
    storage.end();                                                                                                                              //Se cierra el espacio en memoria flash denominado "storage"

    aux["txtCenter01"].as<AutoConnectText>().value = "El <b>SSID</b> ha cambiado a:\n" + args.arg("ssid");                                        //Aparecerá este mensaje en la página web
  }
  else {                                                                                                                                      //Si se introdujo un SSID pero NO se cumplen las condiciones establecidas
    aux["txtCenter01"].as<AutoConnectText>().value = "El <b>SSID</b> introducido NO cumple las condiciones.\nPor favor intente de nuevo.\n";      //Aparecerá este mensaje en la página web
  }

  aux["txt01"].as<AutoConnectText>().value = "<p><b>ADVERTENCIA:</b> para aplicar los cambios el dispositivo debe ser <b>reiniciado</b>, por lo que el dispositivo conectado al OMC-WIFI será desconectado del suministro eléctrico momentáneamente.</p>";

  return String();

}


// Función para validar el cambio de Contraseña
String onChangePass(AutoConnectAux& aux, PageArgument& args) {

  aux["header02"].as<AutoConnectText>().value = "<h2>Cambiar Clave del Dispositivo</h2>";

  if (args.arg("pass1") == args.arg("pass2")) {                                                                                             //Si las claves introducidas coinciden
    AutoConnectInput& pass = ap_config["pass1"].as<AutoConnectInput>();                                                                       //Se guarda el elemento AutoConnectInput denominado "pass1" de la página web ap_config, que guarda los datos de la nueva clave introducidos por el usuario
    //Verificando que la clave introducida cumple con las condiciones
    if (args.arg("pass1") == "") {                                                                                                            //Si NO se introdujo una clave
      aux["txtCenter01"].as<AutoConnectText>().value = "NO se detectó ningún cambio en la <b>clave</b>.\n";                                           //Aparecerá este mensaje en la página web
    }
    else if (pass.isValid()) {                                                                                                                //Si se introdujo una clave y se cumplen las condiciones establecidas
      storage.begin("config", false);                                                                                                           //Se apertura el espacio en memoria flash denominado "storage" para leer y escribir (false)
      storage.putString("pass", args.arg("pass1"));                                                                                             //Se guarda la clave introducida por el usuario en la memoria flash
      Serial.println("La <b>clave</b> ha cambiado a:\n" + storage.getString("pass", ""));
      storage.end();                                                                                                                            //Se cierra el espacio en memoria flash denominado "storage"

      aux["txtCenter01"].as<AutoConnectText>().value = "La <b>clave</b> ha sido cambiada exitosamente.\n";                                            //Aparecerá este mensaje en la página web
    }
    else {                                                                                                                                    //Si se introdujo una clave pero NO se cumplen las condiciones establecidas
      aux["txtCenter01"].as<AutoConnectText>().value = "La <b>clave</b> introducida NO cumple las condiciones.\nPor favor intente de nuevo.\n";       //Aparecerá este mensaje en la página web
    }
  }
  else {                                                                                                                                    //Si las claves introducidas coinciden
    aux["txtCenter01"].as<AutoConnectText>().value = "Las <b>claves</b> introducidas NO coinciden.\nPor favor intente de nuevo.\n";                 //Aparecerá este mensaje en la página web
  }

  aux["txt01"].as<AutoConnectText>().value = "<p><b>ADVERTENCIA:</b> para aplicar los cambios el dispositivo debe ser <b>reiniciado</b>, por lo que el dispositivo conectado al OMC-WIFI será desconectado del suministro eléctrico momentáneamente.</p>";

  return String();

}


// Función para validar la dirección IP del servidor
String onServerIP(AutoConnectAux& aux, PageArgument& args) {
  AutoConnectInput& server = ap_config["server"].as<AutoConnectInput>();                                                            //Se guarda el elemento AutoConnectInput denominado "server" de la página web ap_config, que guarda los datos de la dirección IP introducidos por el usuario

  aux["header03"].as<AutoConnectText>().value = "<h2>Configurar Dirección IP del Servidor</h2>";

  if (args.arg("server") == "") {                                                                                                   //Si NO se introdujo una dirección IP
    aux["txtCenter01"].as<AutoConnectText>().value = "NO introdujo ninguna <b>dirección IP</b>.\n";                                         //Aparecerá este mensaje en la página web
  }
  else if (server.isValid()) {                                                                                                      //Si se introdujo una dirección IP y se cumplen las condiciones establecidas
    storage.begin("config", false);                                                                                                   //Se apertura el espacio en memoria flash denominado "storage" para leer y escribir (false)
    storage.putString("server_ip", args.arg("server"));                                                                               //Se guarda la dirección IP en la memoria flash
    Serial.println("La <b>dirección IP del servidor</b> ha cambiado a:\n" + storage.getString("server_ip", ""));
    storage.end();                                                                                                                    //Se cierra el espacio en memoria flash denominado "storage"

    mqttClient.setServer(MQTT_HOST.fromString(args.arg("server")), MQTT_PORT);

    aux["txtCenter01"].as<AutoConnectText>().value = "La <b>dirección IP del servidor</b> ha cambiado a:\n" + args.arg("server");           //Aparecerá este mensaje en la página web
  }
  else {                                                                                                                            //Si se introdujo una dirección IP pero NO se cumplen las condiciones establecidas
    aux["txtCenter01"].as<AutoConnectText>().value = "La <b>dirección IP</b> introducida es inválida.\nPor favor intente de nuevo.\n";      //Aparecerá este mensaje en la página web
  }

  return String();

}


// Función para validar el reinicio de credenciales
String onCredentialReset(AutoConnectAux& aux, PageArgument& args) {

  aux["header04"].as<AutoConnectText>().value = "<h2>Restablecer Credenciales</h2>";

  storage.begin("config", false);                                                                                 //Se apertura el espacio en memoria flash denominado "storage" para leer y escribir (false)

  if (args.arg("resetCred") == "Sí") {                                               //Si la clave introducida por el usuario coincide con la actual del ESP32

    nvs_flash_deinit();     // Se desinicializa la partición NVS (necesario para poder borrarla)
    nvs_flash_erase();      // Se borra la partición NVS
    nvs_flash_init();       // Se inicializa la partición NVS

    aux["txtCenter01"].as<AutoConnectText>().value = "Los <b>datos guardados</b> han sido restablecidas exitosamente.\n";    //Aparecerá este mensaje en la página web
    aux["txt01"].as<AutoConnectText>().value = "<p><b>ADVERTENCIA:</b> para aplicar los cambios el dispositivo debe ser <b>reiniciado</b>, por lo que el dispositivo conectado al OMC-WIFI será desconectado del suministro eléctrico momentáneamente.</p>";

  }
  else {                                                                                                          //Si la clave introducida por el usuario coincide con la actual del ESP32
    aux["txtCenter01"].as<AutoConnectText>().value = "Los <b>datos guardados</b> se han mantenido.";                  //Aparecerá este mensaje en la página web
    aux["txt01"].as<AutoConnectText>().value = "";

  }

  storage.end();                                                                                                  //Se cierra el espacio en memoria flash denominado "storage"


  return String();

}


String onSupply(AutoConnectAux& aux, PageArgument& args) {

  //Textos que aparecerán es esta página
  aux["txtCenter01"].as<AutoConnectText>().value = String(rmsVolt) + " Volts";
  aux["txtCenter02"].as<AutoConnectText>().value = String(rmsCorr) + " Amps";

  if (relay == HIGH) {
    aux["txtCenter03"].as<AutoConnectText>().value = "Encendido";
  }
  else {
    aux["txtCenter03"].as<AutoConnectText>().value = "Apagado";
  }

  if (controlGlobalRelay == true) {
    aux["txt04"].as<AutoConnectText>().value = "Desde este portal podrá <b>cortar</b> o <b>reestablecer</b> el suministro eléctrico a su dispositivo. Para ello, por favor introduzca la <b>clave actual</b> del dispositivo.\n Actualmente el suministro se encuentra <b>habilitado manualmente</b>.";

  }
  else{
    aux["txt04"].as<AutoConnectText>().value = "Desde este portal podrá <b>cortar</b> o <b>reestablecer</b> el suministro eléctrico a su dispositivo. Para ello, por favor introduzca la <b>clave actual</b> del dispositivo.\n Actualmente el suministro se encuentra <b>cortado manualmente</b>.";

  }


  aux["header01"].as<AutoConnectText>().value = "<h2>Voltaje</h2>";
  aux["header02"].as<AutoConnectText>().value = "<h2>Corriente</h2>";
  aux["header03"].as<AutoConnectText>().value = "<h2>Estado del Relay</h2>";
  aux["header04"].as<AutoConnectText>().value = "<h2>Cortar/Reestablecer Suministro</h2>";


  aux["passVer"].as<AutoConnectInput>().value  = "";

  return String();

}


String onSwitchRelay(AutoConnectAux& aux, PageArgument& args) {

  //AutoConnectText& switchRelay = supply["switchState"].as<AutoConnectText>();
  if (args.arg("passVer") == storage.getString("pass", "12345678")) {

    aux["txtCenter01"].as<AutoConnectText>().value = "";

    //if (aux["switchState"].as<AutoConnectText>().value == "Suministro reestablecido.") {
    if (controlGlobalRelay == true) {
      controlGlobalRelay  = false;
      aux["txtCenter02"].as<AutoConnectText>().value = "Suministro cortado manualmente.";

    }
    //else if (aux["switchState"].as<AutoConnectText>().value == "Suministro cortado.") {
    else {
      controlGlobalRelay  = true;
      aux["txtCenter02"].as<AutoConnectText>().value = "Suministro reestablecido manualmente.";

    }
  }
  else {
    aux["txtCenter01"].as<AutoConnectText>().value = "La <b>clave introducida</b> es <b>inválida</b>. No podrá cambiar el estado del suministro.";
    aux["txtCenter02"].as<AutoConnectText>().value = "";

  }


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

  //AutoConnectText& switchRelay = switch_relay["switchState"].as<AutoConnectText>();

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

  //  if (controlGlobalRelay == true) {
  //    switchRelay.value = "Suministro reestablecido.";
  //  }
  //  else {
  //    switchRelay.value = "Suministro cortado.";
  //  }

  config.apip      = IPAddress(172, 22, 174, 254);                        //Se configura la dirección IPv4 del AP ESP32
  config.title     = "OMC-WIFI-" + chipID;                                //Título de la página web
  config.homeUri   = "/_ac";                                              //Directorio HOME de la página web
  config.menuItems = AC_MENUITEM_CONFIGNEW | AC_MENUITEM_OPENSSIDS | AC_MENUITEM_RESET | AC_MENUITEM_HOME;                           //Se deshabilita el menú de desconectar del AP
  Portal.join({pre_config, ap_config, ap_ssid, ap_pass, cred_reset, server_ip, supply, switch_relay});    //Se cargan las páginas web diseñadas en el portal web
  Portal.on("/pre-config", onPreConfig);                                //Se enlaza la función "onPreConfig" con la página en el directorio "/pre-config" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/ap-config", onConfig);                                    //Se enlaza la función "onConfig" con la página en el directorio "/ap-config" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/ap-ssid", onChangeSSID);                                  //Se enlaza la función "onChangeSSID" con la página en el directorio "/ap-ssid" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/ap-pass", onChangePass);                                  //Se enlaza la función "onChangePass" con la página en el directorio "/ap-pass" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/cred-reset", onCredentialReset);                          //Se enlaza la función "onCredentialReset" con la página en el directorio "/cred-reset" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/server-ip", onServerIP);                                  //Se enlaza la función "onServerIP" con la página en el directorio "/server-ip" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/supply", onSupply);                                       //Se enlaza la función "onSupply" con la página en el directorio "/supply" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/switch-relay", onSwitchRelay);                            //Se enlaza la función "onSwitchRelay" con la página en el directorio "/switch-relay" (la función se ejecutará cada vez que se acceda al directorio)

  Portal.config(config);                                                //Se añaden las configuraciones al portal web
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
    2,                      //Prioridad de la tarea (de 0 a 25)
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
