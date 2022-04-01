#ifndef __INCLUDE_DRIVERS_MOUSE_H
#define __INCLUDE_DRIVERS_MOUSE_H

#include <common/types.h>
#include <hardware/interrupts.h>
#include <hardware/port.h>
#include <drivers/driver.h>

namespace drivers {
	class MouseEventHandler {
		public:
			MouseEventHandler();
			virtual void OnMouseMove(i8 x, i8 y);
			virtual void OnMouseButtonDown(u8 button);
			virtual void OnMouseButtonUp(u8 button);
			virtual void OnMouseSetup();
	};

	class MouseDriver : public hardware::InterruptHandler, public Driver {
		protected:
			MouseEventHandler *handler;
			hardware::Port8Bit dataPort;
			hardware::Port8Bit commandPort;
			u8 buffer[3];
			u8 offset;
			u8 buttons;
		public:
			MouseDriver(hardware::InterruptManager *, MouseEventHandler *);
			~MouseDriver();
			virtual u32 HandleInterrupt(u32 esp);
			virtual void Activate();
	};
}

#endif
