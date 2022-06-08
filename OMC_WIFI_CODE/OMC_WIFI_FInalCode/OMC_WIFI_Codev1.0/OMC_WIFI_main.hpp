#ifndef OMC_WIFI_MAIN_HPP
#define OMC_WIFI_MAIN_HPP


#include "OMC_WIFI_mqtt.hpp"
#include "OMC_WIFI_read.hpp"
#include "OMC_WIFI_Leds.hpp"
#include "OMC_WIFI_TouchSensor.hpp"
#include <AutoConnect.h>
#include <AutoConnectCredential.h>
#include <WebServer.h>


WebServer         Server;             // Constructor del servidor web del ESP32
AutoConnect       Portal(Server);     // Constructor del portal captivo del ESP32
AutoConnectConfig config;             // Constructor de las configuraciones del AutoConnect

boolean inside = false;

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
ACSubmit(backSupply, "Volver", "/supply");
ACButton(updateData, "Actualizar Datos", "window.location = window.location.href");


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


//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*************************************************    FUNCIONES DE LAS PÁGINAS WEB DE AUTOCONNECT     **********************************************************
//***************************************************************************************************************************************************************

// Función para reiniciar los datos en la página de configuración del AP
String onPreConfig(AutoConnectAux& aux, PageArgument& args);

// Función para indicar los datos iniciales en la página de configuración del AP
String onConfig(AutoConnectAux& aux, PageArgument& args);

//Función para validar el cambio de SSID
String onChangeSSID(AutoConnectAux& aux, PageArgument& args);

// Función para validar el cambio de Contraseña
String onChangePass(AutoConnectAux& aux, PageArgument& args);

// Función para validar la dirección IP del servidor
String onServerIP(AutoConnectAux& aux, PageArgument& args);

// Función para validar el reinicio de credenciales
String onCredentialReset(AutoConnectAux& aux, PageArgument& args);

// Función para actualizar los datos e la página del suministro eléctrico
String onSupply(AutoConnectAux& aux, PageArgument& args);

//Función para actualizar los datos de la página de switcheo del relé
String onSwitchRelay(AutoConnectAux& aux, PageArgument& args);

// Función para generar la página de inicio
void rootPage();
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************


void getChipID();

void acSetUp(void);

void acTask (void *acParameter);


#endif
