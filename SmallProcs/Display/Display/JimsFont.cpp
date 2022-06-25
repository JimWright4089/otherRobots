#include "JimsFont.h"

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

JimsFont::JimsFont(LCD320240_4WSPI* myTFT)
{
  mTFT = myTFT;
}

void JimsFont::setFont(const GFXfont *f) 
{
  mGfxFont = (GFXfont *)f;
}

void JimsFont::setLocation(int x, int y) 
{
  mCursorX = x;
  mCursorY = y;
}

void JimsFont::setTextColor(ILI9341_color_16_t color)
{
  mColor = color;
}

void JimsFont::drawString(const char *c)
{
  unsigned char *ptr = (unsigned char *)c;
  while(0 != *ptr)
  {
    drawChar(*ptr);
    ptr++;  
  }
}


void JimsFont::drawChar(unsigned char c) 
{
  if(NULL == mTFT)
  {
    return;
  }
  
  c -= (uint8_t)pgm_read_byte(&mGfxFont->first);
  GFXglyph *glyph = pgm_read_glyph_ptr(mGfxFont, c);
  uint8_t *bitmap = pgm_read_bitmap_ptr(mGfxFont);
  uint16_t top = 240 - (mGfxFont->yAdvance/2);

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
        
          mTFT->pixel(top-(mCursorY + yo + yy), mCursorX + xo + xx, (color_t)&mColor);
      }
      bits <<= 1;
    }
  }
  mCursorX += w+3;
}
