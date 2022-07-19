#ifndef OMC_WIFI_LEDS_HPP
#define OMC_WIFI_LEDS_HPP

#include "OMC_WIFI_GlobalVariables.hpp"

const int greenLed     = 23;       //Pin que se empleará para el Led Verde
const int greenChannel = 0;       //Canal PWM empleado para el Led Verde

const int redLed       = 22;      //Pin que se empleará para el Led Rojo
const int redChannel   = 1;       //Canal PWM empleado para el Led Rojo

void ledSetUp();
void greenLedTask(void *greenLedParameter);
void redLedTask(void *redLedParameter);

#endif
