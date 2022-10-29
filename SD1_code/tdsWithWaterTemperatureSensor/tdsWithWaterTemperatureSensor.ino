// go to https://wiki.dfrobot.com/Gravity__Analog_TDS_Sensor___Meter_For_Arduino_SKU__SEN0244
// for library and calibration instructions
#include <EEPROM.h>
#include "GravityTDS.h"

#define TdsSensorPin A2
GravityTDS gravityTds;

float temperature;
float tdsValue = 0;

// include water temperature sensor library and initialize variables
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS A4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup()
{
  Serial.begin(115200);
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization

  // initialize water temperature sensor
  sensors.begin();

}

void loop()
{
  // read in water temperature sensor
  sensors.requestTemperatures(); 
  float waterTempC = sensors.getTempCByIndex(0);
  float waterTempF = sensors.toFahrenheit(waterTempC);
  temperature = waterTempC;
  Serial.print("Water Temp in Celcius:");
  Serial.println(temperature);

  //temperature = readTemperature();  //add your temperature sensor and read it
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate
  tdsValue = gravityTds.getTdsValue();  // then get the value
  Serial.print(tdsValue,0);
  Serial.println("ppm");
  delay(1000);
}
