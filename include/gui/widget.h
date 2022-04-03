#ifndef __INCLUDE_GUI_WIDGET_H
#define __INCLUDE_GUI_WIDGET_H

#include <common/types.h>
#include <common/graphics.h>
#include <drivers/mouse.h>
#include <drivers/keyboard.h>

namespace gui {
  class WidgetCollection;
  class Widget {
    public:
      Widget(WidgetCollection *parent, i32 x, i32 y, i8 w, i8 h, u8 r, u8 g, u8 b);
      ~Widget();
      i8 x, y, w, h;
      u8 r, g, b;
      GraphicsContext *context;
      virtual void OnSetup();
      virtual void OnMouseMove(i32 oldX, i32 oldY, i32 x, i32 y);
      virtual void OnMouseDown(i32 x, i32 y, u8 button);
      virtual void OnMouseUp(i32 x, i32 y, u8 button);
      virtual void OnKeyDown(char key);
			virtual void OnKeyUp(char key);
			virtual void OnCapsLock(bool capsLock);
			virtual void OnAlt();
			virtual void OnControl();
			virtual void OnShift();
			virtual void OnMeta();
  };

  class MasterWidget;
  class WidgetCollection {
    protected:
      Widget *widgets[256];
      int numWidgets;
      int focusedWidget;
    public:
      WidgetCollection(MasterWidget *parent);
      ~WidgetCollection();
      GraphicsContext *context;
      void AddWidget(Widget *widget);
      void SetupWidgets();
      int GetWidgetFromMouse(i32 x, i32 y);
      void OnMouseMove(i32 oldX, i32 oldY, i32 x, i32 y, u8 r, u8 g, u8 b);
      void OnMouseDown(i32 x, i32 y, u8 button);
      void OnMouseUp(i32 x, i32 y, u8 button);
      void OnKeyDown(char key);
      void OnKeyUp(char key);
      void OnCapsLock(bool capsLock);
      void OnAlt();
      void OnControl();
      void OnShift();
      void OnMeta();
  };

  class MasterWidget : 
    public drivers::MouseEventHandler, 
    public drivers::KeyboardEventHandler {
    protected:
      WidgetCollection *widgetCollections[16];
      int numWidgetCollections;
      int focusedWidgetCollection;
      i32 mouseX;
      i32 mouseY;
    
    public:
      u8 backgroundR;
      u8 backgroundG;
      u8 backgroundB;
      MasterWidget(GraphicsContext *context, u8 r, u8 g, u8 b);
      ~MasterWidget();
      GraphicsContext *context;

      void Background();
      void OnMouseMove(i8 x, i8 y);
      void OnMouseButtonDown(u8 button);
      void OnMouseButtonUp(u8 button);
      void OnKeyDown(char key);
      void OnKeyUp(char key);
      void OnCapsLock(bool capsLock);
      void OnAlt();
      void OnControl();
      void OnShift();
      void OnMeta();
      void AddWidgetCollection(WidgetCollection *collection);
      void ChangeFocusedWidgetCollection(int newFocusedWidgetCollection);
  };
}

#endif