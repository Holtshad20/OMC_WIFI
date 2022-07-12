
#include <Arduino.h>
#include <PZEM004Tv30.h>

PZEM004Tv30 pzem(Serial2, 16, 17);


float times = 0;
float old_Voltage = 0;
bool ms = true;


void setup() {
  // Debugging Serial port
  Serial.begin(115200);

  // Uncomment in order to reset the internal energy counter
  // pzem.resetEnergy()
}

void loop() {
  // Print the custom address of the PZEM
  //Serial.print("Custom Address:");
  //Serial.println(pzem.readAddress(), HEX);

  // Read the data from the sensor
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();

  //float energy = pzem.energy();
  //float frequency = pzem.frequency();
  //float pf = pzem.pf();

  if (ms) {
    times = millis();
    ms = false;
  }

  // Check if the data is valid
  if (isnan(voltage)) {
    Serial.println("Error reading voltage");
  } else if (isnan(current)) {
    Serial.println("Error reading current");
  } else if (isnan(power)) {
    Serial.println("Error reading power");
    //    } else if (isnan(energy)) {
    //        Serial.println("Error reading energy");
    //    } else if (isnan(frequency)) {
    //        Serial.println("Error reading frequency");
    //    } else if (isnan(pf)) {
    //        Serial.println("Error reading power factor");
  } else {

    // Print the values to the Serial console
    Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
    Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
    Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
    //        Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
    //        Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
    //        Serial.print("PF: ");           Serial.println(pf);

  }

  if (old_Voltage != voltage) {


    Serial.println();
    Serial.println();
    Serial.println(millis() - times);
    Serial.println();
    Serial.println();
    
    old_Voltage = voltage;
    ms = true;

  }


  Serial.println();
  //delay(100);
}
