#ifndef DISPLAY_LINE_H
#define DISPLAY_LINE_H
 
#include "JimsFont.h"

#define MAX_DISPLAY_LINE 30

class DisplayLine
{
public:
  DisplayLine(int x, int y, char displayLine[],void* value, void (*valueFormatter)(char*, void *));
  void drawString(JimsFont font);

protected:
  char mDisplayLine[MAX_DISPLAY_LINE];
  char mValueLine[MAX_DISPLAY_LINE];
  int mX = 0;
  int mY = 0;
  void* mValue;

  void (*mValueFormatter)(char*, void *);

};

#endif
