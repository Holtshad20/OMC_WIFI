uint32_t promedioVolt = 0;    //Promedio de las mediciones analógicas, usado para eliminar nivel DC
uint32_t promedioCorr = 0;

float valoresVolt[1000];      //Arreglo con valores cuadrados que se sumarán (Ventana)
float valoresCorr[1000];      //Arreglo con valores cuadrados que se sumarán (Ventana)
uint16_t pos = 0;             //Posición en la ventana de los valores cuadrados

const float multVolt = 1.23 * 1.23;    //Factor de escala para medir voltaje
const float multCorr = 108 * 108;      //Factor de escala para medir corriente

void setup() {
  Serial.begin(115200);       //Inicializa comunicación serial
  delay(100);

  //Sacando promedio (nivel DC) con 10000 muestras
  for (int i = 0; i <= 9999; i++) {
    promedioVolt += analogRead(39);
    delay(1 / (4 * 60));      //Frecuencia de muestreo de 4 veces 60 Hz
  }
  promedioVolt = promedioVolt / 10000;

  //Sacando promedio (nivel DC) con 10000 muestras
  for (int i = 0; i <= 9999; i++) {
    promedioCorr += analogRead(36);
    delay(1 / (4 * 60));      //Frecuencia de muestreo de 4 veces 60 Hz
  }
  promedioCorr = promedioCorr / 10000;

  delay(100);
}

void loop() {
  int lecturaVolt;            //Lectura actual del ADC Voltaje
  int lecturaCorr;            //Lectura actual del ADC Corriente
  float suma = 0;             //Suma de valores cuadrados 
  float rmsVolt = 0;          //Valor RMS Voltaje
  float rmsCorr = 0;          //Valor RMS Corriente

  lecturaVolt = analogRead(39);   //Se lee ADC
  delay(1 / (4 * 60));            //Frecuencia de muestreo de 4 veces 60 Hz (En realidad se obtiene una fs inferior, pero no tanto)
  lecturaCorr = analogRead(36);   //Se lee ADC
  delay(1 / (4 * 60));            //Frecuencia de muestreo de 4 veces 60 Hz (En realidad se obtiene una fs inferior, pero no tanto)
  
  valoresVolt[pos] = (lecturaVolt - promedioVolt) * (lecturaVolt - promedioVolt) / (multVolt); //Se calcula el cuadrado de la lectura reescalada 
  valoresCorr[pos] = (lecturaCorr - promedioCorr) * (lecturaCorr - promedioCorr) / (multCorr); //Se calcula el cuadrado de la lectura reescalada
  pos++;
  if (pos == 999) {               //Si se llega a la última posición se vuelve a la primera
    pos = 0;
  }
  
  for (int i = 0; i <= 999; i++) {
    suma += valoresVolt[i];       //Se suman todos los valores cuadráticos.
  }
  rmsVolt = sqrt(suma / 1000);    //Calcula valor RMS al sacar raiz de promedio de valores cuadráticos
  
  suma = 0;
  for (int i = 0; i <= 999; i++) {
    suma += valoresCorr[i];       //Se suman todos los valores cuadráticos.
  }
  rmsCorr = sqrt(suma / 1000);    //Calcula valor RMS al sacar raiz de promedio de valores cuadráticos

  //Imprime por serial

  //Serial.print("VN =  ");
  //Serial.println(lecturaVolt);
  //Serial.print("VP =  ");
  //Serial.println(lecturaCorr);
  Serial.print("V RMS = ");
  Serial.println(rmsVolt);
  Serial.print("C RMS = ");
  Serial.println(rmsCorr);
}
