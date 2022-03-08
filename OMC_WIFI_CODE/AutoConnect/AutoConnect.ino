#include <WiFi.h>          
#include <WebServer.h>     
#include <AutoConnect.h>
#include <AutoConnectCredential.h>
#include <Preferences.h>

WebServer         Server;          
AutoConnect       Portal(Server);
AutoConnectConfig config;      //Credenciales para acceder al AP
Preferences       storage;                        //Espacio en memoria para guardar las credenciales


//Declaración de elementos AutoConnect para la página web de Configuración del AP
ACText(caption01, "Desde este portal podrá cambiar el SSID y la contraseña del punto de acceso OMC_WIFI", "text-align:justify;font-family:serif;color:#000000;");
ACText(header01, "<h2>Cambio de SSID</h2>", "text-align:center;color:2f4f4f;");
ACText(cond01, "<p>La <b>longitud del SSID</b> debe tener entre 2 y 32 caracteres (solo acepta el guión como caracter especial).</p>", "text-align:justify");
ACInput(ssid, "", "Nueva SSID", "^[a-zA-ZñÑ\\d\\-]{1,31}[a-zA-ZñÑ\\d\\s]$", "Introduzca su nuevo SSID");
ACSubmit(change01, "Guardar SSID", "/ap_ssid");
ACText(header02, "<h2>Cambio de Clave</h2>", "text-align:center;color:2f4f4f;");
ACText(cond02, "<p>La <b>longitud de la clave</b> debe tener entre 8 y 16 caracateres</p>", "text-align:justify");
ACInput(pass1, "", "Nueva Clave", "^.{8,16}$", "Introduzca su nueva clave");
ACInput(pass2, "", "Confirme Clave", "^.{8,16}$", "Introduzca su clave de nuevo");
ACSubmit(change02, "Guardar Clave", "/ap_pass");
ACText(note01, "<p><b>NOTA:</b> si no desea cambiar algún parámetro, deje la casilla en blanco</p>", "text-align:justify");
ACSubmit(back, "Volver al menú", "/_ac");

//Declaración de elementos AutoConnect para la página web para guardado de SSID
ACText(txt11, "", "text-align:center");
ACText(note11, "<p><b>ADVERTENCIA:</b> para aplicar los cambios el dispositivo debe ser <b>reiniciado</b>, por lo que el dispositivo conectado al OMC-WIFI será desconectado del suministro eléctrico momentáneamente.</p>", "text-align:justify");
ACSubmit(reset, "Reiniciar equipo", "/_ac#rdlg");

//Declaración de elementos AutoConnect para la página web para guardado de Contraseña
ACText(txt21, "", "text-align:center");


//Declaración de la página web para la página web de Configuración del AP
AutoConnectAux ap_credentials("/ap_credentials", "Cambiar Credenciales de AP", true,{
  
  caption01,
  header01,
  cond01,
  ssid,
  change01,
  header02,
  cond02,
  pass1,
  pass2,
  change02,
  note01,
  back,

});

//Declaración de la página web para la página web post-configuración del AP
AutoConnectAux ap_ssid("/ap_ssid", "Cambiar Credenciales de AP", false,{
  
  header01,
  txt11,
  note11,
  reset,
  back,

});


//Declaración de la página web para la página web post-configuración del AP
AutoConnectAux ap_pass("/ap_pass", "Cambiar Credenciales de AP", false,{
  
  header02,
  txt21,
  note11,
  reset,
  back,

});

//Datos iniciales en la página de configuración del AP
String onConfig(AutoConnectAux& aux, PageArgument& args){
  
  aux["ssid"].as<AutoConnectInput>().value = "";
  aux["pass1"].as<AutoConnectInput>().value = "";
  aux["pass2"].as<AutoConnectInput>().value = "";
  
  return String();

}

String onChangeSSID(AutoConnectAux& aux, PageArgument& args){
  AutoConnectInput& ssid = ap_credentials["ssid"].as<AutoConnectInput>();      //Se guarda el elemento AutoConnectInput denominado ssid de la página web ap_credentials
  
  if (args.arg("ssid") == ""){
    aux["txt11"].as<AutoConnectText>().value = "NO se detectó ningún cambio en el <b>SSID</b>.\n";
  }
  else if (ssid.isValid()){
    storage.begin("credentials", false);    //Se apertura el espacio de las credenciales para leer y escribir (false)
    storage.putString("ssid", args.arg("ssid"));    //Se guarda el SSID
    Serial.println("El <b>SSID</b> ha cambiado a:\n" + storage.getString("ssid",""));
    storage.end();
          
    aux["txt11"].as<AutoConnectText>().value = "El <b>SSID</b> ha cambiado a:\n" + args.arg("ssid");
  }

  else{
    aux["txt11"].as<AutoConnectText>().value = "El <b>SSID</b> introducido NO cumple las condiciones.\nPor favor intente de nuevo.\n";
  }
  
  return String();
  
  }

//Página para validar el cambio de Contraseña
String onChangePass(AutoConnectAux& aux, PageArgument& args){
  //Verificando que las claves introducidas coinciden
  if (args.arg("pass1") == args.arg("pass2")){ 
    AutoConnectInput& pass = ap_credentials["pass1"].as<AutoConnectInput>();      //Se guarda el elemento AutoConnectInput denominado pass1 de la página web ap_credentials   
    //Verificando que la clave introducida cumple con las condiciones
    if (args.arg("pass1") == "")
      aux["txt21"].as<AutoConnectText>().value = "NO se detectó ningún cambio en la <b>clave</b>.\n";
    else if (pass.isValid())
      aux["txt21"].as<AutoConnectText>().value = "La <b>clave</b> ha sido cambiada exitosamente.\n";
    else
      aux["txt21"].as<AutoConnectText>().value = "La <b>clave</b> introducida NO cumple las condiciones.\nPor favor intente de nuevo.\n";
    }  
  else{
    aux["txt21"].as<AutoConnectText>().value = "Las <b>claves</b> introducidas NO coinciden.\nPor favor intente de nuevo.\n";
  }
  
  return String();
  
  }

//Función para mostrar la página de inicio al conectar
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
void deleteAllCredentials(void) {
  //Se guardan las credenciales que se encuentran en la flash
  AutoConnectCredential credential;
  station_config_t      config;
  uint8_t qty           = credential.entries();       //Cantidad de credenciales guardadas

  Serial.println("Borrando credenciales");
  if (qty)
    Serial.printf("Hay %d credenciales guardadas.\n", qty);
  else {
    Serial.println("No hay credenciales guardadas.");
    return;
  }
  
  //Ciclo para borrar las credenciales
   while (qty--) {
    credential.load((int8_t)0, &config);                                        //Se cargan las credenciales de la flash
    if (credential.del((const char*)&config.ssid[0]))                           //Se borran las credenciales cargadas anteriormente
      Serial.printf("%s fue borrada.\n", (const char*)config.ssid);
    else
      Serial.printf("%s NO pudo ser borrada.\n", (const char*)config.ssid);     
  }                                                
}


//Función 
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  

  //Se borra la configuración Wi-Fi
  deleteAllCredentials();
  WiFi.disconnect(true, true);

  //Se hace la configuración inicial del AP
  storage.begin("credentials", true);
    
  //Configuración Hostname y SSID
  if (storage.getString("ssid","") != ""){      //Si hay un SSID guardado en memoria, se cambia
    config.hostName = storage.getString("ssid","");
    config.apid     = storage.getString("ssid","");    
  }
  else {                                        //Si NO hay un SSID guardado en memoria, se coloca el default
    config.hostName = "OMC-WIFI";
    config.apid     = "OMC-WIFI";                   
  }

  //Configuración Contraseña
  if (storage.getString("pass","") != ""){
    config.psk = storage.getString("pass","");
  }
  else {
    config.psk = "12345678";
  }
  

  storage.end();
  config.title = "OMC-WIFI Access Point";
  config.homeUri = "/_ac",
  Portal.config(config);
  Portal.join({ap_credentials, ap_ssid, ap_pass});
  Portal.on("/ap_credentials", onConfig);
  Portal.on("/ap_ssid", onChangeSSID);
  Portal.on("/ap_pass", onChangePass);
  Portal.begin();
  
  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
}

void loop() {
    Portal.handleClient();
}