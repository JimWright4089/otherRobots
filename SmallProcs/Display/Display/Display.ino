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
