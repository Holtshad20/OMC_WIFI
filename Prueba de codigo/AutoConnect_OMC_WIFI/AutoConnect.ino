#include <WiFi.h>          
#include <WebServer.h>     
#include <AutoConnect.h>
#include <AutoConnectCredential.h>

WebServer         Server;          
AutoConnect       Portal(Server);
AutoConnectConfig config("Tesis_AP","kuma2018");      //Credenciales para acceder al AP

//Función para mostrar la página de inicio al conectar
void rootPage() {
  String content = //"Hello World :D";
    "<html>"
    "<head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-sale=1\">"
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

//Función para borrar las credenciales
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

  config.hostName = "Tesis-AP";
  config.title = "Kumita";
  Portal.config(config);
  Portal.begin();
  
  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
}

void loop() {
    Portal.handleClient();
}
