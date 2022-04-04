#ifndef __INCLUDE_GUI_BUTTON_H
#define __INCLUDE_GUI_BUTTON_H

#include <common/types.h>
#include <gui/widget.h>

namespace gui {
  class Button : public Widget {
    public:
      bool down;
      Button(WidgetCollection *parent, i32 x, i32 y, i8 w, i8 h, u8 r, u8 g, u8 b);
      ~Button();
      void OnSetup();
      void OnMouseMove(i32 oldX, i32 oldY, i32 x, i32 y);
      void OnMouseDown(i32 x, i32 y, u8 button);
      void OnMouseUp(i32 x, i32 y, u8 button);
  };
}

#endif