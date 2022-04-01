#include <gui/window.h>

using namespace gui;

Window::Window(Widget *parent, i32 x, i32 y, i32 width, i32 height, u8 r, u8 g, u8 b) 
  : CompositeWidget(parent, x, y, width, height, r, g, b) {
  dragging = false;
}

Window::~Window() {

}

void Window::OnMouseDown(i32 x, i32 y, u8 button) {
  dragging = button == 1;
  CompositeWidget::OnMouseDown(x, y, button);
}

void Window::OnMouseUp(i32 x, i32 y, u8 button) {
  dragging = false;
  CompositeWidget::OnMouseUp(x, y, button);
}

void Window::OnMouseMove(i8 oldX, i8 oldY, i8 x, i8 y) {
  if(dragging) {
    this->x += x - oldX;
    this->y += y - oldY;
  }

  CompositeWidget::OnMouseMove(oldX, oldY, x, y);
}