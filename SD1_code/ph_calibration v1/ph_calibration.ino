int pH_Value; 
float Voltage;

void setup() 
{ 
  Serial.begin(9600);
  pinMode(pH_Value, INPUT); 
} 
 
void loop() 
{ 
  pH_Value = analogRead(A1); 
  Voltage = pH_Value * (5.0 / 1023.0);
  Serial.println(F("Voltage: "));  
  Serial.println(Voltage); 

  delay(500); 
}
