#include "keyboard.h"

KeyboardDriver::KeyboardDriver(InterruptManager *manager) 
	: InterruptHandler(0x21, manager),
		dataPort(0x60),
		commandPort(0x64) {
	meta = false;
	shift = false;
	capsLock = false;
	capsLockDown = false;
	backspaceDown = false;
	alt = false;
	control = false;
	key = '\0';
	while(commandPort.Read() & 0x1) {
		dataPort.Read();
	}

	commandPort.Write(0xAE); // Activate Keyboard Interrupts.
	commandPort.Write(0x20); // Give the current state of the keyboard.
	u8 status = (dataPort.Read() | 1) & ~0x10;
	commandPort.Write(0x60); // Set Keyboard State.
	dataPort.Write(status);
	dataPort.Write(0xF4);
}

KeyboardDriver::~KeyboardDriver() {}

void printf(const char *);
void printChar(char);
void printGoBack(u32);

bool KeyboardDriver::IsPrintable() {
	return !control && !alt && !meta && !shift && !capsLockDown && !backspaceDown;
}

u32 KeyboardDriver::HandleInterrupt(u32 esp) {
	u8 key = dataPort.Read();
	if(key < 0x80) {
		switch(key) {
			case 0x02: key = ('1'); break;
			case 0x03: key = ('2'); break;
			case 0x04: key = ('3'); break;
			case 0x05: key = ('4'); break;
			case 0x06: key = ('5'); break;
			case 0x07: key = ('6'); break;
			case 0x08: key = ('7'); break;
			case 0x09: key = ('8'); break;
			case 0x0A: key = ('9'); break;
			case 0x0B: key = ('0'); break;
			case 0x10: key = ('q'); break;
			case 0x11: key = ('w'); break;
			case 0x12: key = ('e'); break;
			case 0x13: key = ('r'); break;
			case 0x14: key = ('t'); break;
			case 0x15: key = ('y'); break;
			case 0x16: key = ('u'); break;
			case 0x17: key = ('i'); break;
			case 0x18: key = ('o'); break;
			case 0x19: key = ('p'); break;
			case 0x1E: key = ('a'); break;
			case 0x1F: key = ('s'); break;
			case 0x20: key = ('d'); break;
			case 0x21: key = ('f'); break;
			case 0x22: key = ('g'); break;
			case 0x23: key = ('h'); break;
			case 0x24: key = ('j'); break;
			case 0x25: key = ('k'); break;
			case 0x26: key = ('l'); break;
			case 0x2C: key = ('z'); break;
			case 0x2D: key = ('x'); break;
			case 0x2E: key = ('c'); break;
			case 0x2F: key = ('v'); break;
			case 0x30: key = ('b'); break;
			case 0x31: key = ('n'); break;
			case 0x32: key = ('m'); break;
			case 0x33: key = (','); break;
			case 0x34: key = ('.'); break;
			case 0x35: key = ('/'); break;
			case 0x2B: key = ('\\'); break;
			case 0x0C: key = ('-'); break;
			case 0x1A: key = ('['); break;
			case 0x1B: key = (']'); break;
			case 0x27: key = (';'); break;
			case 0x28: key = ('\''); break;
			case 0x0D: key = ('='); break;
			case 0x1C: key = ('\n'); break;
			case 0x39: key = (' '); break;
			case 0x0F: key = ('\t'); break;
			case 0x45: case 0xC5: break;
			case 0x2A: case 0x36: shift = true; break; // Shift.
			case 0x1D: control = true; break; // Control.
			case 0x5B: case 0x5C: meta = false; // Meta.
			case 0x38: alt = true; break; // Alt.
			case 0x3A: capsLock = !capsLock; capsLockDown = true; break; // Caps Lock.
			case 0x0E: backspaceDown = true; // Backspace.
			case 0x01: break; // Escape.
			case 0xFA: break; // Key hold thing.
			
			// The keycodes you get are absolutely awful.
			default:
				char *message = "keyboard 0x00 ";
				const char *hex = "0123456789ABCDEF";
				message[11] = hex[(key >> 4) & 0x0F];
				message[12] = hex[key & 0x0F];
				printf(message);
				break;
		}

		if(IsPrintable()) {
			printChar(key);
		} else if(backspaceDown) {
			printGoBack(1);
			printChar(' ');
			printGoBack(1);
		}
	} else {
		shift = false;
		capsLock = false;
		alt = false;
		meta = false;
		capsLockDown = false;
	}
	
	return esp;
}
