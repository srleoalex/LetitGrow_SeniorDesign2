// use atmega328p (oldbootloader) 
void setup() {
  pinMode(3, OUTPUT);    // sets the digital pin 2 as Output
}

void loop() {
  digitalWrite(3, HIGH); // sets the digital pin 2 On
  delay(10000);            // waits for 10 second
  digitalWrite(3, LOW);  // sets the digital pin 2 Off
  delay(5000);            // waits for 5 second
}
