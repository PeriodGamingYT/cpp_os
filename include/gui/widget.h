#ifndef __INCLUDE_GUI_WIDGET_H
#define __INCLUDE_GUI_WIDGET_H

#include <common/types.h>
#include <common/graphics.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

namespace gui {
  class Widget : public drivers::KeyboardEventHandler {
    protected:
      Widget *parent;
      i32 x;
      i32 y;
      i32 width;
      i32 height;
      u8 r;
      u8 g;
      u8 b;
      bool focusable;

    public:
      Widget(Widget *parent, i32 x, i32 y, i32 width, i32 height, u8 r, u8 g, u8 b);
      ~Widget();
      bool ContainsCoordinate(i32 x, i32 y);
      virtual void GetFocus(Widget *widget);
      virtual void ModelToScreen(i32 &x, i32 &y);
      virtual void Draw(GraphicsContext *graphicsContext);
      virtual void OnMouseDown(i32 x, i32 y, u8 button);
      virtual void OnMouseUp(i32 x, i32 y, u8 button);
      virtual void OnMouseMove(i8 oldX, i8 oldY, i8 x, i8 y);
      virtual void OnKeyDown(char key);
			virtual void OnKeyUp(char key);
			virtual void OnDelete();
			virtual void OnCapsLock(bool capsLock);
			virtual void OnEscape();
			virtual void OnAlt();
			virtual void OnControl();
			virtual void OnShift();
			virtual void OnMeta();
  };

  class CompositeWidget : public Widget {
    protected:
      Widget *children[256];
      int numChildren;
      Widget *focusedChild;

    public:
      CompositeWidget(Widget *parent, i32 x, i32 y, i32 width, i32 height, u8 r, u8 g, u8 b);
      ~CompositeWidget();
      virtual void GetFocus(Widget *widget);
      virtual void ModelToScreen(i32 &x, i32 &y);
      virtual void Draw(GraphicsContext *graphicsContext);
      virtual bool AddChild(Widget *child);
      virtual void OnMouseDown(i32 x, i32 y, u8 button);
      virtual void OnMouseUp(i32 x, i32 y, u8 button);
      virtual void OnMouseMove(i8 oldX, i8 oldY, i8 x, i8 y);
      virtual void OnKeyDown(char key);
			virtual void OnKeyUp(char key);
			virtual void OnDelete();
			virtual void OnCapsLock(bool capsLock);
			virtual void OnEscape();
			virtual void OnAlt();
			virtual void OnControl();
			virtual void OnShift();
			virtual void OnMeta();
  };
}

#endif