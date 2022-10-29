// include water temperature sensor library and initialize variables
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS A4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600)

  // initialize water temperature sensor
  sensors.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  // read in water temperature sensor
  sensors.requestTemperatures(); 
  float waterTempC = sensors.getTempCByIndex(0);
  float waterTempF = sensors.toFahrenheit(waterTempC);

    // Water Temperature sensor
  Serial.print("WaterTemp C:  ");
  Serial.println(waterTempC);
  Serial.print("WaterTemp F:  ");
  Serial.println(waterTempF);

  delay(1000)

}
