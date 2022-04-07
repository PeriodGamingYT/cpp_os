#ifndef __INCLUDE_GUI_FONT_H
#define __INCLUDE_GUI_FONT_H

#include <common/types.h>
#include <common/graphics.h>
#include <drivers/vga.h>

namespace gui::text {
  extern unsigned char letters[95][13];
  void AddLetter(i32 x, i32 y, const char letter, GraphicsContext *graphicsContext, u8 r, u8 g, u8 b);
}

#endif