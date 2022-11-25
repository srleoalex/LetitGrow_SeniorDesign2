
// Sensor testing for Let it Grow
// When testing on the PCB make sure to set the processor of the nano to the old bootloader

// include sort library
#include <KickSort.h>

// include air/hum sensor library and initialize variables
#include "DHT.h"
#define DHTPIN A5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// include water temperature sensor library and initialize variables
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS A4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// initailize water level variables
int waterLevelPin=A3;// define pin A3

// A2 is A0
// define and include TDS library and variables
// must download the zip file for the library and import it from https://wiki.dfrobot.com/Gravity__Analog_TDS_Sensor___Meter_For_Arduino_SKU__SEN0244
#include <EEPROM.h>
#include "GravityTDS.h"
#define TdsSensorPin A0
GravityTDS gravityTds;
// define tds range
float tdsLow = 600;

// define pH variables and calibration value
#include <Arduino.h>
const float m = -6.16379; 
int phPin = A1;
// define ph range
float phLow = 5;
float phHigh = 8; 

// define pins for pumps
int waterPumpPin = 2;
int phAcidPumpPin = 3;
int phBasePumpPin = 4;
int nutrientAPumpPin = 5;
int nutrientBPumpPin = 6;

// define temporary pump delay
int waterPumpDelay = 10000;
int phPumpDelay = 2000;
int nutrientPumpDelay = 2000;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // initialize the air humidity sensor
  dht.begin();
  
  // initialize water temperature sensor
  sensors.begin();
  
  // set A3 pin as “input” for water level sensor
  pinMode(waterLevelPin,INPUT);

  // initialize tds sensor
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization

  // set pump pins for output
  pinMode(waterPumpPin,OUTPUT);
  pinMode(phAcidPumpPin,OUTPUT);
  pinMode(phBasePumpPin,OUTPUT);
  pinMode(nutrientAPumpPin,OUTPUT);
  pinMode(nutrientBPumpPin,OUTPUT);
}

// function to return ph value median of ten values 
// (individual ph values can very greatly) 
double getPhValue(){
  double bufferArray[10];
  int size = 10;

  // fill array with po values
  for (int i=0; i<size; i++){
    bufferArray[i] = analogRead(phPin) * 5.0 / 1024;
    delay(30);
  }

  // sort array
  KickSort<double>::insertionSort(bufferArray, size);

  // find median
  double Po = (bufferArray[(size-1)/2] + bufferArray[size/2])/2.0;
  // find ph value
  double phValue = 6.86 - (2.7 - Po) * m;

  return phValue;

}


// function that reads and prints sensor values for testing
void printSensorValRasp(){  
  // read air humidity
  float airHumi = dht.readHumidity();
  // read air temperature as Celsius
  float airTempC = dht.readTemperature();
  // read temperature as Fahrenheit
  float airTempF = dht.readTemperature(true);

  // read in water temperature sensor
  sensors.requestTemperatures(); 
  float waterTempC = sensors.getTempCByIndex(0);
  float waterTempF = sensors.toFahrenheit(waterTempC);

  // read in water level sensor
  int waterLevelVal = digitalRead(waterLevelPin);// read the level value of pin A3 and assign if to val

  // read in TDS sensor
  gravityTds.setTemperature(waterTempC);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate
  float tdsValue = gravityTds.getTdsValue();  // then get the value

  // print sensor values for raspberry pi

  // TDS sensor
  Serial.print(tdsValue,0);
  Serial.print(" ");

  // pH Sensor
  Serial.print(getPhValue());
  Serial.print(" ");

  // Water Level Sensor
  // 0 - Indicates no liquid, 1 - Indicates probe is submerged
  Serial.print(waterLevelVal); // print the data from the sensor
  Serial.print(" "); 

  // Water Temperature sensor
  Serial.print(waterTempF);
  Serial.print(" ");

  // Air/Hum sensor
  Serial.print(airHumi);
  Serial.print(" ");

  Serial.print(airTempF);
}

void printSensorVal(){  
  // read air humidity
  float airHumi = dht.readHumidity();
  // read air temperature as Celsius
  float airTempC = dht.readTemperature();
  // read temperature as Fahrenheit
  float airTempF = dht.readTemperature(true);

  // read in water temperature sensor
  sensors.requestTemperatures(); 
  float waterTempC = sensors.getTempCByIndex(0);
  float waterTempF = sensors.toFahrenheit(waterTempC);

  // read in water level sensor
  int waterLevelVal = digitalRead(waterLevelPin);// read the level value of pin A3 and assign if to val

  // read in TDS sensor
  gravityTds.setTemperature(waterTempC);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate
  float tdsValue = gravityTds.getTdsValue();  // then get the value

  // print sensor values for testing
  // Air/Hum sensor
  Serial.print("AirHumidity: ");
  Serial.print(airHumi);
  Serial.print("%");

  Serial.print("  |  "); 

  Serial.print("AirTemperature: ");
  Serial.print(airTempC);
  Serial.print("°C ~ ");
  Serial.print(airTempF);
  Serial.println("°F");

  // Water Temperature sensor
  Serial.print("WaterTemp C:  ");
  Serial.println(waterTempC);
  Serial.print("WaterTemp F:  ");
  Serial.println(waterTempF);

  // Water Level Sensor
  // 0 - Indicates no liquid, 1 - Indicates probe is submerged
  Serial.println(waterLevelVal); // print the data from the sensor

  // TDS sensor
  Serial.print(tdsValue,0);
  Serial.println("ppm");

  // pH Sensor
  Serial.print("ph value = ");
  Serial.println(getPhValue());


}

// function to run all pumps at once for testing
// HIGH activates pumps, LOW stops pumps
void runAllPumps (){

    digitalWrite(waterPumpPin, HIGH);
    digitalWrite(phAcidPumpPin, HIGH);
    digitalWrite(phBasePumpPin, HIGH);
    digitalWrite(nutrientAPumpPin, HIGH);
    digitalWrite(nutrientBPumpPin, HIGH);
    delay(10000);
    digitalWrite(waterPumpPin, LOW);
    digitalWrite(phAcidPumpPin, LOW);
    digitalWrite(phBasePumpPin, LOW);
    digitalWrite(nutrientAPumpPin, LOW);
    digitalWrite(nutrientBPumpPin, LOW);
}

// loop to check and raise water level
// 0 means unsubmerged 1 means submerged
void waterSensorLoop(){
  // reads in water sensor value
  int waterLevelVal = digitalRead(waterLevelPin);
  // Serial.println(waterLevelVal);

  // loop while water sensor is unsubmerged
  while(waterLevelVal == 0)
  {
    // Run pump for designated amount of time
    digitalWrite(waterPumpPin, HIGH);
    delay(waterPumpDelay);
    digitalWrite(waterPumpPin, LOW);

    // wait 2 min sec
    //delay(120000);
    
    // wait 30 sec
    delay(30000);

    // test delay 5 seconds
    // delay(5000);

    // read new water sensor value sensor
    waterLevelVal = digitalRead(waterLevelPin);

    // read water level val to serial monitor
    // Serial.println(waterLevelVal);
  }
}

// loop to check water ph and bring water ph within threshold values
void phSensorLoop(){
  // reads in ph sensor value

  double phVal = getPhValue();

  // Serial.print("ph value = ");
  // Serial.println(phVal);
  
  // loop while water ph is not within threshold values
  while(phVal < phLow || phVal > phHigh)
  {
    // if the water ph is too acidic run base pump for designated amount of time
    if(phVal < phLow)
    {
      digitalWrite(phBasePumpPin, HIGH);
      delay(phPumpDelay);
      digitalWrite(phBasePumpPin, LOW);
    }
    // if the water ph is too basic run acid pump for designated amount of time
    if(phVal > phHigh)
    {
      digitalWrite(phAcidPumpPin, HIGH);
      delay(phPumpDelay);
      digitalWrite(phAcidPumpPin, LOW);
    }
    
    // wait 2 min sec
    //delay(120000);

    // wait 30 sec
    delay(30000);

    // test delay 5 seconds
    // delay(5000);

    // read new ph sensor value sensor
    phVal = getPhValue();
    // Serial.print("ph value = ");
    // Serial.println(phVal);
  }
}

// loop to check water tds and bring water tds within threshold values
void tdsSensorLoop(){
  // read water tempterature value and calibrate the tds sensor
  sensors.requestTemperatures(); 
  float waterTempC = sensors.getTempCByIndex(0);

  // set the temperature and execute temperature compensation
  // gravityTds.setTemperature(24); // manual
  gravityTds.setTemperature(waterTempC); // from water temperature sensor

  gravityTds.update();  //sample and calculate
  float tdsVal = gravityTds.getTdsValue();  // then get the value

  // Serial.print("TDS value: ");
  // Serial.println(tdsVal);

  // loop while water tds is not within threshold values
  while(tdsVal < tdsLow)
  {
    // if the water tds is too low run nutrientA pump for designated amount of time

    digitalWrite(nutrientAPumpPin, HIGH);
    digitalWrite(nutrientBPumpPin, HIGH);
    delay(nutrientPumpDelay);
    digitalWrite(nutrientAPumpPin, LOW);
    digitalWrite(nutrientBPumpPin, LOW);
    
    // wait 2 min sec
    //delay(120000);

    // wait 30 sec
    delay(30000);
    
    // test delay 5 sec
    // delay(5000);

    // read water tempterature value and calibrate the tds sensor
    sensors.requestTemperatures(); 
    float waterTempC = sensors.getTempCByIndex(0);

    // set the temperature and execute temperature compensation
    // gravityTds.setTemperature(24); // manual
    gravityTds.setTemperature(waterTempC); // from water temperature sensor

    gravityTds.update();  //sample and calculate

    // read new tds sensor value sensor
    tdsVal = gravityTds.getTdsValue();

    // Serial.print("TDS value: ");
    // Serial.println(tdsVal);
  }
}

// void modThreshold(data){
  // tdsLow;
  // phLow;
  // phHigh;

// }

void loop() {

    // if (Serial.available() > 0) {
      // String data = Serial.readStringUntil('\n');
      // modThreshold(data);
    // }

  // sensor loops
  printSensorValRasp();
  waterSensorLoop();
  phSensorLoop();
  tdsSensorLoop();

  //testing sensors and pumps
  //runAllPumps();
  //printSensorVal();

  // test delay
  delay(30000);

  // check sensors after an hour
  // 1 hour delay
  // delay(3600000);
}
