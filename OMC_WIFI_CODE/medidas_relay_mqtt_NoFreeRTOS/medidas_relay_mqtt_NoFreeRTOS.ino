///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <WiFi.h>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include <AsyncMqttClient.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MQTT
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define WIFI_SSID "dos-desktop"
#define WIFI_PASSWORD "8RlGCKQL"
#define MQTT_HOST IPAddress(192, 168, 7, 217)
#define MQTT_PORT 1883
//#define WIFI_SSID "Thorondor"
//#define WIFI_PASSWORD "n140862m041260n200689m170697gizmokingdom"
//#define MQTT_HOST IPAddress(192, 168, 0, 50)
//#define MQTT_PORT 1883

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;
TimerHandle_t publishTimer;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Medición V/C
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t promedioVolt = 0;    //Promedio de las mediciones analógicas, usado para eliminar nivel DC
uint32_t promedioCorr = 0;

float lecturaVolt[5000];      //Arreglo con valores del ADC para promedio (Ventana)
float lecturaCorr[5000];      //Arreglo con valores del ADC para promedio (Ventana)
float cuadradoVolt[5000];     //Arreglo con valores cuadrados que se sumarán (Ventana)
float cuadradoCorr[5000];     //Arreglo con valores cuadrados que se sumarán (Ventana)
int pos = 0;             //Posición en la ventana de los valores cuadrados

const float multVolt = 1.28 * 1.28;    //Factor de escala para medir voltaje 1.23 1.21
const float multCorr = 108 * 108;      //Factor de escala para medir corriente 108

float rmsVolt = 0;          //Valor RMS Voltaje
float rmsCorr = 0;          //Valor RMS Corriente
float rmsV = 0;          //Valor RMS Voltaje
float rmsC = 0;          //Valor RMS Corriente
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Relay
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int temporal = 0;

uint8_t voltSup = 135;  // Máximo voltaje permitido
uint8_t voltInf = 100;  // Mínimo voltaje permitido
uint8_t corrSup = 15;   // Máxima corriente permitida

uint8_t tiempoRecuperacion = 10; // Tiempo requerido permitir paso de corriente luego de una falla o un reinicio (segundos)

boolean relay = LOW;      // Estado del relay (software)
boolean pasoElTiempo = 0; // Indica si transcurrió el tiempo de recuperación 

boolean controlGlobalRelay = 1; // Control Global del Relé
                                // Si controlGlobalRelay = 0 entonces estamos forzando a que se mantenga apagado sin importar el voltaje o la corriente.
                                // Si controlGlobalRelay = 1 entonces estamos trabajando de manera normal con los márgenes de voltaje y corriente normales.

TimerHandle_t timerRecuperacion;  // Temporizador, se desborda y ejecuta pasoTiempoRecuperacion() luego de que trascurran "tiempoRecuperacion" segundos
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////




// MQTT
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void connectToWifi() {
  Serial.println();
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println();
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
    Serial.println();
    Serial.printf("[WiFi-event] event: %d\n", event);
    switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        connectToMqtt();
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
        xTimerStart(wifiReconnectTimer, 0);
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
  rmsV = rmsVolt;
  rmsC = 0;
  mqttClient.publish("esp32/volt", 0, true, String(rmsV).c_str());
  //mqttClient.publish("esp32/corr", 0, true, String(rmsCorr).c_str());
  mqttClient.publish("esp32/corr", 0, true, "0");
  Serial.println();
  Serial.print("V RMS = ");
  Serial.println(rmsV);
  Serial.print("C RMS = ");
  Serial.println(rmsC);
  xTimerReset(publishTimer, 0);
}

void setupMQTT() {
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
  publishTimer = xTimerCreate("publishTimer", pdMS_TO_TICKS(1000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(publicarValores));
  xTimerStop(publishTimer, 0);

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWifi();
  xTimerReset(publishTimer, 0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Medición V/C
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void mediciones(){
  float suma = 0;             //Suma de valores cuadrados

  lecturaVolt[pos] = analogRead(39);   //Se lee ADC
  delay(1 / (4 * 60));                 //Frecuencia de muestreo de 4 veces 60 Hz (En realidad se obtiene una fs inferior, pero no tanto)
  lecturaCorr[pos] = analogRead(36);   //Se lee ADC
  delay(1 / (4 * 60));                 //Frecuencia de muestreo de 4 veces 60 Hz (En realidad se obtiene una fs inferior, pero no tanto)

  cuadradoVolt[pos] = (lecturaVolt[pos] - promedioVolt) * (lecturaVolt[pos] - promedioVolt) / (multVolt); //Se calcula el cuadrado de la lectura reescalada
  cuadradoCorr[pos] = (lecturaCorr[pos] - promedioCorr) * (lecturaCorr[pos] - promedioCorr) / (multCorr); //Se calcula el cuadrado de la lectura reescalada
  pos++;
  if (pos == 4999) {               //Si se llega a la última posición se vuelve a la primera
    pos = 0;
    //Sacando promedios nuevamente cada 1000 muestras
    for (int i = 0; i <= 4999; i++) {
      promedioVolt += lecturaVolt[i];
      promedioCorr += lecturaCorr[i];
    }
    promedioVolt = promedioVolt / 5000;
    promedioCorr = promedioCorr / 5000;
  }

  suma = 0;
  for (int i = 0; i <= 4999; i++) {
    suma += cuadradoVolt[i];       //Se suman todos los valores cuadráticos.
  }
  rmsVolt = sqrt(suma / 5000);    //Calcula valor RMS al sacar raiz de promedio de valores cuadráticos

  suma = 0;
  for (int i = 0; i <= 4999; i++) {
    suma += cuadradoCorr[i];       //Se suman todos los valores cuadráticos.
  }
  //rmsCorr = sqrt(suma / 5000);    //Calcula valor RMS al sacar raiz de promedio de valores cuadráticos
  rmsCorr = 0;
}

void setupMedicion() {
  //Sacando promedio inicial (nivel DC) con 10000 muestras
  for (int i = 0; i <= 4999; i++) {
    promedioVolt += analogRead(39);
    delay(1 / (4 * 60));      //Frecuencia de muestreo de 4 veces 60 Hz
  }
  promedioVolt = promedioVolt / 5000;

  //Sacando promedio inicial (nivel DC) con 10000 muestras
  for (int i = 0; i <= 4999; i++) {
    promedioCorr += analogRead(36);
    delay(1 / (4 * 60));      //Frecuencia de muestreo de 4 veces 60 Hz
  }
  promedioCorr = promedioCorr / 5000;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Relay
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void pasoTiempoRecuperacion() {   // Se ejecuta luego de que trascurran "tiempoRecuperacion" segundos
  pasoElTiempo = 1;               // La variable indica que transcurrieron los segundos
}

void controlRelay() {
    if (controlGlobalRelay == 1) {  // Si el relay está en modo automático entonces ejecuta el control automático:
      if ( (rmsV >= voltInf) && (rmsV <= voltSup) && (rmsC <= corrSup) && (relay == LOW) ) { // Una vez esté la alimenteciión en los niveles correctos:
        if (xTimerIsTimerActive(timerRecuperacion) == pdFALSE ){   
          xTimerReset(timerRecuperacion, 0);  // Se inicia el temporizador solo si no estaba activo antes

          Serial.println();
          Serial.println("*** Voltaje/corriente dentro de rango *** ");
          Serial.print("Temporizador inicializado - Esperando ");
          Serial.print(tiempoRecuperacion);
          Serial.println(" segundos para restaurar alimentación");
        }
       if (pasoElTiempo == 1) {  // Si pasaron los segundos necesarios entonces
          relay = HIGH;           // Enciende el relay
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
          delay(500);
          xTimerReset(publishTimer, 0);
          
        }
      }
      if ( (rmsV < voltInf) || (rmsV > voltSup) || (rmsC > corrSup) ) {  // Si la alimentación está mal entonces:
        if (relay == HIGH) {  // Solo si el relay estaba encendido antes
          relay = LOW;        // Apaga el relay
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
        else {  // Si en cambio el relay estaba apagado entonces significa que se interrumpio la recuperación
          if (xTimerIsTimerActive(timerRecuperacion) != pdFALSE ){  // Pero solo se muestra el mensaje una sola vez
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
    else if (relay == HIGH) { // Si el relé se mandó a apagar de manera manual:
      relay = LOW;            // Se apaga el relé
      digitalWrite(23, LOW);
    
      xTimerReset(timerRecuperacion, 0); // Se detiene el temporizador
      xTimerStop(timerRecuperacion, 0);
      pasoElTiempo = 0;
    
      mqttClient.publish("esp32/estadoRelay", 0, true, "OFF");

      Serial.println();
      Serial.println("*** Se ha forzado corte de alimentación ***");
      Serial.println("*** Relay apagado ***");
    }
}

void setupRelay() {
  pinMode(23, OUTPUT);        // El Relé es una salida
  digitalWrite(23, LOW);      // Al reiniciar el dispositivo el relé está apagado
  relay = LOW;                // Variable del estado relay (para ser leída por software)

  pasoElTiempo = 0;           // La variable indica que transcurrieron los segundos necesarios para restaurar corriente
  
  // Creación del temporizador, se desborda y ejecuta pasoTiempoRecuperacion() luego de que trascurran "tiempoRecuperacion" segundos
  timerRecuperacion = xTimerCreate("TimerDeRecuperacion", pdMS_TO_TICKS(tiempoRecuperacion * 1000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(pasoTiempoRecuperacion));
  xTimerStop(timerRecuperacion, 0); // Mantiene apagado el temporizador hasta que se vuelva a iniciar
  
  mqttClient.publish("esp32/estadoRelay", 0, true, "OFF");

  Serial.println();
  Serial.println("Dispositivo inicializado");
  Serial.println("*** Relay apagado ***");

  delay(100);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(115200);

  setupMQTT();
  setupMedicion(); 
  setupRelay();
}

void loop() {
  mediciones();
  controlRelay();
}
