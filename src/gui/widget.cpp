#include <gui/widget.h>

using namespace gui;

MasterWidget::MasterWidget(GraphicsContext *context, u8 r, u8 g, u8 b) 
  : drivers::KeyboardEventHandler(),
    drivers::MouseEventHandler() {
  this->context = context;
  backgroundR = r;
  backgroundG = g;
  backgroundB = b;
  numWidgetCollections = 0;
  focusedWidgetCollection = -1;
  mouseX = 320 / 2;
  mouseY = 200 / 2;
}

MasterWidget::~MasterWidget() {}

void MasterWidget::AddWidgetCollection(WidgetCollection *collection) {
  if(numWidgetCollections >= 15) {
    return;
  }

  widgetCollections[numWidgetCollections] = collection;
  numWidgetCollections++;
}

void MasterWidget::ChangeFocusedWidgetCollection(int newFocusedWidgetCollection) {
  if(newFocusedWidgetCollection > numWidgetCollections) {
    return;
  }

  focusedWidgetCollection = newFocusedWidgetCollection;
  context->FillRectangle(0, 0, 320, 200, backgroundR, backgroundG, backgroundB);
  widgetCollections[focusedWidgetCollection]->SetupWidgets();
}

void MasterWidget::OnMouseButtonDown(u8 button) {
  if(focusedWidgetCollection == -1) return;
  widgetCollections[focusedWidgetCollection]->OnMouseDown(mouseX, mouseY, button);
}

void MasterWidget::OnMouseButtonUp(u8 button) {
  if(focusedWidgetCollection == -1) return;
  widgetCollections[focusedWidgetCollection]->OnMouseUp(mouseX, mouseY, button);
}

void MasterWidget::OnMouseMove(i8 x, i8 y) {
  if(focusedWidgetCollection == -1) return;
  i32 newMouseX = mouseX + x;
  i32 newMouseY = mouseY + y;
  newMouseX = newMouseX < 0 ? 0 : newMouseX;
  newMouseY = newMouseY < 0 ? 0 : newMouseY;
  newMouseX = newMouseX > 320 ? 320 : newMouseX;
  newMouseY = newMouseY > 200 ? 200 : newMouseY;
  widgetCollections[focusedWidgetCollection]->OnMouseMove(
    mouseX, 
    mouseY, 
    newMouseX, 
    newMouseY, 
    backgroundR, 
    backgroundG, 
    backgroundB
  );

  mouseX = newMouseX;
  mouseY = newMouseY;
}

void MasterWidget::OnKeyDown(char key) {
  if(focusedWidgetCollection == -1) return;
  widgetCollections[focusedWidgetCollection]->OnKeyDown(key);
}

void MasterWidget::OnKeyUp(char key) {
  if(focusedWidgetCollection == -1) return;
  widgetCollections[focusedWidgetCollection]->OnKeyUp(key);
}

void MasterWidget::OnCapsLock(bool capsLock) {
  if(focusedWidgetCollection == -1) return;
  widgetCollections[focusedWidgetCollection]->OnCapsLock(capsLock);
}

void MasterWidget::OnAlt() {
  if(focusedWidgetCollection == -1) return;
  widgetCollections[focusedWidgetCollection]->OnAlt();
}

void MasterWidget::OnControl() {
  if(focusedWidgetCollection == -1) return;
  widgetCollections[focusedWidgetCollection]->OnControl();
}

void MasterWidget::OnShift() {
  if(focusedWidgetCollection == -1) return;
  widgetCollections[focusedWidgetCollection]->OnShift();
}

void MasterWidget::OnMeta() {
  if(focusedWidgetCollection == -1) return;
  widgetCollections[focusedWidgetCollection]->OnMeta();
}

WidgetCollection::WidgetCollection(MasterWidget *parent) {
  if(parent != 0) {
    context = parent->context;
    this->numWidgets = 0;
    this->focusedWidget = -1;
    parent->AddWidgetCollection(this);
  }
}

WidgetCollection::~WidgetCollection() {}

void WidgetCollection::AddWidget(Widget *widget) {
  if(numWidgets >= 255) {
    return;
  }

  widgets[numWidgets] = widget;
  numWidgets++;
}

void WidgetCollection::SetupWidgets() {
  for(int i = 0; i < numWidgets; i++) {
    widgets[i]->OnSetup();
  }
}

int WidgetCollection::GetWidgetFromMouse(i32 x, i32 y) {
  for(int i = 0; i < numWidgets; i++) {
    if(
      widgets[i]->x <= x &&
      widgets[i]->y <= y &&
      widgets[i]->w + widgets[i]->x > x &&
      widgets[i]->h + widgets[i]->y > y
    ) {
      return i;
    }
  }

  return -1;
}

void WidgetCollection::OnMouseMove(i32 oldX, i32 oldY, i32 x, i32 y, u8 r, u8 g, u8 b) {
  int index = GetWidgetFromMouse(oldX, oldY);
  context->PutPixel(oldX, oldY, r, g, b);
  if(index != -1) widgets[index]->OnMouseMove(oldX, oldY, x, y);
  context->PutPixel(x, y, 0xFF, 0xFF, 0xFF);
}

void WidgetCollection::OnMouseDown(i32 x, i32 y, u8 button) {
  int index = GetWidgetFromMouse(x, y);
  if(index != -1) widgets[index]->OnMouseDown(x, y, button);
}

void WidgetCollection::OnMouseUp(i32 x, i32 y, u8 button) {
  int index = GetWidgetFromMouse(x, y);
  if(index != -1) widgets[index]->OnMouseUp(x, y, button);
}

void WidgetCollection::OnKeyDown(char key) {
  if(focusedWidget == -1) return;
  widgets[focusedWidget]->OnKeyDown(key);
}

void WidgetCollection::OnKeyUp(char key) {
  if(focusedWidget == -1) return;
  widgets[focusedWidget]->OnKeyUp(key);
}

void WidgetCollection::OnCapsLock(bool capsLock) {
  if(focusedWidget == -1) return;
  widgets[focusedWidget]->OnCapsLock(capsLock);
}

void WidgetCollection::OnAlt() {
  if(focusedWidget == -1) return;
  widgets[focusedWidget]->OnAlt();
}

void WidgetCollection::OnControl() {
  if(focusedWidget == -1) return;
  widgets[focusedWidget]->OnControl();
}

void WidgetCollection::OnShift() {
  if(focusedWidget == -1) return;
  widgets[focusedWidget]->OnShift();
}

void WidgetCollection::OnMeta() {
  if(focusedWidget == -1) return;
  widgets[focusedWidget]->OnMeta();
}

Widget::Widget(WidgetCollection *parent, i32 x, i32 y, i8 w, i8 h, u8 r, u8 g, u8 b) {
  if(parent != 0) {
    context = parent->context;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->r = r;
    this->g = g;
    this->b = b;
    parent->AddWidget(this);
  }
}

Widget::~Widget() {}
void Widget::OnSetup() {}
void Widget::OnMouseMove(i32 oldX, i32 oldY, i32 x, i32 y) {}
void Widget::OnMouseDown(i32 x, i32 y, u8 button) {}
void Widget::OnMouseUp(i32 x, i32 y, u8 button) {}
void Widget::OnKeyDown(char key) {}
void Widget::OnKeyUp(char key) {}
void Widget::OnCapsLock(bool capsLock) {}
void Widget::OnAlt() {}
void Widget::OnControl() {}
void Widget::OnShift() {}
void Widget::OnMeta() {}