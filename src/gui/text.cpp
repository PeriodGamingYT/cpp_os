#include <gui/text.h>

using namespace gui;

Text::Text(WidgetCollection *parent, i16 x, i16 y, u8 r, u8 g, u8 b, char *str)
  : Widget(parent, x, y, 0, 0, r, g, b) {
  this->str = str;
  int stringSize;
  for(stringSize = 0; str[stringSize] != '\0'; stringSize++);

  this->stringSize = stringSize;
  this->w = stringSize * 8;
  this->h = 8;
}

Text::~Text() {}

void Text::OnSetup() {
  context->PutText(x, y, this->str, r, g, b);
}

void Text::OnMouseMove(i16 oldX, i16 oldY, i16 x, i16 y) {
  context->PutTextPixel(this->x, this->y, this->str, this->stringSize, r, g, b, oldX, oldY);
}