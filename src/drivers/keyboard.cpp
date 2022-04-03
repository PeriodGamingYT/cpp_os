#include <drivers/keyboard.h>

using namespace drivers;
KeyboardEventHandler::KeyboardEventHandler() {}
void KeyboardEventHandler::OnKeyUp(char key) {}
void KeyboardEventHandler::OnKeyDown(char key) {}
void KeyboardEventHandler::OnCapsLock(bool capsLock) {}
void KeyboardEventHandler::OnAlt() {}
void KeyboardEventHandler::OnControl() {}
void KeyboardEventHandler::OnShift() {}
void KeyboardEventHandler::OnMeta() {}

KeyboardDriver::KeyboardDriver(hardware::InterruptManager *manager, KeyboardEventHandler *handler) 
	: hardware::InterruptHandler(0x21, manager),
		dataPort(0x60),
		commandPort(0x64) {
	this->handler = handler;
}

KeyboardDriver::~KeyboardDriver() {}

void KeyboardDriver::Activate() {
	meta = false;
	shift = false;
	capsLock = false;
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

void printf(const char *);
void printChar(char);
void printGoBack(u32);
void printfHex(u8);

bool KeyboardDriver::IsPrintable() {
	return !control && !alt && !meta && !shift && key >= ' ' && key <= 126;
}

char uppercaseChar(char key) {
	if(key >= 'a' && key <= 'z') {
		return key - 32;
	}

	return key;
}

u32 KeyboardDriver::HandleInterrupt(u32 esp) {
	u8 dataReadKey = dataPort.Read();
	if(handler == 0) {
		return esp;
	}
	
	if(dataReadKey < 0x80) {
		switch(dataReadKey) {
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
			case 0x2A: case 0x36: shift = true; handler->OnShift(); break; // Shift.
			case 0x1D: control = true; handler->OnControl(); break; // Control.
			case 0x5B: case 0x5C: meta = false; handler->OnMeta(); break;// Meta.
			case 0x38: alt = true; handler->OnAlt(); break; // Alt.
			case 0x3A: capsLock = !capsLock; handler->OnCapsLock(capsLock); break; // Caps Lock.
			case 0x0E: key = 127; break; // Backspace.
			case 0x01: key = 27; break; // Escape.
			case 0xFA: break; // Key hold thing.
			
			// The keycodes you get are absolutely awful.
			default:
				printf("UNHANDLED KEYCODE: ");
				printfHex(dataReadKey);
				printf(".\n");
				break;
		}

		key = capsLock ? uppercaseChar(key) : key;
		if(IsPrintable()) {
			handler->OnKeyDown(key);;
		}
	} else {
		key = '\0';
		shift = false;
		alt = false;
		meta = false;
	}
	
	return esp;
}
