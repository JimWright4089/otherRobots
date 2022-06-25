#include "HyperDisplay_4DLCD-320240_4WSPI.h"
#include "gfxfont.h"
#include "FreeMono12pt7b.h"
#include "FreeMono18pt7b.h"
#include "FreeMono24pt7b.h"
#include "FreeSans12pt7b.h"
#include "FreeSans18pt7b.h"
#include "FreeSans24pt7b.h"

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

  myTFT.setTextCursor(50,50);            // Sets the cursor relative to the current window, however the current (default) window is equivalent to the whole display. (0,0) is the upper left corner and (myTFT.xExt-1, myTFT.yExt-1) is the lower right
  myTFT.setCurrentWindowColorSequence((color_t)&defaultColor);
//  setFont(&FreeMono18pt7b);
  setFont(&FreeSans12pt7b);
//  setFont(&FreeMono12pt7b);

  myTFT.pixel(0, 0, (color_t)&defaultColor);
  myTFT.pixel(50, 0, (color_t)&defaultColor);
  myTFT.pixel(0, 100, (color_t)&defaultColor);
  myTFT.pixel(100, 100, (color_t)&defaultColor);

  delay(1500);

  for(int i=0;i<16;i++)
  {
    drawChar('J');
    drawChar('i');
    drawChar('m');
    drawChar(' ');
  }

  for(int j=1;j<10;j++)
  {
  cursor_y = j*24;
  cursor_x = 0;
  for(int i=0;i<7;i++)
  {
    drawChar('X');
    drawChar('Y');
    drawChar('U');
    drawChar('Z');
  }
  }
}

void loop() {
    delay(500);
}


/**************************************************************************/
/*!
    @brief Set the font to display when print()ing, either custom or default
    @param  f  The GFXfont object, if NULL use built in 6x8 font
*/
/**************************************************************************/
void setFont(const GFXfont *f) {
  if (f) {          // Font struct pointer passed in?
    if (!gfxFont) { // And no current font struct?
      // Switching from classic to new font behavior.
      // Move cursor pos down 6 pixels so it's on baseline.
      cursor_y += 6;
    }
  } else if (gfxFont) { // NULL passed.  Current font struct defined?
    // Switching from new to classic font behavior.
    // Move cursor pos up 6 pixels so it's at top-left of char.
    cursor_y -= 6;
  }
  gfxFont = (GFXfont *)f;
}

inline GFXglyph *pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c) {
#ifdef __AVR__
  return &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
#else
  // expression in __AVR__ section may generate "dereferencing type-punned
  // pointer will break strict-aliasing rules" warning In fact, on other
  // platforms (such as STM32) there is no need to do this pointer magic as
  // program memory may be read in a usual way So expression may be simplified
  return gfxFont->glyph + c;
#endif //__AVR__
}

inline uint8_t *pgm_read_bitmap_ptr(const GFXfont *gfxFont) {
#ifdef __AVR__
  return (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);
#else
  // expression in __AVR__ section generates "dereferencing type-punned pointer
  // will break strict-aliasing rules" warning In fact, on other platforms (such
  // as STM32) there is no need to do this pointer magic as program memory may
  // be read in a usual way So expression may be simplified
  return gfxFont->bitmap;
#endif //__AVR__
}


// Draw a character
/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    x   Bottom left corner x coordinate
    @param    y   Bottom left corner y coordinate
    @param    c   The 8-bit font-indexed character (likely ascii)
    @param    color 16-bit 5-6-5 Color to draw chraracter with
    @param    bg 16-bit 5-6-5 Color to fill background with (if same as color,
   no background)
    @param    size_x  Font magnification level in X-axis, 1 is 'original' size
    @param    size_y  Font magnification level in Y-axis, 1 is 'original' size
*/
/**************************************************************************/
void drawChar(unsigned char c) 
{
  c -= (uint8_t)pgm_read_byte(&gfxFont->first);
  GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c);
  uint8_t *bitmap = pgm_read_bitmap_ptr(gfxFont);
  uint16_t top = 240 - (gfxFont->yAdvance/2);

  uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
  uint16_t w = pgm_read_byte(&glyph->width);
  uint16_t h = pgm_read_byte(&glyph->height);
  int8_t xo = pgm_read_byte(&glyph->xOffset);
  int8_t yo = pgm_read_byte(&glyph->yOffset);
  uint16_t xx, yy, bits = 0, bit = 0;

  for (yy = 0; yy < h; yy++) {
    for (xx = 0; xx < w; xx++) {
      if (!(bit++ & 7)) {
        bits = pgm_read_byte(&bitmap[bo++]);
      }
      if (bits & 0x80) {
        
          myTFT.pixel(top-(cursor_y + yo + yy), cursor_x + xo + xx, (color_t)&defaultColor);
      }
      bits <<= 1;
    }
  } // End classic vs custom font
  cursor_x += w+3;
}
















#ifdef JUNK
// TEXT- AND CHARACTER-HANDLING FUNCTIONS ----------------------------------

// Draw a character
/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    x   Bottom left corner x coordinate
    @param    y   Bottom left corner y coordinate
    @param    c   The 8-bit font-indexed character (likely ascii)
    @param    color 16-bit 5-6-5 Color to draw chraracter with
    @param    bg 16-bit 5-6-5 Color to fill background with (if same as color,
   no background)
    @param    size  Font magnification level, 1 is 'original' size
*/
/**************************************************************************/
void Adafruit_GFX::drawChar(int16_t x, int16_t y, unsigned char c,
                            uint16_t color, uint16_t bg, uint8_t size) {
  drawChar(x, y, c, color, bg, size, size);
}

/**************************************************************************/
/*!
    @brief  Print one byte/character of data, used to support print()
    @param  c  The 8-bit ascii character to write
*/
/**************************************************************************/
size_t Adafruit_GFX::write(uint8_t c) {
  if (!gfxFont) { // 'Classic' built-in font

    if (c == '\n') {              // Newline?
      cursor_x = 0;               // Reset x to zero,
      cursor_y += textsize_y * 8; // advance y one line
    } else if (c != '\r') {       // Ignore carriage returns
      if (wrap && ((cursor_x + textsize_x * 6) > _width)) { // Off right?
        cursor_x = 0;                                       // Reset x to zero,
        cursor_y += textsize_y * 8; // advance y one line
      }
      drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x,
               textsize_y);
      cursor_x += textsize_x * 6; // Advance x one char
    }

  } else { // Custom font

    if (c == '\n') {
      cursor_x = 0;
      cursor_y +=
          (int16_t)textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
    } else if (c != '\r') {
      uint8_t first = pgm_read_byte(&gfxFont->first);
      if ((c >= first) && (c <= (uint8_t)pgm_read_byte(&gfxFont->last))) {
        GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c - first);
        uint8_t w = pgm_read_byte(&glyph->width),
                h = pgm_read_byte(&glyph->height);
        if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
          int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
          if (wrap && ((cursor_x + textsize_x * (xo + w)) > _width)) {
            cursor_x = 0;
            cursor_y += (int16_t)textsize_y *
                        (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
          }
          drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x,
                   textsize_y);
        }
        cursor_x +=
            (uint8_t)pgm_read_byte(&glyph->xAdvance) * (int16_t)textsize_x;
      }
    }
  }
  return 1;
}

/**************************************************************************/
/*!
    @brief   Set text 'magnification' size. Each increase in s makes 1 pixel
   that much bigger.
    @param  s  Desired text size. 1 is default 6x8, 2 is 12x16, 3 is 18x24, etc
*/
/**************************************************************************/
void Adafruit_GFX::setTextSize(uint8_t s) { setTextSize(s, s); }

/**************************************************************************/
/*!
    @brief   Set text 'magnification' size. Each increase in s makes 1 pixel
   that much bigger.
    @param  s_x  Desired text width magnification level in X-axis. 1 is default
    @param  s_y  Desired text width magnification level in Y-axis. 1 is default
*/
/**************************************************************************/
void Adafruit_GFX::setTextSize(uint8_t s_x, uint8_t s_y) {
  textsize_x = (s_x > 0) ? s_x : 1;
  textsize_y = (s_y > 0) ? s_y : 1;
}

/**************************************************************************/
/*!
    @brief  Helper to determine size of a character with current font/size.
            Broke this out as it's used by both the PROGMEM- and RAM-resident
            getTextBounds() functions.
    @param  c     The ASCII character in question
    @param  x     Pointer to x location of character. Value is modified by
                  this function to advance to next character.
    @param  y     Pointer to y location of character. Value is modified by
                  this function to advance to next character.
    @param  minx  Pointer to minimum X coordinate, passed in to AND returned
                  by this function -- this is used to incrementally build a
                  bounding rectangle for a string.
    @param  miny  Pointer to minimum Y coord, passed in AND returned.
    @param  maxx  Pointer to maximum X coord, passed in AND returned.
    @param  maxy  Pointer to maximum Y coord, passed in AND returned.
*/
/**************************************************************************/
void Adafruit_GFX::charBounds(unsigned char c, int16_t *x, int16_t *y,
                              int16_t *minx, int16_t *miny, int16_t *maxx,
                              int16_t *maxy) {

  if (gfxFont) {

    if (c == '\n') { // Newline?
      *x = 0;        // Reset x to zero, advance y by one line
      *y += textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
    } else if (c != '\r') { // Not a carriage return; is normal char
      uint8_t first = pgm_read_byte(&gfxFont->first),
              last = pgm_read_byte(&gfxFont->last);
      if ((c >= first) && (c <= last)) { // Char present in this font?
        GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c - first);
        uint8_t gw = pgm_read_byte(&glyph->width),
                gh = pgm_read_byte(&glyph->height),
                xa = pgm_read_byte(&glyph->xAdvance);
        int8_t xo = pgm_read_byte(&glyph->xOffset),
               yo = pgm_read_byte(&glyph->yOffset);
        if (wrap && ((*x + (((int16_t)xo + gw) * textsize_x)) > _width)) {
          *x = 0; // Reset x to zero, advance y by one line
          *y += textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        }
        int16_t tsx = (int16_t)textsize_x, tsy = (int16_t)textsize_y,
                x1 = *x + xo * tsx, y1 = *y + yo * tsy, x2 = x1 + gw * tsx - 1,
                y2 = y1 + gh * tsy - 1;
        if (x1 < *minx)
          *minx = x1;
        if (y1 < *miny)
          *miny = y1;
        if (x2 > *maxx)
          *maxx = x2;
        if (y2 > *maxy)
          *maxy = y2;
        *x += xa * tsx;
      }
    }

  } else { // Default font

    if (c == '\n') {        // Newline?
      *x = 0;               // Reset x to zero,
      *y += textsize_y * 8; // advance y one line
      // min/max x/y unchaged -- that waits for next 'normal' character
    } else if (c != '\r') { // Normal char; ignore carriage returns
      if (wrap && ((*x + textsize_x * 6) > _width)) { // Off right?
        *x = 0;                                       // Reset x to zero,
        *y += textsize_y * 8;                         // advance y one line
      }
      int x2 = *x + textsize_x * 6 - 1, // Lower-right pixel of char
          y2 = *y + textsize_y * 8 - 1;
      if (x2 > *maxx)
        *maxx = x2; // Track max x, y
      if (y2 > *maxy)
        *maxy = y2;
      if (*x < *minx)
        *minx = *x; // Track min x, y
      if (*y < *miny)
        *miny = *y;
      *x += textsize_x * 6; // Advance x one char
    }
  }
}

/**************************************************************************/
/*!
    @brief  Helper to determine size of a string with current font/size.
            Pass string and a cursor position, returns UL corner and W,H.
    @param  str  The ASCII string to measure
    @param  x    The current cursor X
    @param  y    The current cursor Y
    @param  x1   The boundary X coordinate, returned by function
    @param  y1   The boundary Y coordinate, returned by function
    @param  w    The boundary width, returned by function
    @param  h    The boundary height, returned by function
*/
/**************************************************************************/
void Adafruit_GFX::getTextBounds(const char *str, int16_t x, int16_t y,
                                 int16_t *x1, int16_t *y1, uint16_t *w,
                                 uint16_t *h) {

  uint8_t c; // Current character
  int16_t minx = 0x7FFF, miny = 0x7FFF, maxx = -1, maxy = -1; // Bound rect
  // Bound rect is intentionally initialized inverted, so 1st char sets it

  *x1 = x; // Initial position is value passed in
  *y1 = y;
  *w = *h = 0; // Initial size is zero

  while ((c = *str++)) {
    // charBounds() modifies x/y to advance for each character,
    // and min/max x/y are updated to incrementally build bounding rect.
    charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);
  }

  if (maxx >= minx) {     // If legit string bounds were found...
    *x1 = minx;           // Update x1 to least X coord,
    *w = maxx - minx + 1; // And w to bound rect width
  }
  if (maxy >= miny) { // Same for height
    *y1 = miny;
    *h = maxy - miny + 1;
  }
}

/**************************************************************************/
/*!
    @brief    Helper to determine size of a string with current font/size. Pass
   string and a cursor position, returns UL corner and W,H.
    @param    str    The ascii string to measure (as an arduino String() class)
    @param    x      The current cursor X
    @param    y      The current cursor Y
    @param    x1     The boundary X coordinate, set by function
    @param    y1     The boundary Y coordinate, set by function
    @param    w      The boundary width, set by function
    @param    h      The boundary height, set by function
*/
/**************************************************************************/
void Adafruit_GFX::getTextBounds(const String &str, int16_t x, int16_t y,
                                 int16_t *x1, int16_t *y1, uint16_t *w,
                                 uint16_t *h) {
  if (str.length() != 0) {
    getTextBounds(const_cast<char *>(str.c_str()), x, y, x1, y1, w, h);
  }
}

/**************************************************************************/
/*!
    @brief    Helper to determine size of a PROGMEM string with current
   font/size. Pass string and a cursor position, returns UL corner and W,H.
    @param    str     The flash-memory ascii string to measure
    @param    x       The current cursor X
    @param    y       The current cursor Y
    @param    x1      The boundary X coordinate, set by function
    @param    y1      The boundary Y coordinate, set by function
    @param    w      The boundary width, set by function
    @param    h      The boundary height, set by function
*/
/**************************************************************************/
void Adafruit_GFX::getTextBounds(const __FlashStringHelper *str, int16_t x,
                                 int16_t y, int16_t *x1, int16_t *y1,
                                 uint16_t *w, uint16_t *h) {
  uint8_t *s = (uint8_t *)str, c;

  *x1 = x;
  *y1 = y;
  *w = *h = 0;

  int16_t minx = _width, miny = _height, maxx = -1, maxy = -1;

  while ((c = pgm_read_byte(s++)))
    charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);

  if (maxx >= minx) {
    *x1 = minx;
    *w = maxx - minx + 1;
  }
  if (maxy >= miny) {
    *y1 = miny;
    *h = maxy - miny + 1;
  }
}

#endif
