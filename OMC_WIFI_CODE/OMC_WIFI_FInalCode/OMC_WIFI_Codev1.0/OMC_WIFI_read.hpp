#ifndef OMC_WIFI_READ_HPP
#define OMC_WIFI_READ_HPP

#include "OMC_WIFI_GlobalVariables.hpp"



//***************************************************************************************************************************************************************
//*************************************************    VARIABLES, CONSTANTES Y ARREGLOS PARA LECTURA ANALÓGICA     **********************************************
//***************************************************************************************************************************************************************

extern uint32_t promedioVolt;            // Promedio de las mediciones analógicas, usado para eliminar nivel DC
extern uint32_t promedioCorr;

extern float lecturaVolt[];              // Arreglo con valores del ADC para promedio (Ventana)
extern float lecturaCorr[];              // Arreglo con valores del ADC para promedio (Ventana)
extern float cuadradoVolt[];             // Arreglo con valores cuadrados que se sumarán (Ventana)
extern float cuadradoCorr[];             // Arreglo con valores cuadrados que se sumarán (Ventana)
extern int   pos;                            // Posición en la ventana de los valores cuadrados

const float multVolt = 2.8 * 2.8;     // Factor de escala para medir voltaje 2.75
const float multCorr = 108 * 108;     // Factor de escala para medir corriente 108

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



//***************************************************************************************************************************************************************
//*************************************************    VARIABLES Y CONSTANTES PARA CONTROLAR EL RELAY     *******************************************************
//***************************************************************************************************************************************************************

const int relayPin = 32;      //Pin que se empleará para el relay

extern boolean pasoElTiempo;   // Indica si transcurrió el tiempo de recuperación

//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************



void readSetUp();

void relaySetUp ();

void pasoTiempoRecuperacion();

void readCode (void *readParameter);


#endif
