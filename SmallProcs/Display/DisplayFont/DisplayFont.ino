#include "Jims_Font.h"

#define SERIAL_PORT Serial    // Allows users to easily change target serial port (e.g. SAMD21's SerialUSB)

#define PWM_PIN PWM0             // Pin definitions
#define CS_PIN D0
#define DC_PIN D1
#define SPI_PORT SPI
#define SPI_SPEED 32000000        // Requests host uC to use the fastest possible SPI speed up to 32 MHz

LCD320240_4WSPI myTFT;

ILI9341_color_16_t defaultColor; // Global objects are used for default colors so that they are always in-scope
GFXfont *gfxFont;
int cursor_y = 0;
int cursor_x = 0;

void setup() {
  SERIAL_PORT.begin(115200);
  SERIAL_PORT.println("Example1 Display Test : SparkFun TFT LCD 2.4in");

  defaultColor = myTFT.rgbTo16b( 0, 255, 255);
  myTFT.begin(DC_PIN, CS_PIN, PWM_PIN, SPI_PORT, SPI_SPEED);
  myTFT.setInterfacePixelFormat(ILI9341_PXLFMT_16);
  myTFT.clearDisplay();

  Jims_Font font(&myTFT);
  font.setFont(&FreeSans12pt7b);
  font.setTextColor(defaultColor);

  font.setLocation(0,0);
  for(int i=0;i<16;i++)
  {
    font.drawChar('J');
    font.drawChar('i');
    font.drawChar('m');
    font.drawChar(' ');
  }

  for(int j=1;j<10;j++)
  {
    font.setLocation(0,j*24);
    for(int i=0;i<7;i++)
    {
      font.drawChar('X');
      font.drawChar('Y');
      font.drawChar('U');
      font.drawChar('Z');
    }
  }
}

void loop() {
    delay(500);
}
