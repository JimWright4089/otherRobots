#ifndef JIMS_FONTS_H
#define JIMS_FONTS_H

#include "HyperDisplay_4DLCD-320240_4WSPI.h"
#include "gfxfont.h"
#include "FreeMono12pt7b.h"
#include "FreeMono18pt7b.h"
#include "FreeMono24pt7b.h"
#include "FreeSans12pt7b.h"
#include "FreeSans18pt7b.h"
#include "FreeSans24pt7b.h"

class JimsFont 
{
public:
  JimsFont(LCD320240_4WSPI* myTFT);
  void setFont(const GFXfont *f);
  void setLocation(int x, int y);
  void setTextColor(ILI9341_color_16_t color);
  void drawChar(unsigned char c);
  void drawString(const char *c);

protected:
  ILI9341_color_16_t mColor; 
  GFXfont* mGfxFont = (GFXfont*)&FreeSans12pt7b;
  int mCursorX = 0;
  int mCursorY = 0;
  LCD320240_4WSPI* mTFT = NULL;
};

#endif
