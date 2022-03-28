#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "types.h"
#include "interrupts.h"
#include "port.h"

class KeyboardDriver : public InterruptHandler {
	protected:
		Port8Bit dataPort;
		Port8Bit commandPort;
	public:
		KeyboardDriver(InterruptManager *manager);
		~KeyboardDriver();
		virtual u32 HandleInterrupt(u32 esp);
};

#endif
