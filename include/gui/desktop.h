#ifndef __INCLUDE_GUI_DESKTOP_H
#define __INCLUDE_GUI_DESKTOP_H

#include <common/types.h>
#include <drivers/mouse.h>
#include <gui/widget.h>

namespace gui {
  class Desktop 
  : public CompositeWidget, 
    public drivers::MouseEventHandler {
    protected:
      u32 mouseX;
      u32 mouseY;
    
    public:
      Desktop(i32 width, i32 height, u8 r, u8 g, u8 b);
      ~Desktop();

      void Draw(GraphicsContext *graphicsContext);
      void OnMouseMove(i8 x, i8 y);
			void OnMouseButtonDown(u8 button);
			void OnMouseButtonUp(u8 button);
			void OnMouseSetup();
  };
}

#endif