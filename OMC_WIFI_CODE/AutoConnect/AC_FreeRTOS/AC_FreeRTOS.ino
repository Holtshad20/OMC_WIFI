#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <AutoConnectCredential.h>
#include <Preferences.h>



//***************************************************************************************************************************************************************
//*******************************************************    CONSTANTES Y CONSTRUCTORES PARA AUTOCONNECT     ****************************************************
//***************************************************************************************************************************************************************

WebServer         Server;             //Constructor del servidor web del ESP32
AutoConnect       Portal(Server);     //Constructor del portal captivo del ESP32
AutoConnectConfig config;             //Constructor de las configuraciones del AutoConnect
Preferences       storage;            //Espacio en memoria para guardar los datos necesarios

String chipID;                        //Variable donde se guardan los últimos 3 bytes de la dirección MAC (ESP.getEfuseMAC extrae los bytes deordenados)

boolean manualControl = true;

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************

//const char* scSwitchSupply = R"(
//<script type='text/javascript'>
//function switchSupply(switchState) {
//  if (switchState){
//    manualControl = true;
//  }
//  else{
//    manualControl = false;
//  }
//}
//</script>
//)";


//***************************************************************************************************************************************************************
//*************************************************    VARIABLES, CONSTANTES Y ARREGLOS PARA LECTURA ANALÓGICA     **********************************************
//***************************************************************************************************************************************************************

uint32_t promedioVolt = 0;              //Variable para guardar el promedio de las mediciones (de voltaje) analógicas, usado para eliminar nivel DC
uint32_t promedioCorr = 0;              //Variable para guardar el promedio de las mediciones (de corriente) analógicas, usado para eliminar nivel DC

float lecturaVolt[1000];                //Arreglo con valores del ADC para promedio (Ventana)
float lecturaCorr[1000];                //Arreglo con valores del ADC para promedio (Ventana)
float cuadradoVolt[1000];               //Arreglo con valores cuadrados que se sumarán (Ventana)
float cuadradoCorr[1000];               //Arreglo con valores cuadrados que se sumarán (Ventana)
uint16_t pos = 0;                       //Posición en la ventana de los valores cuadrados

const float multVolt = 1.11 * 1.11;     //Factor de escala para medir voltaje 1.11
const float multCorr = 108 * 108;       //Factor de escala para medir corriente 108

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*************************************************    ELEMENTOS Y CONSTRUCTORES PARA PÁGINA WEB DE AUTOCONNECT     *********************************************
//***************************************************************************************************************************************************************

//Declaración de elementos AutoConnect para la página web de configuración del AP del ESP32
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


//Declaración de elementos AutoConnect para la página web para guardado de contraseña
ACText(txt21, "", "text-align:center");


//Declaración de elementos AutoConnect para la página web para restablecer las credenciales
ACText(txt31, "", "text-align:center");


//Declaración de elementos AutoConnect para la página web para configurar la dirección IP del servidor
ACText(txt41, "", "text-align:center");


//Declaración de elementos AutoConnect para la página web de corte del suministro eléctrico
ACText(caption02, "Desde este portal podrá <b>cortar</b> o <b>reestablecer</b> el suministro eléctrico a su dispositivo", "text-align:justify;font-family:serif;color:#000000;");
ACButton(cut, "Cortar suministro", "switchSupply(false)");
ACButton(restore, "Reestablecer suministro", "switchSupply(true)");
ACElement(SwitchSupply, "<script type='text/javascript'>function switchSupply(switchState) {if (switchState){manualControl = true;}else{manualControl = false;}}</script>");


//Declaración de la página web para la página web de configuración del AP del ESP32
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

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************


//***************************************************************************************************************************************************************
//*************************************************    FUNCIONES DE LAS PÁGINAS WEB DE AUTOCONNECT     **********************************************************
//***************************************************************************************************************************************************************

//Función para reiniciar los datos en la página de configuración del AP
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


//Función para validar el cambio de Contraseña
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


//Función para validar el reinicio de credenciales
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


//Función para validar la dirección IP del servidor
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

    aux["txt41"].as<AutoConnectText>().value = "La <b>dirección IP del servidor</b> ha cambiado a:\n" + args.arg("server");           //Aparecerá este mensaje en la página web
  }
  else {                                                                                                                            //Si se introdujo una dirección IP pero NO se cumplen las condiciones establecidas
    aux["txt41"].as<AutoConnectText>().value = "La <b>dirección IP</b> introducida es inválida.\nPor favor intente de nuevo.\n";      //Aparecerá este mensaje en la página web
  }

  return String();

}


//void switchSupply (boolean switchState){
//
//  if (switchState == false){
//    manualControl = false;
//  }
//  else{
//    manualControl = true;
//  }
//  
//}

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************


//Función para generar la página de inicio
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

//Función para configurar inicialmente los parámetros de AutoConnect
void acSetUp(void) {
  byte mac[6];

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

  config.apip    = IPAddress(172, 22, 174, 254);                        //Se configura la dirección IPv4 del AP ESP32
  config.title   = "OMC-WIFI-" + chipID;                                //Título de la página web
  config.homeUri = "/_ac",                                              //Directorio HOME de la página web
         Portal.config(config);                                                //Se añaden las configuraciones al portal web
  Portal.join({ap_config, ap_ssid, ap_pass, cred_reset, server_ip, cut_supply});    //Se cargan las páginas web diseñadas en el portal web
  Portal.on("/ap_config", onConfig);                                    //Se enlaza la función "onConfig" con la página en el directorio "/ap_config" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/ap_ssid", onChangeSSID);                                  //Se enlaza la función "onChangeSSID" con la página en el directorio "/ap_ssid" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/ap_pass", onChangePass);                                  //Se enlaza la función "onChangePass" con la página en el directorio "/ap_pass" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/cred_reset", onCredentialReset);                          //Se enlaza la función "onCredentialReset" con la página en el directorio "/cred_reset" (la función se ejecutará cada vez que se acceda al directorio)
  Portal.on("/server_ip", onServerIP);                                  //Se enlaza la función "onServerIP" con la página en el directorio "/server_ip" (la función se ejecutará cada vez que se acceda al directorio)
  //  Portal.begin();                                                       //Se inicializa el portal una vez ha sido configurado
  //
  //  Server.on("/", rootPage);                                             //Se inicializa el servidor web
  //  if (Portal.begin()) {                                                 //Si se configura una conexión del ESP32 a un punto de acceso
  //    Serial.println("WiFi connected: " + WiFi.localIP().toString());       //Se imprime la dirección IP del ESP32 en esa red en la pantalla serial
  //  }

}


void analogReadSetUp(void) {

  //Calculando el promedio (nivel DC) con 1000 muestras
  for (int i = 0; i <= 999; i++) {
    promedioVolt += analogRead(39);
    vTaskDelay((1 / (4 * 60)) / portTICK_PERIOD_MS);           //Frecuencia de muestreo de 4 veces 60 Hz
  }
  promedioVolt = promedioVolt / 1000;

  //Calculando el promedio (nivel DC) con 1000 muestras
  for (int i = 0; i <= 999; i++) {
    promedioCorr += analogRead(36);
    vTaskDelay((1 / (4 * 60)) / portTICK_PERIOD_MS);           //Frecuencia de muestreo de 4 veces 60 Hz
  }
  promedioCorr = promedioCorr / 1000;

  pinMode(23, OUTPUT);                    //Se inicializa el PIN 23 como salida

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
  float rmsVolt;                                  //Variable para guardar el valor RMS de Voltaje
  float rmsCorr;                                  //Variable para guardar el valor RMS de Corriente

  Serial.println("Analog Read Task created");

  while (true) {
    suma    = 0;
    rmsVolt = 0;
    rmsCorr = 0;

    lecturaVolt[pos] = analogRead(39);                  //Se lee ADC
    vTaskDelay((1 / (4 * 60)) / portTICK_PERIOD_MS);    //Frecuencia de muestreo de 4 veces 60 Hz (En realidad se obtiene una fs inferior, pero no tanto)
    lecturaCorr[pos] = analogRead(36);                  //Se lee ADC
    vTaskDelay((1 / (4 * 60)) / portTICK_PERIOD_MS);    //Frecuencia de muestreo de 4 veces 60 Hz (En realidad se obtiene una fs inferior, pero no tanto)

    cuadradoVolt[pos] = (lecturaVolt[pos] - promedioVolt) * (lecturaVolt[pos] - promedioVolt) / (multVolt); //Se calcula el cuadrado de la lectura reescalada
    cuadradoCorr[pos] = (lecturaCorr[pos] - promedioCorr) * (lecturaCorr[pos] - promedioCorr) / (multCorr); //Se calcula el cuadrado de la lectura reescalada
    pos++;
    if (pos == 999) {                           //Si se llega a la última posición se vuelve a la primera
      pos = 0;

      //Calculando promedios nuevamente cada 1000 muestras
      for (int i = 0; i <= 999; i++) {
        promedioVolt += lecturaVolt[i];
        promedioCorr += lecturaCorr[i];
      }
      promedioVolt = promedioVolt / 1000;
      promedioCorr = promedioCorr / 1000;
    }

    suma = 0;
    for (int i = 0; i <= 999; i++) {
      suma += cuadradoVolt[i];                //Se suman todos los valores cuadráticos.
    }
    rmsVolt = sqrt(suma / 1000);              //Calcula valor RMS al sacar raiz de promedio de valores cuadráticos

    suma = 0;
    for (int i = 0; i <= 999; i++) {
      suma += cuadradoCorr[i];                //Se suman todos los valores cuadráticos.
    }
    rmsCorr = sqrt(suma / 1000);              //Calcula valor RMS al sacar raiz de promedio de valores cuadráticos


    //Imprime por serial

    //Serial.print("VN =  ");
    //Serial.println(lecturaVolt[i]);
    //Serial.print("VP =  ");
    //Serial.println(lecturaCorr[i]);
    //vTaskDelay(3000 / portTICK_PERIOD_MS);

    Serial.print("V RMS = ");
    Serial.println(rmsVolt);
    Serial.print("C RMS = ");
    Serial.println(rmsCorr);

  }

}


void printCode (void *analogReadParameter) {
  Serial.println("Print Task created");
  
  while (true) {
    vTaskDelay(3000 / portTICK_PERIOD_MS);

    if (manualControl == 1) {
      Serial.println("Reestablecido");
    }
    else {
      Serial.println("CORTADO");
    }
  }

}


//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************


//Función de configuración inicial del microcontrolador
void setup() {

  vTaskDelay(500 / portTICK_PERIOD_MS);

  Serial.begin(115200);
  Serial.println("Inicializando OMC-WIFI-" + chipID);

  analogReadSetUp();
  acSetUp();


  //Tarea para ejecutar el código de AutoConnect
  xTaskCreatePinnedToCore(
    acCode,                 //Función que se ejecutará en la tarea
    "AutoConnectCode",      //Nombre descriptivo
    10000,                  //Tamaño del Stack para esta tarea
    NULL,                   //Parámetro para guardar la función
    1,                      //Prioridad de la tarea (de 0 a 25)
    NULL,                   //Manejador de tareas
    0);                     //Núcleo en el que se ejecutará


  //Tarea para ejecutar el código de lectura analógica de voltaje y correinte
  //  xTaskCreatePinnedToCore(
  //    analogReadCode,         //Función que se ejecutará en la tarea
  //    "AnalogReadCode",       //Nombre descriptivo
  //    100000,                  //Tamaño del Stack para esta tarea
  //    NULL,                   //Parámetro para guardar la función
  //    1,                      //Prioridad de la tarea (de 0 a 25)
  //    NULL,                   //Manejador de tareas
  //    1);                     //Núcleo en el que se ejecutará


  //Tarea para ejecutar el código de lectura analógica de voltaje y correinte
  xTaskCreatePinnedToCore(
    printCode,              //Función que se ejecutará en la tarea
    "PrintCode",            //Nombre descriptivo
    1024,                   //Tamaño del Stack para esta tarea
    NULL,                   //Parámetro para guardar la función
    1,                      //Prioridad de la tarea (de 0 a 25)
    NULL,                   //Manejador de tareas
    1);                     //Núcleo en el que se ejecutará

  vTaskDelay(500 / portTICK_PERIOD_MS);

  //vTaskStartScheduler();    //Se inicializa el organizador de tareas

}


void loop() {

}
