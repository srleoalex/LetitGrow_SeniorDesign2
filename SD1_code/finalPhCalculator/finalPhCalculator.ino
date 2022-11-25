// refer to https://www.e-tinkers.com/2019/11/measure-ph-with-a-low-cost-arduino-ph-sensor-board/
#include <Arduino.h>
#include <KickSort.h>

const int adcPin = A1;
  
// calculate your own m using ph_calibrate.ino
// When using the buffer solution of pH4 for calibration, m can be derived as:
// m = (pH6.86 - pH4) / (po2.7 - po3.164)
const float m = -6.81818182; 

void setup() {
  Serial.begin(9600);
}

double getPoValue(){
  double bufferArray[10];
  int size = 10;

  // fill array with po values
  for (int i=0; i<size; i++){
    bufferArray[i] = analogRead(adcPin) * 5.0 / 1024;
    delay(30);
  }

  // sort array
  KickSort<double>::insertionSort(bufferArray, size);

  // find median
  double poMedian = (bufferArray[(size-1)/2] + bufferArray[size/2])/2.0;

  double poMean = 0;
  for(int i=0; i<size; i++){
    poMean =+ bufferArray[i];
  }
  poMean = poMean/size;


  // find ph value
  // Serial.print(";  PoMedian  = "); Serial.println(poMedian);
  // Serial.print(";  PoMean  = "); Serial.println(poMean);

  return(bufferArray[2]);

}

void loop() {
  // float Po = analogRead(adcPin) * 5.0 / 1024;
  float Po = getPoValue();

  float phValue = 6.86 - (2.7 - Po) * m;
  Serial.print("ph value = "); Serial.println(phValue);
  delay(2000);
}