#ifndef __INCLUDE_DRIVERS_VGA_H
#define __INCLUDE_DRIVERS_VGA_H

#include <common/types.h>
#include <hardware/interrupts.h>
#include <hardware/port.h>
#include <drivers/driver.h>

namespace drivers {
  class VideoGraphicsArray {
    protected:
      hardware::Port8Bit miscPort;
      hardware::Port8Bit crtcIndexPort;
      hardware::Port8Bit crtcDataPort;
      hardware::Port8Bit sequencerIndexPort;
      hardware::Port8Bit sequencerDataPort;
      hardware::Port8Bit graphicsControllerIndexPort;
      hardware::Port8Bit graphicsControllerDataPort;
      hardware::Port8Bit attributeControllerIndexPort;
      hardware::Port8Bit attributeControllerReadPort;
      hardware::Port8Bit attributeControllerWritePort;
      hardware::Port8Bit attributeControllerResetPort;
      void WriteRegisters(u8* registers);
      u8 *GetFrameBufferSegment();
      virtual u8 GetColorIndex(u32 r, u32 g, u32 b);
    public:
      VideoGraphicsArray();
      ~VideoGraphicsArray();
      virtual bool SupportsMode(u32 width, u32 height, u32 colorDepth);
      virtual bool SetMode(u32 width, u32 height, u32 colorDepth);
      virtual void PutPixel(u32 x, u32 y, u32 r, u32 g, u32 b);
      virtual void PutPixel(u32 x, u32 y, u8 colorCode);
  };
}

#endif