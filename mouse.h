#ifndef __MOUSE_H
#define __MOUSE_H

#include "types.h"
#include "interrupts.h"
#include "port.h"

class MouseDriver : public InterruptHandler {
	protected:
		Port8Bit dataPort;
		Port8Bit commandPort;
		u8 buffer[3];
		u8 offset;
		u8 buttons;
	public:
		i8 x;
		i8 y;
		void Activate();
		void Deactivate();
		MouseDriver(InterruptManager *manager);
		~MouseDriver();
		virtual u32 HandleInterrupt(u32 esp);
};

#endif
