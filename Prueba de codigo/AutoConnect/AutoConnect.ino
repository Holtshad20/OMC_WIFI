#include <WiFi.h>          
#include <WebServer.h>     
#include <AutoConnect.h>
#include <AutoConnectCredential.h>

WebServer         Server;          
AutoConnect       Portal(Server);
AutoConnectConfig config("Tesis_AP","kuma2018");      //Credenciales para acceder al AP

//Declaración de elementos AutoConnect para la página web de Configuración del AP
ACText(caption, "Desde este portal podrá cambiar el nombre (SSID) y la contraseña del punto de acceso", "text-align:justify;font-family:serif;color:#000000;");
ACText(header, "<h2>Cambiar Credenciales</h2>", "text-align:center;color:2f4f4f;");
ACInput(ssid, "", "Nueva SSID", "", "Introduzca su nuevo SSID");
ACInput(pass1, "", "Nueva Clave", "", "Introduzca su nueva clave");
ACInput(pass2, "", "Confirme Clave", "", "Introduzca su clave de nuevo");
ACSubmit(change, "Guardar cambios", "/_ac");
ACSubmit(back, "Volver", "/_ac");
ACText(note, "<p><b>NOTA:</b> si no desea cambiar algún parámetro, deje la casilla en blanco</p>", "text-align:justify");

//Declaración de elementos AutoConnect para la página web post-configuración del AP


//Declaración de la página web para la página web de Configuración del AP
AutoConnectAux ap_config("/ap_config", "Configuración de AP", true,{
  caption,
  header,
  ssid,
  pass1,
  pass2,
  change,
  back,
  note,
});

//Datos iniciales en la página de configuración del AP
String onConfig(AutoConnectAux& aux, PageArgument& args){
  aux["ssid"].as<AutoConnectInput>().value = "";
  aux["pass1"].as<AutoConnectInput>().value = "";
  aux["pass2"].as<AutoConnectInput>().value = "";
  return String();
}

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

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  deleteAllCredentials();
  
  //Se borra la configuración Wi-Fi
  WiFi.disconnect(true, true);

  config.hostName = "esp32_ap";
  config.title = "ESP32 Access Point";
  Portal.config(config);
  Portal.join({ap_config});
  Portal.begin();
  
  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
}

void loop() {
    Portal.handleClient();
}
