#include <gui/desktop.h>

using namespace gui;

Desktop::Desktop(i32 width, i32 height, u8 r, u8 g, u8 b) 
  : CompositeWidget(0, 0, 0, width, height, r, g, b),
    MouseEventHandler() {
  mouseX = width / 2;
  mouseY = height / 2;
}

Desktop::~Desktop() {}

void Desktop::Draw(GraphicsContext *graphicsContext) {
  CompositeWidget::Draw(graphicsContext);
  for(i32 i = 0; i < 4; i++) {
    graphicsContext->PutPixel(mouseX-i, mouseY, 0xFF, 0xFF, 0xFF);
    graphicsContext->PutPixel(mouseX+i, mouseY, 0xFF, 0xFF, 0xFF);
    graphicsContext->PutPixel(mouseX, mouseY-i, 0xFF, 0xFF, 0xFF);
    graphicsContext->PutPixel(mouseX, mouseY+i, 0xFF, 0xFF, 0xFF);
  }
}

void Desktop::OnMouseMove(i8 x, i8 y) {
  // The mouse is kinda fast so this is going to slow it down.
  x /= 4;
  y /= 4;
  i32 newMouseX = mouseX + x;
  i32 newMouseY = mouseY + y;
  if(newMouseX < 0) newMouseX = 0;
  if(newMouseX >= width) newMouseX = width - 1;
  if(newMouseY < 0) newMouseY = 0;
  if(newMouseY >= height) newMouseY = height - 1;

  CompositeWidget::OnMouseMove(mouseX, mouseY, newMouseX, newMouseY);
  mouseX = newMouseX;
  mouseY = newMouseY;
}

void Desktop::OnMouseButtonDown(u8 button) {
  CompositeWidget::OnMouseDown(mouseX, mouseY, button);
}

void Desktop::OnMouseButtonUp(u8 button) {
  CompositeWidget::OnMouseUp(mouseX, mouseY, button);
}

void Desktop::OnMouseSetup() {}