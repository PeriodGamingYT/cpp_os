#ifndef __INCLUDE_GUI_WINDOW_H
#define __INCLUDE_GUI_WINDOW_H

#include <common/types.h>
#include <gui/widget.h>

namespace gui {
  class Window : public CompositeWidget {
    protected:
      bool dragging;

    public:
      Window(Widget *parent, i32 x, i32 y, i32 width, i32 height, u8 r, u8 g, u8 b);
      ~Window();
      void OnMouseDown(i32 x, i32 y, u8 button);
      void OnMouseUp(i32 x, i32 y, u8 button);
      void OnMouseMove(i8 oldX, i8 oldY, i8 x, i8 y);
  };
}

#endif