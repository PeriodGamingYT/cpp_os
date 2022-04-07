#ifndef __INCLUDE_GUI_FONT_H
#define __INCLUDE_GUI_FONT_H

#include <common/types.h>
#include <common/graphics.h>
#include <drivers/vga.h>

namespace gui::text {
  extern unsigned char letters[128][8];
  void AddLetter(i32 X, i32 Y, const char letter, GraphicsContext *graphicsContext, u8 r, u8 g, u8 b);
  void AddText(i32 X, i32 Y, const char *string, GraphicsContext *graphicsContext, u8 r, u8 g, u8 b);
}

#endif