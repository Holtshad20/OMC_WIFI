#ifndef OMC_WIFI_LEDS_HPP
#define OMC_WIFI_LEDS_HPP

#include "OMC_WIFI_GlobalVariables.hpp"
#include "esp_wifi.h"

const int greenLed     = 2;       //Pin que se empleará para el Led Verde
const int greenChannel = 0;       //Canal PWM empleado para el Led Verde

const int redLed       = 23;      //Pin que se empleará para el Led Rojo
const int redChannel   = 1;       //Canal PWM empleado para el Led Rojo

void ledSetUp();
void greenLedTask(void *greenLedParameter);
void redLedTask(void *redLedParameter);

#endif
