#ifndef __INCLUDE_GUI_BUTTON_H
#define __INCLUDE_GUI_BUTTON_H

#include <common/types.h>
#include <gui/widget.h>

namespace gui {
  class Button : public Widget {
    public:
      bool down;
      Button(WidgetCollection *parent, i16 x, i16 y, i16 w, i16 h, u8 r, u8 g, u8 b);
      ~Button();
      void OnSetup();
      void OnMouseMove(i16 oldX, i16 oldY, i16 x, i16 y);
      void OnMouseDown(i16 x, i16 y, u8 button);
      void OnMouseUp(i16 x, i16 y, u8 button);
      virtual void OnButtonDown();
      virtual void OnButtonUp();
      virtual void OnButtonHover(i16 x, i16 y);
  };
}

#endif