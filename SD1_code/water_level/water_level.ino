int inpin=2;// initialize pin 7
int val;// define val
void setup()
{ 
Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
pinMode(inpin,INPUT);// set button pin as “input”
}
void loop()
{
val=digitalRead(inpin);// read the level value of pin 7 and assign if to val
Serial.println(val); // print the data from the sensor
delay(100);
if(val==LOW)// check if the button is pressed, if yes, turn on the LED
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
else
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
}
