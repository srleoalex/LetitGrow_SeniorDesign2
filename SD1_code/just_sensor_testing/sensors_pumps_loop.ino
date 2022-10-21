// Sensor testing for Let it Grow
// When testing on the PCB make sure to set the processor of the nano to the old bootloader

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

// define and include TDS library and variables
#include <EEPROM.h>
#include "GravityTDS.h"
#define TdsSensorPin A2
GravityTDS gravityTds;

// define ph variables and calibration value
float calibration_value = 21.34+.5;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
float ph_act;
int ph_pin = A1;

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
}

// caluclates ph by averaging a series of ph values 
int phSensorCalculations(){
  for(int i=0;i<10;i++) 
  { 
      buffer_arr[i]=analogRead(A1);
      delay(30);
  }
    for(int i=0;i<9;i++)
    {
      for(int j=i+1;j<10;j++)
      {
        if(buffer_arr[i]>buffer_arr[j])
        {
          temp=buffer_arr[i];
          buffer_arr[i]=buffer_arr[j];
          buffer_arr[j]=temp;
        }
      }
    }
    avgval=0;
    for(int i=2;i<8;i++)
    {
    avgval+=buffer_arr[i];
    }
    float volt=(float)avgval*5.0/1024/6; 
    ph_act = -5.70 * volt + calibration_value;

    return ph_act;
}

// function that reads and prints sensor values for testing
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
  int waterLevelVal = digitalRead(waterLevelPin);// read the level value of pin 7 and assign if to val

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

  Serial.print("pH: ");
  Serial.println(phSensorCalculations());

}

void loop() {

  printSensorVal();

  // one second delay
  delay(1000);
}
