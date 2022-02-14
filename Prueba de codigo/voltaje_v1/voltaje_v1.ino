#include <ZMPT101B.h>

ZMPT101B voltageSensor(34);

void setup() {
  Serial.begin(115200);
  delay(1000);
  //analogReadResolution(10);
  voltageSensor.calibrate();
  voltageSensor.setSensitivity(0.019); 
}

void loop() {
  float U = voltageSensor.getVoltageAC(60);
  if (U < 10) {
    U = 0;
  }
  Serial.println(String("U = ") + U + " V");
  delay(1000);
}
