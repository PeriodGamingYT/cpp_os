#include <gui/rectangle.h>

using namespace gui;

Rectangle::Rectangle(WidgetCollection *parent, i16 x, i16 y, i8 w, i8 h, u8 r, u8 g, u8 b) 
  : Widget(parent, x, y, w, h, r, g, b) {}
Rectangle::~Rectangle() {}

void Rectangle::OnSetup() {
  context->FillRectangle(x, y, w, h, r, g, b);
}

void Rectangle::OnMouseMove(i16 oldX, i16 oldY, i16 x, i16 y) {
  context->PutPixel(oldX, oldY, r, g, b);
}