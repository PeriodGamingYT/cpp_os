#ifndef __MOUSE_H
#define __MOUSE_H

#include "types.h"
#include "interrupts.h"
#include "port.h"
#include "driver.h"

class MouseEventHandler {
	public:
		MouseEventHandler();
		virtual void OnMouseBeforeMove(i8 x, i8 y);
		virtual void OnMouseAfterMove(i8 x, i8 y);
		virtual void OnMouseButtonDown(u8 button);
};

class MouseDriver : public InterruptHandler, public Driver {
	protected:
		MouseEventHandler *handler;
		Port8Bit dataPort;
		Port8Bit commandPort;
		u8 buffer[3];
		u8 offset;
		u8 buttons;
	public:
		i8 x;
		i8 y;
		MouseDriver(InterruptManager *, MouseEventHandler *);
		~MouseDriver();
		virtual u32 HandleInterrupt(u32 esp);
		virtual void Activate();
};

#endif
