#include <WiFi.h>          
#include <WebServer.h>     
#include <AutoConnect.h>
#include <AutoConnectCredential.h>
#include <EEPROM.h>

WebServer         Server;          
AutoConnect       Portal(Server);
AutoConnectConfig config("OMC_WIFI","12345678");      //Credenciales para acceder al AP

//Declaración de elementos AutoConnect para la página web de Configuración del AP
ACText(caption01, "Desde este portal podrá cambiar el hostname, el SSID y la contraseña del punto de acceso OMC_WIFI", "text-align:justify;font-family:serif;color:#000000;");
ACText(header01, "<h2>Cambiar Hostname</h2>", "text-align:center;color:2f4f4f;");
ACInput(hostname, "", "Nuevo Hostname", "", "Introduzca el nuevo Hostname");
ACText(cond01, "<p>La <b>longitud del Hostname</b> no debe exceder los 64 caracteres, solo puede contener <b>caracteres alfanuméricos</b> y <b>guiones (-)</b> y no puede culminar en un guión (-)</p>", "text-align:justify");
ACText(rec01, "<p><b>RECOMENDACIÓN:</b> emplear el hostname <u>OMC_WIFI_nombre</u>. Por ejemplo: OMC_WIFI_laptop o OMC_WIFI_minero</p>")
ACText(header02, "<h2>Cambiar Credenciales</h2>", "text-align:center;color:2f4f4f;");
ACInput(ssid, "", "Nueva SSID", "", "Introduzca su nuevo SSID");
ACInput(pass1, "", "Nueva Clave", "", "Introduzca su nueva clave");
ACInput(pass2, "", "Confirme Clave", "", "Introduzca su clave de nuevo");
ACText(cond02, "<p>La <b>longitud del SSID</b> no debe exceder los 32 caracteres y ,<b>longitud de la clave</b> debe tener entre 8 y 63 caracateres</p>", "text-align:justify");
ACSubmit(change0, "Guardar cambios", "/post_config");
ACSubmit(back0, "Volver", "/_ac");
ACText(note01, "<p><b>NOTA:</b> si no desea cambiar algún parámetro, deje la casilla en blanco</p>", "text-align:justify");

//Declaración de elementos AutoConnect para la página web post-configuración del AP

ACSubmit(reset, "Reiniciar", "/_ac#rdlg");
ACSubmit(back1, "Volver al menú", "/_ac");


//Declaración de la página web para la página web de Configuración del AP
AutoConnectAux ap_config("/ap_config", "Configuración de AP", true,{
  caption01,
  header01,
  hostname,
  cond01,
  header02,
  ssid,
  pass1,
  pass2,
  cond02,
  change0,
  back0,
  note01,
});

//Declaración de la página web para la página web post-configuración del AP
AutoConnectAux post_config("/post_config", "Configuración de AP", false,{
  reset,
  back1,
});

//Datos iniciales en la página de configuración del AP
//String onConfig(AutoConnectAux& aux, PageArgument& args){
  //aux["ssid"].as<AutoConnectInput>().value = "";
  //aux["pass1"].as<AutoConnectInput>().value = "";
  //aux["pass2"].as<AutoConnectInput>().value = "";
  //return String();
//}

//String onPostConfig(AutoConnect& aux, PageArgument& args){
  
  
//}

//Función para mostrar la página de inicio al conectar
void rootPage() {
  String content = 
    "<html>"
    "<head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "<title>Tesis_AP</title>"
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


//Función para cargar las credenciales de la flash

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  deleteAllCredentials();
  
  //Se borra la configuración Wi-Fi
  WiFi.disconnect(true, true);

  config.hostName = "OMC_WIFI";
  config.title = "OMC_WIFI Access Point";
  config.homeUri = "/_ac",
  Portal.config(config);
  Portal.join({ap_config, post_config});
  Portal.begin();
  
  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
}

void loop() {
    Portal.handleClient();
}
