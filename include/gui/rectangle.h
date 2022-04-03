#ifndef __INCLUDE_GUI_RECTANGLE_H
#define __INCLUDE_GUI_RECTANGLE_H

#include <common/types.h>
#include <gui/widget.h>

namespace gui {
  class Rectangle : public Widget {
    public:
      Rectangle(WidgetCollection *parent, i32 x, i32 y, i8 w, i8 h, u8 r, u8 g, u8 b);
      ~Rectangle();
      void OnSetup();
      void OnMouseMove(i32 oldX, i32 oldY, i32 x, i32 y);
  };
}

#endif