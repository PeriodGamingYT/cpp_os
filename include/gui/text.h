#ifndef __INCLUDE_GUI_TEXT_H
#define __INCLUDE_GUI_TEXT_H

#include <common/types.h>
#include <common/graphics.h>
#include <gui/widget.h>

namespace gui {
  class Text : public Widget {
    protected:
      char *str;
      int stringSize;

    public:
      Text(WidgetCollection *parent, i16 x, i16 y, u8 r, u8 g, u8 b, char *str);
      ~Text();
      void OnSetup();
      void OnMouseMove(i16 oldX, i16 oldY, i16 x, i16 y);
  };
}

#endif