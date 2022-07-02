#include <Wire.h>

void setup()
{
  Wire.begin(43);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(115200);           // start serial for output
}

void loop()
{
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  (void)howMany; // avoid compiler warning about unused parameter
  int command = Wire.read(); // receive byte as a character
  Serial.print(command);         // print the character
  Serial.print(":");         // print the character
  
  while(0 < Wire.available()) // loop through all but the last
  {
    int c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
    Serial.print(" ");         // print the character
  }
  Serial.println(" ");         // print the character
}
