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
		bool meta;
		bool control;
		bool alt;
		bool shift;
		bool capsLock;
		bool capsLockDown;
		bool backspaceDown;
		char key;
		bool IsPrintable();
		KeyboardDriver(InterruptManager *manager);
		~KeyboardDriver();
		virtual u32 HandleInterrupt(u32 esp);
};

#endif
