uint32_t promedioVolt = 0;    //Promedio de las mediciones analógicas, usado para eliminar nivel DC
uint32_t promedioCorr = 0;

float lecturaVolt[1000];      //Arreglo con valores del ADC para promedio (Ventana)
float lecturaCorr[1000];      //Arreglo con valores del ADC para promedio (Ventana)
float cuadradoVolt[1000];     //Arreglo con valores cuadrados que se sumarán (Ventana)
float cuadradoCorr[1000];     //Arreglo con valores cuadrados que se sumarán (Ventana)
uint16_t pos = 0;             //Posición en la ventana de los valores cuadrados

const float multVolt = 2.70*2.70;    //Factor de escala para medir voltaje ultimo: 1.23 2.46
const float multCorr = 108 * 108;      //Factor de escala para medir corriente 108

void setup() {
  Serial.begin(115200);       //Inicializa comunicación serial
  delay(100);

  //Sacando promedio (nivel DC) con 1000 muestras
  for (int i = 0; i <= 999; i++) {
    promedioVolt += analogRead(39);
    delay(1 / (4 * 60));      //Frecuencia de muestreo de 4 veces 60 Hz
  }
  promedioVolt = promedioVolt / 1000;

  //Sacando promedio (nivel DC) con 1000 muestras
  for (int i = 0; i <= 999; i++) {
    promedioCorr += analogRead(36);
    delay(1 / (4 * 60));      //Frecuencia de muestreo de 4 veces 60 Hz
  }
  promedioCorr = promedioCorr / 1000;


  pinMode(23, OUTPUT);
  delay(100);
}

void loop() {
  float suma = 0;             //Suma de valores cuadrados
  float rmsVolt = 0;          //Valor RMS Voltaje
  float rmsCorr = 0;          //Valor RMS Corriente

  lecturaVolt[pos] = analogRead(39);   //Se lee ADC
  delay(1 / (4 * 60));                 //Frecuencia de muestreo de 4 veces 60 Hz (En realidad se obtiene una fs inferior, pero no tanto)
  lecturaCorr[pos] = analogRead(36);   //Se lee ADC
  delay(1 / (4 * 60));                 //Frecuencia de muestreo de 4 veces 60 Hz (En realidad se obtiene una fs inferior, pero no tanto)

  cuadradoVolt[pos] = (lecturaVolt[pos] - promedioVolt) * (lecturaVolt[pos] - promedioVolt) / (multVolt); //Se calcula el cuadrado de la lectura reescalada
  cuadradoCorr[pos] = (lecturaCorr[pos] - promedioCorr) * (lecturaCorr[pos] - promedioCorr) / (multCorr); //Se calcula el cuadrado de la lectura reescalada
  pos++;
  if (pos == 999) {               //Si se llega a la última posición se vuelve a la primera
    pos = 0;
    //Sacando promedios nuevamente cada 1000 muestras
    for (int i = 0; i <= 999; i++) {
      promedioVolt += lecturaVolt[i];
      promedioCorr += lecturaCorr[i];
    }
    promedioVolt = promedioVolt / 1000;
    promedioCorr = promedioCorr / 1000;
  }

  suma = 0;
  for (int i = 0; i <= 999; i++) {
    suma += cuadradoVolt[i];       //Se suman todos los valores cuadráticos.
  }
  rmsVolt = sqrt(suma / 1000);    //Calcula valor RMS al sacar raiz de promedio de valores cuadráticos

  suma = 0;
  for (int i = 0; i <= 999; i++) {
    suma += cuadradoCorr[i];       //Se suman todos los valores cuadráticos.
  }
  rmsCorr = sqrt(suma / 1000);    //Calcula valor RMS al sacar raiz de promedio de valores cuadráticos


  //Imprime por serial

  //Serial.print("VN =  ");
  //Serial.println(lecturaVolt[i]);
  //Serial.print("VP =  ");
  //Serial.println(lecturaCorr[i]);
  //Serial.print("V RMS = ");
  Serial.println(rmsVolt);
  //Serial.print("C RMS = ");
  //Serial.println(rmsCorr);
}
