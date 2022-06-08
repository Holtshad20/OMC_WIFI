#include "OMC_WIFI_main.hpp"


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

  storage.begin("config", true);       // Se apertura el espacio en memoria flash denominado "storage" para leer (true)

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

    credReset();

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


// Función para actualizar los datos e la página del suministro eléctrico
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
  else {
    aux["txt04"].as<AutoConnectText>().value = "Desde este portal podrá <b>cortar</b> o <b>reestablecer</b> el suministro eléctrico a su dispositivo. Para ello, por favor introduzca la <b>clave actual</b> del dispositivo.\n Actualmente el suministro se encuentra <b>cortado manualmente</b>.";

  }


  aux["header01"].as<AutoConnectText>().value = "<h2>Voltaje</h2>";
  aux["header02"].as<AutoConnectText>().value = "<h2>Corriente</h2>";
  aux["header03"].as<AutoConnectText>().value = "<h2>Estado del Relay</h2>";
  aux["header04"].as<AutoConnectText>().value = "<h2>Cortar/Reestablecer Suministro</h2>";


  aux["passVer"].as<AutoConnectInput>().value  = "";

  return String();

}

//Función para actualizar los datos de la página de switcheo del relé
String onSwitchRelay(AutoConnectAux& aux, PageArgument& args) {

  storage.begin("config", true);

  //AutoConnectText& switchRelay = supply["switchState"].as<AutoConnectText>();
  if (args.arg("passVer") == storage.getString("pass", "12345678")) {

    aux["txtCenter01"].as<AutoConnectText>().value = "";

    //if (aux["switchState"].as<AutoConnectText>().value == "Suministro reestablecido.") {
    if (controlGlobalRelay == true) {
      controlGlobalRelay  = false;
      aux["txtCenter02"].as<AutoConnectText>().value = "Suministro cortado manualmente.";
      mqttClient.publish("esp32/controlRelay", 0, true, "0");
    }
    //else if (aux["switchState"].as<AutoConnectText>().value == "Suministro cortado.") {
    else {
      controlGlobalRelay  = true;
      aux["txtCenter02"].as<AutoConnectText>().value = "Suministro reestablecido manualmente.";
      mqttClient.publish("esp32/controlRelay", 0, true, "1");
    }
  }
  else {
    aux["txtCenter01"].as<AutoConnectText>().value = "La <b>clave introducida</b> es <b>inválida</b>. No podrá cambiar el estado del suministro.";
    aux["txtCenter02"].as<AutoConnectText>().value = "";

  }

  storage.end();

  return String();

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


//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************


//Función para obtner el ChipID para el hostname
void getChipID() {

  byte mac[6];

  //Se toman los 3 últimos bytes de la MAC del ESP32
  WiFi.macAddress(mac);

  for (int i = 3; i < 6; i++) {
    if (mac[i] < 0x10) {
      hostname += '0';
    }
    hostname += String(mac[i], HEX);
  }

  hostname = "OMC-WIFI-" + hostname;

}

//Función para configurar inicialmente los parámetros de AutoConnect
void acSetUp(void) {

  getChipID();

  //Se hace la configuración inicial del AP
  storage.begin("config", true);                          //Se apertura el espacio de las credenciales para leer y sin posibilidad de escribir (true)

  //Configuración Hostname y SSID
  if (storage.getString("ssid", "") != "") {              //Si hay un SSID guardado en memoria, se cambia
    config.hostName = storage.getString("ssid", "");        //Se extrae el hostname guardado en el espacio de memoria "storage"
    config.apid     = storage.getString("ssid", "");        //Se extrae el SSID guardado en el espacio de memoria "storage"
  }
  else {                                                  //Si NO hay un SSID guardado en memoria, se coloca el default
    config.hostName = hostname;    //Se coloca el hostname por defecto
    config.apid     = hostname;    //Se coloca el hostname por defecto
  }

  //Configuración Contraseña
  if (storage.getString("pass", "") != "") {              //Si hay una clave guardada en memoria, se cambia
    config.psk = storage.getString("pass", "");             //Se extrae la contraseña guardada en el espacio de memoria "storage"
  }
  else {                                                  //Si NO hay una clave guardada en memoria, se cambia
    config.psk = "12345678";                                //Se coloca la contraseña por defecto
  }

  storage.end();

  config.apip       = IPAddress(172, 16, 16, 1);      //Se configura la dirección IPv4 del AP ESP32
  config.gateway    = IPAddress(172, 16, 16, 1);      //Se configura la dirección IPv4 del gateway
  config.retainPortal = true;                         //Se mantiene el portal
  //config.preserveAPMode = true;
  config.title      = hostname;                       //Título de la página web
  config.homeUri    = "/_ac";                         //Directorio HOME de la página web
  config.menuItems  = AC_MENUITEM_CONFIGNEW | AC_MENUITEM_OPENSSIDS | AC_MENUITEM_RESET | AC_MENUITEM_HOME;     //Se deshabilita el menú de desconectar del AP
  Portal.join({pre_config, ap_config, ap_ssid, ap_pass, cred_reset, server_ip, supply, switch_relay});    //Se cargan las páginas web diseñadas en el portal web
  Portal.on("/pre-config", onPreConfig);              //Se enlaza la función "onPreConfig" con la página en el directorio "/pre-config" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/ap-config", onConfig);                  //Se enlaza la función "onConfig" con la página en el directorio "/ap-config" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/ap-ssid", onChangeSSID);                //Se enlaza la función "onChangeSSID" con la página en el directorio "/ap-ssid" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/ap-pass", onChangePass);                //Se enlaza la función "onChangePass" con la página en el directorio "/ap-pass" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/cred-reset", onCredentialReset);        //Se enlaza la función "onCredentialReset" con la página en el directorio "/cred-reset" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/server-ip", onServerIP);                //Se enlaza la función "onServerIP" con la página en el directorio "/server-ip" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/supply", onSupply);                     //Se enlaza la función "onSupply" con la página en el directorio "/supply" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/switch-relay", onSwitchRelay);          //Se enlaza la función "onSwitchRelay" con la página en el directorio "/switch-relay" (la función se ejecutará cada vez que se acceda al directorio)

  Portal.config(config);                              //Se añaden las configuraciones al portal web

}

//Código para el portal captivo de AutoConnect
void acTask (void *acParameter) {
  Serial.println("AutoConnect Task created");

  Portal.begin();                                                       //Se inicializa el portal una vez ha sido configurado
  Server.on("/", rootPage);                                             //Se inicializa el servidor web

  while (true) {
    Portal.handleClient();
  }

}


//Función de configuración inicial del microcontrolador
void setup() {

  Serial.begin(115200);

  vTaskDelay(500 / portTICK_PERIOD_MS);

  Serial.println("Inicializando " + hostname);
  Serial.println(controlGlobalRelay);

  mqttSetUp();
  readSetUp();
  relaySetUp();
  acSetUp();
  ledSetUp();
  touchSetUp();


  //Tarea para ejecutar el código de AutoConnect
  xTaskCreatePinnedToCore(
    acTask,                 //Función que se ejecutará en la tarea
    "AutoConnectCode",      //Nombre descriptivo
    8192,                  //Tamaño del Stack para esta tarea
    NULL,                   //Parámetro para guardar la función
    1,                      //Prioridad de la tarea (de 0 a 25)
    NULL,                   //Manejador de tareas
    1);                     //Núcleo en el que se ejecutará


  //Tarea para ejecutar el código de lectura analógica de voltaje y corriente y control del relay
  xTaskCreatePinnedToCore(
    readCode,         //Función que se ejecutará en la tarea
    "readCode",       //Nombre descriptivo
    8192,                  //Tamaño del Stack para esta tarea
    NULL,                   //Parámetro para guardar la función
    1,                      //Prioridad de la tarea (de 0 a 25)
    NULL,                   //Manejador de tareas
    1);                     //Núcleo en el que se ejecutará


  xTaskCreatePinnedToCore(
    greenLedTask,               //Función que se ejecutará en la tarea
    "greenLedTask",             //Nombre descriptivo
    1024,                       //Tamaño del Stack para esta tarea
    NULL,                       //Parámetro para guardar la función
    0,                          //Prioridad de la tarea (de 0 a 25)
    NULL,                       //Manejador de tareas
    0);                         //Núcleo en el que se ejecutará


  xTaskCreatePinnedToCore(
    redLedTask,                 //Función que se ejecutará en la tarea
    "redLedTask",               //Nombre descriptivo
    1024,                       //Tamaño del Stack para esta tarea
    NULL,                       //Parámetro para guardar la función
    0,                          //Prioridad de la tarea (de 0 a 25)
    NULL,                       //Manejador de tareas
    0);                         //Núcleo en el que se ejecutará


  //Tarea para ejecutar el código de lectura analógica de voltaje y corriente y control del relay
  xTaskCreatePinnedToCore(
    touchTask,              //Función que se ejecutará en la tarea
    "touch",                //Nombre descriptivo
    4096,                   //Tamaño del Stack para esta tarea
    NULL,                   //Parámetro para guardar la función
    1,                      //Prioridad de la tarea (de 0 a 25)
    &xTouchHandle,          //Manejador de tareas
    1);                     //Núcleo en el que se ejecutará

  vTaskSuspend(xTouchHandle);

  //vTaskStartScheduler();    //Se inicializa el organizador de tareas

}


void loop() {

}
