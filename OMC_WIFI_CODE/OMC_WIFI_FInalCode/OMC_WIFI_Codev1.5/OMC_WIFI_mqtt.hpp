#ifndef OMC_WIFI_MQTT_HPP
#define OMC_WIFI_MQTT_HPP

#include "OMC_WIFI_GlobalVariables.hpp"
#include <PZEM004Tv30.h>


extern void connectToMqtt();

void WiFiEvent(WiFiEvent_t event);

void onMqttConnect(bool sessionPresent);

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);

void onMqttSubscribe(uint16_t packetId, uint8_t qos);

void onMqttUnsubscribe(uint16_t packetId);

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);

void onMqttPublish(uint16_t packetId);

void publicarValores();

void mqttSetUp();

#endif
