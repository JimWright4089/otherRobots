#include "DisplayLine.h"

DisplayLine::DisplayLine(int x, int y, char displayLine[],void* value, void (*valueFormatter)(char*, void *))
{ 
  memcpy(mDisplayLine,displayLine,MAX_DISPLAY_LINE);
  mX = x;
  mY = y;
  mValue = value;
  mValueFormatter = valueFormatter;
}

void DisplayLine::drawString(JimsFont font)
{
  mValueFormatter(mValueLine,mValue);
//  uint8_t* ptr = (uint8_t*)mValue;
//  sprintf(mValueLine,"%d",*ptr);
  font.setLocation(mX,mY);
  font.drawString(mDisplayLine);
  font.drawString(mValueLine);
}
