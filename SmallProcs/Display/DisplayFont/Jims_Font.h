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

class Jims_Font 
{
public:
  Jims_Font(LCD320240_4WSPI* myTFT); // Constructor

protected:
  ILI9341_color_16_t mDefaultColor = 0xFFFF; 
  GFXfont* mGfxFont = &FreeSans12pt7b;
  int mCursorY = 0;
  int mCursorX = 0;
  LCD320240_4WSPI* mTFT;
};

#endif
