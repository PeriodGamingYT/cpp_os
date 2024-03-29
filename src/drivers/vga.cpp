#include <drivers/vga.h>

using namespace drivers;

void VideoGraphicsArray::WriteRegisters(u8* registers) {
  miscPort.Write(*(registers++));

  for(int i = 0; i < 5; i++) {
    sequencerIndexPort.Write(i);
    sequencerDataPort.Write(*(registers++));
  }

  crtcIndexPort.Write(0x03);
  crtcDataPort.Write(crtcDataPort.Read() | 0x80);
  crtcIndexPort.Write(0x11);
  crtcDataPort.Write(crtcDataPort.Read() & ~0x80);
  registers[0x03] |= 0x80;
  registers[0x11] &= ~0x80;
  for(u8 i = 0; i < 25; i++) {
    crtcIndexPort.Write(i);
    crtcDataPort.Write(*(registers++));
  }

  for(u8 i = 0; i < 9; i++) {
    graphicsControllerIndexPort.Write(i);
    graphicsControllerDataPort.Write(*(registers++));
  }

  for(u8 i = 0; i < 21; i++) {
    attributeControllerResetPort.Read();
    attributeControllerIndexPort.Write(i);
    attributeControllerWritePort.Write(*(registers++));
  }

  attributeControllerResetPort.Read();
  attributeControllerIndexPort.Write(0x20);
}

void printfHex(u8);
u8 *VideoGraphicsArray::GetFrameBufferSegment() {
  graphicsControllerIndexPort.Write(0x06);
  u8 segmentNum = (graphicsControllerDataPort.Read() >> 2) & 0x03;
  printfHex(segmentNum);
  switch(segmentNum) {
    default:
    case 0: return (u8*)0x00000; // 0x00000.
    case 1: return (u8*)0xA0000; // 0xA0000.
    case 2: return (u8*)0xB0000; // 0xB0000.
    case 3: return (u8*)0xB8000; // 0xB8000.
  }
}

void VideoGraphicsArray::PutPixel(i16 x, i16 y, u8 colorCode) {
  if(x < 0 || y < 0 || x > 320 || y > 200) {
    return;
  }

  u8 *pixelAddress = GetFrameBufferSegment() + ((320 * y) + x);
  *pixelAddress = colorCode;
}

u8 VideoGraphicsArray::GetPixelCode(i16 x, i16 y) {
  if(x < 0 || y < 0 || x > 320 || y > 200) {
    return 0;
  }

  u8 *pixelAddress = GetFrameBufferSegment() + ((320 * y) + x);
  return *pixelAddress;
}

u8 VideoGraphicsArray::GetPixelR(u8 colorCode) {
  return vgaToRgb[colorCode][0];
}

u8 VideoGraphicsArray::GetPixelG(u8 colorCode) {
  return vgaToRgb[colorCode][1];
}

u8 VideoGraphicsArray::GetPixelB(u8 colorCode) {
  return vgaToRgb[colorCode][2];
}

// If someone can replace this please do so.
u8 VideoGraphicsArray::GetColorIndex(u8 r, u8 g, u8 b) {
  for(int i = 0; i < 256; i++) {
    if(
      vgaToRgb[i][0] == r &&
      vgaToRgb[i][1] == g &&
      vgaToRgb[i][2] == b
    ) {
      return i;
    }
  }

  return 0x00;
}

VideoGraphicsArray::VideoGraphicsArray() 
  : miscPort(0x3c2),
    crtcIndexPort(0x3d4),
    crtcDataPort(0x3d5),
    sequencerIndexPort(0x3c4),
    sequencerDataPort(0x3c5),
    graphicsControllerIndexPort(0x3ce),
    graphicsControllerDataPort(0x3cf),
    attributeControllerIndexPort(0x3c0),
    attributeControllerReadPort(0x3c1),
    attributeControllerWritePort(0x3c0),
    attributeControllerResetPort(0x3da) {}
VideoGraphicsArray::~VideoGraphicsArray() {}

bool VideoGraphicsArray::SupportsMode(u32 width, u32 height, u32 colorDepth) {
  return width == 320 && height == 200 && colorDepth == 8;
}

bool VideoGraphicsArray::SetMode(u32 width, u32 height, u32 colorDepth) {
  if(!SupportsMode(width, height, colorDepth)) {
    return false;
  }

  unsigned char g_320x200x256[] = { // https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
  /* MISC */
    0x63,
  /* SEQ */
    0x03, 0x01, 0x0F, 0x00, 0x0E,
  /* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
    0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
    0xFF,
  /* GC */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
    0xFF,
  /* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x41, 0x00, 0x0F, 0x00,	0x00
  };

  WriteRegisters(g_320x200x256);
  return true;
}

void VideoGraphicsArray::PutPixel(i16 x, i16 y, u8 r, u8 g, u8 b) {
  PutPixel(x, y, GetColorIndex(r, g, b));
}

void VideoGraphicsArray::FillRectangle(i16 x, i16 y, i16 width, i16 height, u8 r, u8 g, u8 b) {
  if(x < 0 || y < 0 || x + width > 320 || y + height > 200) {
    return;
  }

  for(i16 Y = 0; Y < height; Y++) {
    for(i16 X = 0; X < width; X++) {
      PutPixel(X + x, Y + y, r, g, b);
    }
  }
}



void VideoGraphicsArray::PutTextLetter(i16 X, i16 Y, const char letter, u8 r, u8 g, u8 b) {
  if(X < 0 || Y < 0 || X > 320 || Y > 200 || letter > 128) {
    return;
  }

  for(int y = 0; y < 8; y++) {
    for(int x = 0; x < 8; x++) {
      if((VideoGraphicsArray::letters[letter][y] >> x) & 1 == 1) {
        PutPixel(x + X, y + Y, r, g, b);
      }
    }
  }
}

void VideoGraphicsArray::PutText(i16 x, i16 y, const char *str, u8 r, u8 g, u8 b) {
  for(int i = 0; str[i] != '\0'; i++) {
    VideoGraphicsArray::PutTextLetter(x + (i * 8), y, str[i], r, g, b);
  }
}

void VideoGraphicsArray::PutTextPixel(i16 x, i16 y, const char *str, int stringSize, u8 r, u8 g, u8 b, i16 brokenX, i16 brokenY) {
  if(
      brokenX >= x &&
      brokenY >= y &&
      brokenX <= x + (stringSize * 8) &&
      brokenY <= y + 8 &&
      str[(brokenX - x) / 8] <= 128 &&
      (VideoGraphicsArray::letters[str[(brokenX - x) / 8]][brokenY - y] >> ((brokenX - x) % 8)) & 1 == 1
    ) {
    PutPixel(brokenX, brokenY, r, g, b);
  }
}