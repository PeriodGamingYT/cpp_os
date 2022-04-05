#include <gui/button.h>

using namespace gui;

Button::Button(WidgetCollection *parent, i16 x, i16 y, i16 w, i16 h, u8 r, u8 g, u8 b) 
  : Widget(parent, x, y, w, h, r, g, b) {
  down = false;
}

Button::~Button() {}

void Button::OnSetup() {
  if(down) {
    context->FillRectangle(x, y, w, h, ~r, ~g, ~b);
  }

  context->FillRectangle(x + down, y + down, w - (down * 2), h - (down * 2), r, g, b);
}

void Button::OnMouseMove(i16 oldX, i16 oldY, i16 x, i16 y) {
  if(down && (
      oldX == this->x || 
      oldX == w + this->x || 
      oldY == this->y || 
      oldY == h + this->y
    )) {
    context->PutPixel(oldX, oldY, ~r, ~g, ~b);
  } else {
    context->PutPixel(oldX, oldY, r, g, b);
  }
}

void Button::OnMouseDown(i16 x, i16 y, u8 button) {
  if(button == 1) {
    down = true;
    context->FillRectangle(this->x, this->y, w, 1, ~r, ~g, ~b);
    context->FillRectangle(this->x, this->y + h - 1, w, 1, ~r, ~g, ~b);
    context->FillRectangle(this->x, this->y + 1, 1, h - 2, ~r, ~g, ~b);
    context->FillRectangle(this->x + w - 1, this->y + 1, 1, h - 2, ~r, ~g, ~b);
  }
}

void Button::OnMouseUp(i16 x, i16 y, u8 button) {
  if(button == 1) {
    down = false;
    context->FillRectangle(this->x, this->y, w, 1, r, g, b);
    context->FillRectangle(this->x, this->y + h - 1, w, 1, r, g, b);
    context->FillRectangle(this->x, this->y + 1, 1, h - 2, r, g, b);
    context->FillRectangle(this->x + w - 1, this->y + 1, 1, h - 2, r, g, b);
  }
}