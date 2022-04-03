#ifndef __INCLUDE_DRIVERS_KEYBOARD_H
#define __INCLUDE_DRIVERS_KEYBOARD_H

#include <common/types.h>
#include <hardware/interrupts.h>
#include <hardware/port.h>
#include <drivers/driver.h>

namespace drivers {
	enum keys {
		ESCAPE = 27, 
		DELETE = 127
	};

	class KeyboardEventHandler {
		public:
			KeyboardEventHandler();
			virtual void OnKeyDown(char);
			virtual void OnKeyUp(char);
			virtual void OnCapsLock(bool);
			virtual void OnAlt();
			virtual void OnControl();
			virtual void OnShift();
			virtual void OnMeta();
	};

	class KeyboardDriver : public hardware::InterruptHandler, public Driver {
		protected:
			hardware::Port8Bit dataPort;
			hardware::Port8Bit commandPort;
			KeyboardEventHandler *handler;
		public:
			bool meta;
			bool control;
			bool alt;
			bool shift;
			bool capsLock;
			char key;
			bool IsPrintable();
			KeyboardDriver(hardware::InterruptManager *manager, KeyboardEventHandler *handler);
			~KeyboardDriver();
			virtual u32 HandleInterrupt(u32 esp);
			virtual void Activate();
	};
}

#endif
