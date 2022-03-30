#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "types.h"
#include "interrupts.h"
#include "port.h"
#include "driver.h"

class KeyboardEventHandler {
	public:
		KeyboardEventHandler();
		virtual void OnKeyDown(char);
		virtual void OnKeyUp(char);
		virtual void OnDelete();
		virtual void OnCapsLock(bool);
		virtual void OnEscape();
		virtual void OnAlt();
		virtual void OnControl();
		virtual void OnShift();
		virtual void OnMeta();
};

class KeyboardDriver : public InterruptHandler, public Driver {
	protected:
		Port8Bit dataPort;
		Port8Bit commandPort;
		KeyboardEventHandler *handler;
	public:
		bool meta;
		bool control;
		bool alt;
		bool shift;
		bool capsLock;
		bool capsLockDown;
		bool backspaceDown;
		bool escapeDown;
		char key;
		bool IsPrintable();
		KeyboardDriver(InterruptManager *manager, KeyboardEventHandler *handler);
		~KeyboardDriver();
		virtual u32 HandleInterrupt(u32 esp);
		virtual void Activate();
};

#endif
