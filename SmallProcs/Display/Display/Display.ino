#include<SparkFun_MicroMod_Button.h>
#include<Wire.h>
#include "JimsFont.h"
#include "DisplayLine.h"
#include "GlobalData.h"
 
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
int screenStatus = NAV_BATT;
MicroModButton button;
DisplayLine gStatusLine(10,12,"Status: ",(void*)&gStatus,&formatterByte);
DisplayLine gIPLine(10,38,"IP: ",(void*)&gIP,&formatterIP);
DisplayLine gDockerLine(10,64,"Docked: ",(void*)&gDocked,&formatterDock);


DisplayLine gMapNameLine(10,12,"Map: ",(void*)&gMapName,&formatterString);
DisplayLine gXMeterLine(10,38,"X: ",(void*)&gXMeters,formatterInt16Fixed);
DisplayLine gYMeterLine(175,38,"Y: ",(void*)&gYMeters,formatterInt16Fixed);
DisplayLine gThetaLine(10,64,"Theta: ",(void*)&gTheta,formatterInt16Fixed);
DisplayLine gLatLine(10,90,"Lat: ",(void*)&gLat,formatterFloat);
DisplayLine gLongLine(10,116,"Long: ",(void*)&gLong,formatterFloat);

DisplayLine gChargeLine(10,12,"Charge: ",(void*)&gCharge,formatterCharge);
DisplayLine gVoltsLine(10,38,"Volts: ",(void*)&gVolt,formatterInt16Fixed);
DisplayLine gCurrentLine(10,64,"Current: ",(void*)&gCurrent,formatterInt16Fixed);
DisplayLine gTempLine(10,90,"Temp: ",(void*)&gTemp,formatterInt16Fixed);


/*
extern uint8_t ;
extern int16_t ;
extern int16_t ;
extern int16_t ;
extern uint16_t gRSCaution;
extern uint16_t gRSWarning;
extern uint16_t gRSStatus;
extern uint16_t gRSInfo;
*/

void setup() {
  SERIAL_PORT.begin(115200);
  SERIAL_PORT.println("TurtleBot4 Display");

  delay(2500);

  Wire.begin();
  Serial.println("!A");
  if(!button.begin()) //Connect to the buttons 
  {
    Serial.println("Buttons not found");
    while(1);
  }

  Wire.begin(43);
  Wire.onReceive(receiveEvent);

  Serial.println("A");
  for(int i=0;i<INDEX_MAX;i++)
  {
    gStatusFlag[i] = 0;
    gDisplayFlag[i] = 0;
  }
  
  Serial.println("B");

  myTFT.begin(DC_PIN, CS_PIN, PWM_PIN, SPI_PORT, SPI_SPEED);
  myTFT.setInterfacePixelFormat(ILI9341_PXLFMT_16);
  myTFT.clearDisplay();

//  ILI9341_color_16_t defaultColor = myTFT.rgbTo16b( 255, 255, 255);
  font.setFont(&FreeSans18pt7b);

  Serial.println("C");
  frame();
  Serial.println("D");
}

uint8_t oldPressed = 0;

void loop() {
//  Serial.println("Check Status");
  checkStatus();
//  Serial.println("Check Status Finished");

/*  
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
*/  

  bool redraw = false;
  if(NAV_INFO ==screenStatus)
  {
    for(int i=0;i<3;i++)
    {
      if(1 == gDisplayFlag[i])
      {
        Serial.print("redraw:");
        Serial.print(i);
        Serial.println(" ");
  
        redraw = true;
        gDisplayFlag[i] = 0;
      }
    }
  }
  if(NAV_BATT ==screenStatus)
  {
    for(int i=8;i<12;i++)
    {
      if(1 == gDisplayFlag[i])
      {
        Serial.print("redraw:");
        Serial.print(i);
        Serial.println(" ");
  
        redraw = true;
        gDisplayFlag[i] = 0;
      }
    }
  }
  if(NAV_LOC ==screenStatus)
  {
    for(int i=3;i<8;i++)
    {
      if(1 == gDisplayFlag[i])
      {
        Serial.print("redraw:");
        Serial.print(i);
        Serial.println(" ");
  
        redraw = true;
        gDisplayFlag[i] = 0;
      }
    }
    if(1 == gDisplayFlag[INDEX_MAP_NAME])
    {
      Serial.println("Map Name ");

      redraw = true;
      gDisplayFlag[INDEX_MAP_NAME] = 0;
    }
  }
  if(true == redraw)
  {
      frame();
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

  gStatusLine.drawString(font);
  gIPLine.drawString(font);
  gDockerLine.drawString(font);
}

void displayLocation()
{
  ILI9341_color_16_t color;
  color = myTFT.rgbTo16b( 255, 255, 255 );
  font.setFont(&FreeSans12pt7b);
  font.setTextColor(color);

  gMapNameLine.drawString(font);
  gXMeterLine.drawString(font);
  gYMeterLine.drawString(font);
  gThetaLine.drawString(font);
  gLatLine.drawString(font);
  gLongLine.drawString(font);
  
}

void displayBattery()
{
  ILI9341_color_16_t color;
  color = myTFT.rgbTo16b( 255, 255, 255 );
  font.setFont(&FreeSans12pt7b);
  font.setTextColor(color);

  gChargeLine.drawString(font);
  gVoltsLine.drawString(font);
  gCurrentLine.drawString(font);
  gTempLine.drawString(font);
}

void displayRobotStatus()
{
  
}

void receiveEvent(int howMany)
{
  (void)howMany; // avoid compiler warning about unused parameter
  int command = Wire.read(); // receive byte as a character
  Serial.print(command);         // print the character
  Serial.print(":");         // print the character
  int theLocation = command - LOC_START;
  
  while(0 < Wire.available()) // loop through all but the last
  {
    int c = Wire.read(); // receive byte as a character
    gTotal[theLocation] = c;
    theLocation++;
    Serial.print(c);         // print the character
    Serial.print(" ");         // print the character
  }
  Serial.println(" ");         // print the character
  gStatusFlag[getIndex(command)] =1;
  gDisplayFlag[getIndex(command)] =1;
}

 
