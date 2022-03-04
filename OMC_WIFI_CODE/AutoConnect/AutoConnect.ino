#include <WiFi.h>          
#include <WebServer.h>     
#include <AutoConnect.h>
#include <AutoConnectCredential.h>

WebServer         Server;          
AutoConnect       Portal(Server);
AutoConnectConfig config("OMC_WIFI","12345678");      //Credenciales para acceder al AP
AutoConnectAux    ap_config;


////Declaración de elementos AutoConnect para la página web de Configuración del AP
//ACText(caption01, "Desde este portal podrá cambiar el hostname, el SSID y la contraseña del punto de acceso OMC_WIFI", "text-align:justify;font-family:serif;color:#000000;");
//ACText(header01, "<h2>Cambiar Hostname</h2>", "text-align:center;color:2f4f4f;");
//ACInput(hostname, "", "Nuevo Hostname", "^[a-zA-ZñÑ\\d\\-]{1,31}[a-zA-ZñÑ\\d\\s]$", "Introduzca el nuevo Hostname");
//ACText(cond01, "<p>La <b>longitud del Hostname</b> no debe exceder los 64 caracteres, solo puede contener <b>caracteres alfanuméricos</b> y <b>guiones (-)</b> y no puede culminar en un guión (-)</p>", "text-align:justify");
//ACText(rec01, "<p><b>RECOMENDACIÓN:</b> emplear el hostname <u>OMC-WIFI-nombre</u>. Por ejemplo: OMC-WIFI-laptop o OMC-WIFI-minero</p>");
//ACText(header02, "<h2>Cambiar Credenciales</h2>", "text-align:center;color:2f4f4f;");
//ACInput(ssid, "", "Nueva SSID", "^[a-zA-ZñÑ\\d\\s\\-]{2,31}$", "Introduzca su nuevo SSID");
//ACInput(pass1, "", "Nueva Clave", "^.{8,16}$", "Introduzca su nueva clave");
//ACInput(pass2, "", "Confirme Clave", "^.{8,16}$", "Introduzca su clave de nuevo");
//ACText(cond02, "<p>La <b>longitud del SSID</b> debe tener entre 2 y 32 caracteres (solo acepta el guión como caracter especial) y <b>longitud de la clave</b> debe tener entre 8 y 16 caracateres</p>", "text-align:justify");
//ACSubmit(change0, "Guardar cambios", "/post_config");
ACSubmit(back0, "Volver al menú", "/_ac");
//ACText(note01, "<p><b>NOTA:</b> si no desea cambiar algún parámetro, deje la casilla en blanco</p>", "text-align:justify");

//Declaración de elementos AutoConnect para la página web post-configuración del AP
ACText(header11, "<h2>Hostname</h2>", "text-align:center;color:2f4f4f;");
ACText(ver11, "", "text-align:center");
ACText(header12, "<h2>SSID</h2>", "text-align:center;color:2f4f4f;");
ACText(ver12, "", "text-align:center");
ACText(header13, "<h2>Contraseña</h2>", "text-align:center;color:2f4f4f;");
ACText(ver13, "", "text-align:center");
ACSubmit(reset, "Reiniciar", "/_ac#rdlg");
//ACSubmit(back1, "Volver al menú", "/_ac");



////Declaración de la página web para la página web de Configuración del AP
//AutoConnectAux ap_config("/ap_config", "Configuración de AP", true,{
//  
//  caption01,
//  header01,
//  hostname,
//  cond01,
//  header02,
//  ssid,
//  pass1,
//  pass2,
//  cond02,
//  note01,
//  change0,
//  back0,
//
//});

//Declaración de la página web para la página web post-configuración del AP
AutoConnectAux post_config("/post_config", "Configuración de AP", false,{
  
  header11,
  ver11,
  header12,
  ver12,
  header13,
  ver13,
  reset,
  back0,

});

//Datos iniciales en la página de configuración del AP
String onConfig(AutoConnectAux& aux, PageArgument& args){
  
  aux["hostname"].as<AutoConnectInput>().value = "";
  aux["ssid"].as<AutoConnectInput>().value = "";
  aux["pass1"].as<AutoConnectInput>().value = "";
  aux["pass2"].as<AutoConnectInput>().value = "";
  
  return String();

}

String onPostConfig(AutoConnectAux& aux, PageArgument& args){
//VERIFICACIÓN GLOBAL
  if (!ap_config.isValid())                         //Se verifica que la regexp sea válida
    aux["ver11"].as<AutoConnectText>().value = "Alguno de los datos introducidos NO cumple las condiciones establecidas.\nPor favor intente de nuevo.\n";
  else
    if (args.arg("ssid") == ""){
      aux["ver12"].as<AutoConnectText>().value = "NO se detectó ningún cambio.\n";
    }
    else{
      aux["ver12"].as<AutoConnectText>().value = "Cambiado a: " + args.arg("ssid") + ".\n";
    }
    
    if (args.arg("ssid") == ""){
      aux["ver12"].as<AutoConnectText>().value = "NO se detectó ningún cambio.\n";
    }
    else{
      aux["ver12"].as<AutoConnectText>().value = "Cambiado a: " + args.arg("ssid") + ".\n";
    }


//VERIFICACIONES INDIVIDUALES


//  //Verificando que el hostname introducido cumple con las condiciones
//  AutoConnectInput& input = ap_config["hostname"].as<AutoConnectInput>();      //Se guarda el elemento AutoConnectInput denominado hostname de la página web ap_config
//
//  if (args.arg("hostname") == "")
//    aux["ver11"].as<AutoConnectText>().value = "NO se detectó ningún cambio.\n";
//  else if (input.isValid())
//    aux["ver11"].as<AutoConnectText>().value = "Cambiado a: " + args.arg("hostname") + ".\n";
//  else
//    aux["ver11"].as<AutoConnectText>().value = "El hostname introducido NO cumple las condiciones.\nIntente de nuevo.\n";
//
//  //Verificando que el SSID introducido cumple con las condiciones
//  input = ap_config["ssid"].as<AutoConnectInput>();      //Se guarda el elemento AutoConnectInput denominado ssid de la página web ap_config
//
//  if (args.arg("ssid") == "")
//    aux["ver12"].as<AutoConnectText>().value = "NO se detectó ningún cambio.\n";
//  else if (input.isValid())
//    aux["ver12"].as<AutoConnectText>().value = "Cambiado a: " + args.arg("ssid") + ".\n";
//  else
//    aux["ver12"].as<AutoConnectText>().value = "El ssid introducido NO cumple las condiciones.\nIntente de nuevo.\n";

//  //Verificando que las claves introducidas coinciden
//  if (args.arg("pass1") == args.arg("pass2")){
//    //Verificando que la clave introducida cumple con las condiciones
//    input = ap_config["pass1"].as<AutoConnectInput>();      //Se guarda el elemento AutoConnectInput denominado pass1 (contraseña) de la página web ap_config
//
//    if (args.arg("pass1") == "")
//      aux["ver13"].as<AutoConnectText>().value = "NO se detectó ningún cambio.\n";
//    else if (input.isValid())
//      aux["ver13"].as<AutoConnectText>().value = "La contraseña ha sido cambiada exitosamente.\n";
//    else
//      aux["ver13"].as<AutoConnectText>().value = "El ssid introducido NO cumple las condiciones.\nIntente de nuevo.\n";
//    }  
//  else
//    aux["ver13"].as<AutoConnectText>().value = "Las contraseñas introducidas NO coinciden.\nIntente de nuevo.\n";
//  
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


//Función para cargar las credenciales de la flash

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  deleteAllCredentials();
  
  //Se borra la configuración Wi-Fi
  WiFi.disconnect(true, true);

  
  //ap_config.load(page);
  
  config.hostName = "OMC-WIFI";
  config.title = "OMC-WIFI Access Point";
  config.homeUri = "/_ac",
  Portal.config(config);
  Portal.join({ap_config, post_config});
  Portal.on("/ap_config", onConfig);
  Portal.on("/post_config", onPostConfig);
  Portal.begin();
  
  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
}

void loop() {
    Portal.handleClient();
}
