#include "HyperDisplay_4DLCD-320240_4WSPI.h"

#define SERIAL_PORT Serial    // Allows users to easily change target serial port (e.g. SAMD21's SerialUSB)

#define PWM_PIN PWM0             // Pin definitions
#define CS_PIN D0
#define DC_PIN D1
#define SPI_PORT SPI
#define SPI_SPEED 32000000        // Requests host uC to use the fastest possible SPI speed up to 32 MHz

LCD320240_4WSPI myTFT;

ILI9341_color_16_t defaultColor; // Global objects are used for default colors so that they are always in-scope

void setup() {
  SERIAL_PORT.begin(115200);
  SERIAL_PORT.println("Example1 Display Test : SparkFun TFT LCD 2.4in");

  myTFT.begin(DC_PIN, CS_PIN, PWM_PIN, SPI_PORT, SPI_SPEED);
  myTFT.setInterfacePixelFormat(ILI9341_PXLFMT_16);
  myTFT.clearDisplay();

  myTFT.setTextCursor(0,0);            // Sets the cursor relative to the current window, however the current (default) window is equivalent to the whole display. (0,0) is the upper left corner and (myTFT.xExt-1, myTFT.yExt-1) is the lower right
  myTFT.setCurrentWindowColorSequence((color_t)&defaultColor);

  uint16_t hue = HSV_HUE_MIN;
  while(hue <= HSV_HUE_MAX){
    myTFT.setTextCursor(0,0);
    defaultColor = myTFT.hsvTo16b( hue+=30, 255, 255 );
    myTFT.println("Hello world!");   // Prints using the default font at the cursor location
    delay(500);
  }


  frame();
}

void loop() {
    delay(500);
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

  for(uint8_t indi = 0; indi < 6; indi+=1)
  {
    myTFT.line(0,98+indi,37,98+indi,1,(color_t)&color);
    myTFT.line(0,228+indi,37,228+indi,1,(color_t)&color);
  }


}
