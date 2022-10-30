// refer to https://www.e-tinkers.com/2019/11/measure-ph-with-a-low-cost-arduino-ph-sensor-board/
#include <Arduino.h>
const int adcPin = A1;
  
// calculate your own m using ph_calibrate.ino
// When using the buffer solution of pH4 for calibration, m can be derived as:
// m = (pH6.86 - pH4) / (po2.7 - po3.164)
const float m = -6.16379; 

void setup() {
  Serial.begin(115200);
}

void loop() {
  float Po = analogRead(adcPin) * 5.0 / 1024;
  float phValue = 6.86 - (2.7 - Po) * m;
  Serial.print("ph value = "); Serial.println(phValue);
  delay(5000);
}