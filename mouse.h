#ifndef __MOUSE_H
#define __MOUSE_H

#include "types.h"
#include "interrupts.h"
#include "port.h"
#include "driver.h"

class MouseEventHandler {
	public:
		MouseEventHandler();
		virtual void OnMouseMove(i8 oldX, i8 oldY, i8 x, i8 y);
		virtual void OnMouseButtonDown(u8 button);
		virtual void OnMouseButtonUp(u8 button);
		virtual void OnMouseSetup();
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
