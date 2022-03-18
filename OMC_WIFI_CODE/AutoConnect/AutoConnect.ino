#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <AutoConnectCredential.h>
#include <Preferences.h>

WebServer         Server;
AutoConnect       Portal(Server);
AutoConnectConfig config;             //Credenciales para acceder al AP
Preferences       storage;            //Espacio en memoria para guardar los datos necesarios

String chipID;
boolean manualControl = 1;


//Declaración de elementos AutoConnect para la página web de Configuración del AP
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
ACSubmit(back, "Volver al menú", "/_ac");

//Declaración de elementos AutoConnect para la página web para guardado de SSID
ACText(txt11, "", "text-align:center");
ACText(note11, "<p><b>ADVERTENCIA:</b> para aplicar los cambios el dispositivo debe ser <b>reiniciado</b>, por lo que el dispositivo conectado al OMC-WIFI será desconectado del suministro eléctrico momentáneamente.</p>", "text-align:justify");
ACSubmit(reset, "Reiniciar equipo", "/_ac#rdlg");

//Declaración de elementos AutoConnect para la página web para guardado de Contraseña
ACText(txt21, "", "text-align:center");


//Declaración de elementos AutoConnect para la página web para restablecer las credenciales
ACText(txt31, "", "text-align:center");

//Declaración de elementos AutoConnect para la página web para configurar la dirección IP del servidor
ACText(txt41, "", "text-align:center");


//Declaración de elementos AutoConnect para la página web de corte del suministro eléctrico
ACText(caption02, "Desde este portal podrá <b>cortar</b> o <b>reestablecer</b> el suministro eléctrico a su dispositivo", "text-align:justify;font-family:serif;color:#000000;");
ACButton(cut, "Cortar", "manualControl = 0");
ACButton(restore, "Reestablecer", "manualControl = 1");

//Declaración de la página web para la página web de Configuración del AP
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
  back,

});

//Declaración de la página web para indicar el cambio de SSID
AutoConnectAux ap_ssid("/ap_ssid", "Configuración del Dispositivo", false, {

  header01,
  txt11,
  note11,
  reset,
  back,

});

//Declaración de la página web para la página web para indicar el cambio de clave
AutoConnectAux ap_pass("/ap_pass", "Configuración del Dispositivo", false, {

  header02,
  txt21,
  note11,
  reset,
  back,

});

//Declaración de la página web para restablecer las credenciales a las de fábrica
AutoConnectAux cred_reset("/cred_reset", "Configuración del Dispositivo", false, {

  header03,
  txt31,
  note11,
  reset,
  back,

});

//Declaración de la página web para configurar la dirección IP del servidor
AutoConnectAux server_ip("/server_ip", "Configuración del Dispositivo", false, {

  header04,
  txt41,
  back,

});

AutoConnectAux cut_supply("/cut_supply", "Suministro Eléctrico", true, {

  caption02,
  cut,
  restore,

});


//Función para colocar los datos iniciales en la página de configuración del AP
String onConfig(AutoConnectAux& aux, PageArgument& args) {

  aux["ssid"].as<AutoConnectInput>().value   = "";
  aux["pass1"].as<AutoConnectInput>().value  = "";
  aux["pass2"].as<AutoConnectInput>().value  = "";
  aux["pass3"].as<AutoConnectInput>().value  = "";
  aux["server"].as<AutoConnectInput>().value = "";

  return String();

}

//Función para validar el cambio de SSID
String onChangeSSID(AutoConnectAux& aux, PageArgument& args) {
  AutoConnectInput& ssid = ap_config["ssid"].as<AutoConnectInput>();                                   //Se guarda el elemento AutoConnectInput denominado ssid de la página web ap_config

  if (args.arg("ssid") == "") {
    aux["txt11"].as<AutoConnectText>().value = "NO se detectó ningún cambio en el <b>SSID</b>.\n";
  }
  else if (ssid.isValid()) {
    storage.begin("config", false);                                                                    //Se apertura el espacio de las credenciales para leer y escribir (false)
    storage.putString("ssid", args.arg("ssid"));                                                            //Se guarda el SSID
    Serial.println("El <b>SSID</b> ha cambiado a:\n" + storage.getString("ssid", ""));
    storage.end();

    aux["txt11"].as<AutoConnectText>().value = "El <b>SSID</b> ha cambiado a:\n" + args.arg("ssid");
  }
  else {
    aux["txt11"].as<AutoConnectText>().value = "El <b>SSID</b> introducido NO cumple las condiciones.\nPor favor intente de nuevo.\n";
  }

  return String();

}

//Función para validar el cambio de Contraseña
String onChangePass(AutoConnectAux& aux, PageArgument& args) {
  //Verificando que las claves introducidas coinciden
  if (args.arg("pass1") == args.arg("pass2")) {
    AutoConnectInput& pass = ap_config["pass1"].as<AutoConnectInput>();                                //Se guarda el elemento AutoConnectInput denominado pass1 de la página web ap_config
    //Verificando que la clave introducida cumple con las condiciones
    if (args.arg("pass1") == "") {
      aux["txt21"].as<AutoConnectText>().value = "NO se detectó ningún cambio en la <b>clave</b>.\n";
    }
    else if (pass.isValid()) {
      storage.begin("config", false);                                                                  //Se apertura el espacio de las credenciales para leer y escribir (false)
      storage.putString("pass", args.arg("pass1"));                                                         //Se guarda la clave
      Serial.println("La <b>clave</b> ha cambiado a:\n" + storage.getString("pass", ""));
      storage.end();

      aux["txt21"].as<AutoConnectText>().value = "La <b>clave</b> ha sido cambiada exitosamente.\n";
    }
    else {
      aux["txt21"].as<AutoConnectText>().value = "La <b>clave</b> introducida NO cumple las condiciones.\nPor favor intente de nuevo.\n";
    }
  }
  else {
    aux["txt21"].as<AutoConnectText>().value = "Las <b>claves</b> introducidas NO coinciden.\nPor favor intente de nuevo.\n";
  }

  return String();

}

String onCredentialReset(AutoConnectAux& aux, PageArgument& args) {
  storage.begin("config", false);                           //Se apertura el espacio de las credenciales para leer y escribir (false)

  if (args.arg("pass3") == storage.getString("pass", "12345678")) {
    storage.putString("ssid", "OMC-WIFI-" + chipID);    //Se guarda el SSID
    storage.putString("pass", "12345678");    //Se guarda la clave
    Serial.println("La <b>clave</b> ha cambiado a:\n" + storage.getString("pass", ""));
    Serial.println("La <b>clave</b> ha cambiado a:\n" + storage.getString("pass", ""));
    storage.end();

    aux["txt31"].as<AutoConnectText>().value = "Las <b>credenciales</b> han sido restablecidas exitosamente.\n";
  }
  else {
    aux["txt31"].as<AutoConnectText>().value = "La <b>clave</b> introducida es inválida.\nIntente de nuevo.";
  }

  storage.end();
  return String();

}

String onServerIP(AutoConnectAux& aux, PageArgument& args) {
  AutoConnectInput& server = ap_config["server"].as<AutoConnectInput>();                                   //Se guarda el elemento AutoConnectInput denominado server de la página web ap_config

  if (args.arg("server") == "") {
    aux["txt41"].as<AutoConnectText>().value = "NO introdujo ninguna <b>dirección IP</b>.\n";
  }
  else if (server.isValid()) {
    storage.begin("config", false);                                                                    //Se apertura el espacio de las credenciales para leer y escribir (false)
    storage.putString("server_ip", args.arg("server"));                                                            //Se guarda el SSID
    Serial.println("La <b>dirección IP del servidor</b> ha cambiado a:\n" + storage.getString("server_ip", ""));
    storage.end();

    aux["txt41"].as<AutoConnectText>().value = "La <b>dirección IP del servidor</b> ha cambiado a:\n" + args.arg("server");
  }
  else {
    aux["txt41"].as<AutoConnectText>().value = "La <b>dirección IP</b> introducida es inválida.\nPor favor intente de nuevo.\n";
  }

  return String();

}

//Función para generar la página de inicio
void rootPage() {
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
  Server.send(200, "text/html", content);
}


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


//Función para configurar inicialmente los parámetros
void acConfig(void) {
  byte mac[6];                                        //Variable para guardar la MAC del ESP32


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
  storage.begin("config", true);

  //Configuración Hostname y SSID
  if (storage.getString("ssid", "") != "") {          //Si hay un SSID guardado en memoria, se cambia
    config.hostName = storage.getString("ssid", "");
    config.apid     = storage.getString("ssid", "");
  }
  else {                                              //Si NO hay un SSID guardado en memoria, se coloca el default
    config.hostName = "OMC-WIFI-" + chipID;
    config.apid     = "OMC-WIFI-" + chipID;
  }

  //Configuración Contraseña
  if (storage.getString("pass", "") != "") {          //Si hay una clave guardada en memoria, se cambia
    config.psk = storage.getString("pass", "");
  }
  else {                                              //Si NO hay una clave guardada en memoria, se cambia
    config.psk = "12345678";
  }

  storage.end();

  config.apip    = IPAddress(172, 22, 174, 254);
  config.title   = "OMC-WIFI-" + chipID;
  config.homeUri = "/_ac",
  Portal.config(config);
  Portal.join({ap_config, ap_ssid, ap_pass, cred_reset, server_ip, cut_supply});
  Portal.on("/ap_config", onConfig);
  Portal.on("/ap_ssid", onChangeSSID);
  Portal.on("/ap_pass", onChangePass);
  Portal.on("/cred_reset", onCredentialReset);
  Portal.on("/server_ip", onServerIP);
  Portal.begin();

  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }

}

//Función de configuración
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("Inicializando OMC-WIFI-" + chipID);

  acConfig();

}


void loop() {
  Portal.handleClient();

  if(manualControl == 1){
    Serial.println("CORTADO");
  }
  else{
    Serial.println("Restablecido");
  }
}
