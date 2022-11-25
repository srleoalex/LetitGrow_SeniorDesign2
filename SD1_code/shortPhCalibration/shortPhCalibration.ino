#include <Arduino.h>
#include <KickSort.h>

const int adcPin = A1;

void setup() {
  Serial.begin(9600);
}

void getPoValue(){
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
  Serial.print(";  PoMedian  = "); Serial.println(poMedian);
  Serial.print(";  PoMean  = "); Serial.println(poMean);


}

void loop() {
  int adcValue = analogRead(adcPin);
  float phVoltage = (float)adcValue * 5.0 / 1024;
  Serial.print("ADC = "); Serial.print(adcValue);
  Serial.print(";  Po  = "); Serial.println(phVoltage, 3);
  // getPoValue();
  delay(1000);
}
//(7-4.00)/(2.67-3.11)
