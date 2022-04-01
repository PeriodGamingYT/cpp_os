#include <gui/widget.h>

using namespace gui;

Widget::Widget(Widget *parent, i32 x, i32 y, i32 width, i32 height, u8 r, u8 g, u8 b) {
  this->parent = parent;
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->r = r;
  this->g = g;
  this->b = b;
  this->focusable = true;
}

Widget::~Widget() {}

void Widget::GetFocus(Widget *widget) {
  if(parent != 0) {
    parent->GetFocus(widget);
  }
}

void Widget::ModelToScreen(i32 &x, i32 &y) {
  if(parent != 0) {
    parent->ModelToScreen(x, y);
  }

  x += this->x;
  y += this->y;
}

void Widget::Draw(GraphicsContext *graphicsContext) {
  i32 X = x;
  i32 Y = y;
  ModelToScreen(X, Y);
  graphicsContext->FillRectangle(X, Y, width, height, r, g, b);
}

void Widget::OnMouseDown(i32 x, i32 y, u8 button) {
  if(focusable) {
    GetFocus(this);
  }
}

bool Widget::ContainsCoordinate(i32 x, i32 y) {
  return 
    this->x <= x &&
    this->y <= y &&
    this->width + this->x >= x &&
    this->height + this->y >= y;
}

void Widget::OnMouseUp(i32 x, i32 y, u8 button) {}
void Widget::OnMouseMove(i8 oldX, i8 oldY, i8 x, i8 y) {}
void Widget::OnKeyDown(char key) {}
void Widget::OnKeyUp(char key) {}
void Widget::OnDelete() {}
void Widget::OnCapsLock(bool capsLock) {}
void Widget::OnEscape() {}
void Widget::OnAlt() {}
void Widget::OnControl() {}
void Widget::OnShift() {}
void Widget::OnMeta() {}

CompositeWidget::CompositeWidget(Widget *parent, i32 x, i32 y, i32 width, i32 height, u8 r, u8 g, u8 b) 
  : Widget(parent, x, y, width, height, r, g, b) {
  focusedChild = 0;
  numChildren = 0;
}

CompositeWidget::~CompositeWidget() {}

bool CompositeWidget::AddChild(Widget *child) {
  if(numChildren >= 255) {
    return false;
  }

  children[numChildren] = child;
  numChildren++;
  return true;
}

void CompositeWidget::GetFocus(Widget *widget) {
  this->focusedChild = widget;
  if(parent != 0) {
    parent->GetFocus(this);
  }
}

void CompositeWidget::ModelToScreen(i32 &x, i32 &y) {}

void CompositeWidget::Draw(GraphicsContext *graphicsContext) {
  Widget::Draw(graphicsContext);
  for(i32 i = numChildren - 1; i >= 0; i--) {
    children[i]->Draw(graphicsContext);
  }
}

void CompositeWidget::OnMouseDown(i32 x, i32 y, u8 button) {
  for(i32 i = 0; i < numChildren; i++) {
    if(children[i]->ContainsCoordinate(x - this->x, y - this->y)) {
      children[i]->OnMouseDown(x - this->x, y - this->y, button);
      break;
    }
  }
}

void CompositeWidget::OnMouseUp(i32 x, i32 y, u8 button) {
  for(i32 i = 0; i < numChildren; i++) {
    if(children[i]->ContainsCoordinate(x - this->x, y - this->y)) {
      children[i]->OnMouseUp(x - this->x, y - this->y, button);
      break;
    }
  }
}

void CompositeWidget::OnMouseMove(i8 oldX, i8 oldY, i8 x, i8 y) {
  i32 firstChild = -1;
  for(i32 i = 0; i < numChildren; i++) {
    if(children[i]->ContainsCoordinate(oldX - this->x, oldY - this->y)) {
      children[i]->OnMouseMove(oldX - this->x, oldY - this->y, x - this->x, y - this->y);
      firstChild = i;
      break;
    }
  }

  for(i32 i = 0; i < numChildren; i++) {
    if(children[i]->ContainsCoordinate(x - this->x, y - this->y)) {
      if(firstChild != i) {
        children[i]->OnMouseMove(oldX - this->x, oldY - this->y, x - this->x, y - this->y);
      }
      
      break;
    }
  }
}


void CompositeWidget::OnKeyDown(char key) {
  if(focusedChild != 0) {
    focusedChild->OnKeyDown(key);
  }
}

void CompositeWidget::OnKeyUp(char key) {
  if(focusedChild != 0) {
    focusedChild->OnKeyUp(key);
  }
}

void CompositeWidget::OnDelete() {}
void CompositeWidget::OnCapsLock(bool capsLock) {}
void CompositeWidget::OnEscape() {}
void CompositeWidget::OnAlt() {}
void CompositeWidget::OnControl() {}
void CompositeWidget::OnShift() {}
void CompositeWidget::OnMeta() {}