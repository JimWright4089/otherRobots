#include<SparkFun_MicroMod_Button.h>
#include<Wire.h>
#include "JimsFont.h"

#define SERIAL_PORT Serial    // Allows users to easily change target serial port (e.g. SAMD21's SerialUSB)

#define PWM_PIN PWM0             // Pin definitions
#define CS_PIN D0
#define DC_PIN D1
#define SPI_PORT SPI
#define SPI_SPEED 32000000        // Requests host uC to use the fastest possible SPI speed up to 32 MHz

#define LABEL_INFO "Information"
#define LABEL_LOC  "Locaton"
#define LABEL_BATT "Battery"
#define LABEL_STAT "Robot Status"

#define NAV_INFO 0
#define NAV_LOC  1
#define NAV_BATT 2
#define NAV_STAT 3
#define NAV_MAX  4

LCD320240_4WSPI myTFT;
JimsFont font(&myTFT);
int screenStatus = NAV_INFO;
MicroModButton button;

void setup() {
  SERIAL_PORT.begin(115200);
  SERIAL_PORT.println("TurtleBot4 Display");

  Wire.begin();
  delay(100); //Wait for serial port to open
  
  if(!button.begin()) //Connect to the buttons 
  {
    Serial.println("Buttons not found");
    while(1);
  }

  myTFT.begin(DC_PIN, CS_PIN, PWM_PIN, SPI_PORT, SPI_SPEED);
  myTFT.setInterfacePixelFormat(ILI9341_PXLFMT_16);
  myTFT.clearDisplay();

//  ILI9341_color_16_t defaultColor = myTFT.rgbTo16b( 255, 255, 255);
  font.setFont(&FreeSans18pt7b);

  frame();
}

uint8_t oldPressed = 0;

void loop() {
  if(button.getPressedInterrupt())  //Check to see if a button has been pressed
  {
    uint8_t pressed = button.getPressed(); //Read which button has been pressed
    
    if(pressed & 0x01)
    {
      Serial.println("Button A pressed!");
    }
    if(pressed & 0x02)
    {
      Serial.println("Button B pressed!");
    }
    if(pressed & 0x04)
    {
      if((0 == oldPressed)&&(NAV_INFO != screenStatus))
      {
        screenStatus--;
        frame();
      }
      Serial.println("Button UP pressed!");
    }
    if(pressed & 0x08)
    {
      if((0 == oldPressed)&&(NAV_STAT != screenStatus))
      {
        screenStatus++;
        frame();
      }
      Serial.println("Button DOWN pressed!");
    }
    oldPressed = pressed;
  }
  delay(100);
}

void frame()
{
  myTFT.clearDisplay();
  ILI9341_color_16_t color;
  font.setFont(&FreeSans18pt7b);

  color = myTFT.rgbTo16b( 255, 255, 255 );
  for(uint8_t indi = 0; indi < 5; indi+=1)
  {
    myTFT.rectangle(0+indi, 0+indi, 239-indi, 319-indi, false, (color_t)&color);
  }

  for(uint8_t indi = 37; indi < 42; indi+=1)
  {
    myTFT.line(indi,0,indi,319,1,(color_t)&color);
  }

  font.setTextColor(color);
  font.setLocation(15,210);
  font.drawString(getLabel());

  switch(screenStatus)
  {
    case(NAV_LOC):
      displayLocation();
      break;
    case(NAV_BATT):
      displayBattery();
      break;
    case(NAV_STAT):
      displayRobotStatus();
      break;
    default:
      displayInformation();
      break;
  }


}

char *getLabel()
{
  switch(screenStatus)
  {
    case(NAV_LOC):
      return LABEL_LOC;
    case(NAV_BATT):
      return LABEL_BATT;
    case(NAV_STAT):
      return LABEL_STAT;
    default:
      screenStatus = NAV_INFO;
      return LABEL_INFO;
  }
}

void displayInformation()
{
  ILI9341_color_16_t color;
  color = myTFT.rgbTo16b( 255, 255, 255 );
  font.setFont(&FreeSans12pt7b);
  font.setTextColor(color);
  font.setLocation(10,12);
  font.drawString("Status:");
  font.setLocation(10,38);
  font.drawString("IP:");
  font.setLocation(10,64);
  font.drawString("Docked:");
}

void displayLocation()
{
  ILI9341_color_16_t color;
  color = myTFT.rgbTo16b( 255, 255, 255 );
  font.setFont(&FreeSans12pt7b);
  font.setTextColor(color);
  font.setLocation(10,12);
  font.drawString("Map:");
  font.setLocation(10,38);
  font.drawString("X:");
  font.setLocation(175,38);
  font.drawString("Y:");
  font.setLocation(10,64);
  font.drawString("Theta:");
  font.setLocation(10,90);
  font.drawString("Lat:");
  font.setLocation(10,116);
  font.drawString("Long:");
}

void displayBattery()
{
  ILI9341_color_16_t color;
  color = myTFT.rgbTo16b( 255, 255, 255 );
  font.setFont(&FreeSans12pt7b);
  font.setTextColor(color);
  font.setLocation(10,12);
  font.drawString("Charge:");
  font.setLocation(10,38);
  font.drawString("Volts:");
  font.setLocation(10,64);
  font.drawString("Current:");
  font.setLocation(10,90);
  font.drawString("Temp:");
}
void displayRobotStatus()
{
  
}





/*
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
*/
 
