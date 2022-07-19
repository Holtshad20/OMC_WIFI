#ifndef OMC_WIFI_TOUCHSENSOR_HPP
#define OMC_WIFI_TOUCHSENSOR_HPP


#include "OMC_WIFI_GlobalVariables.hpp"


//***************************************************************************************************************************************************************
//*******************************************************    CONSTANTES Y CONSTRUCTORES PARA TOUCH SENSOR     ***************************************************
//***************************************************************************************************************************************************************

#define TOUCH_SENSOR  T9                    //Se define cuál es el pin a utilizar como touch sensor

const int THRESHOLD            = 40;        //Se define cuál es el valor para activar la interrrupción del Touch Sensor
const int AP_MODE_THRESHOLD    = 1000;      //Se define cuál es el valor para activar o desactivar el modo AP
const int REBOOT_THRESHOLD     = 5000;      //Se define cuál es el valor para reiniciar el equipo
const int CRED_RESET_THRESHOLD = 10000;     //Se define cuál es el valor para reiniciar credenciales y luego el equipo

extern TaskHandle_t  xTouchHandle;          //Manejador de tareas de la rutina del Touch Sensor

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************

void touchInterrupt();

void touchSetUp();

void touchTask(void *touchParameter);

#endif
