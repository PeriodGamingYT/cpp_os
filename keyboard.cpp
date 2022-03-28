#include "keyboard.h"

KeyboardDriver::KeyboardDriver(InterruptManager *manager) 
	: InterruptHandler(0x21, manager),
		dataPort(0x60),
		commandPort(0x64) {
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

u32 KeyboardDriver::HandleInterrupt(u32 esp) {
	u8 key = dataPort.Read();
	if(key < 0x80) {
		switch(key) {
			case 0x02: printf("1"); break;
			case 0x03: printf("2"); break;
			case 0x04: printf("3"); break;
			case 0x05: printf("4"); break;
			case 0x06: printf("5"); break;
			case 0x07: printf("6"); break;
			case 0x08: printf("7"); break;
			case 0x09: printf("8"); break;
			case 0x0A: printf("9"); break;
			case 0x0B: printf("0"); break;
			case 0x10: printf("q"); break;
			case 0x11: printf("w"); break;
			case 0x12: printf("e"); break;
			case 0x13: printf("r"); break;
			case 0x14: printf("t"); break;
			case 0x15: printf("z"); break;
			case 0x16: printf("u"); break;
			case 0x17: printf("i"); break;
			case 0x18: printf("o"); break;
			case 0x19: printf("p"); break;
			case 0x1E: printf("a"); break;
			case 0x1F: printf("s"); break;
			case 0x20: printf("d"); break;
			case 0x21: printf("f"); break;
			case 0x22: printf("g"); break;
			case 0x23: printf("h"); break;
			case 0x24: printf("j"); break;
			case 0x25: printf("k"); break;
			case 0x26: printf("l"); break;
			case 0x2C: printf("y"); break;
			case 0x2D: printf("x"); break;
			case 0x2E: printf("c"); break;
			case 0x2F: printf("v"); break;
			case 0x30: printf("b"); break;
			case 0x31: printf("n"); break;
			case 0x32: printf("m"); break;
			case 0x33: printf(","); break;
			case 0x34: printf("."); break;
			case 0x35: printf("/"); break;
			case 0x2B: printf("\\"); break;
			case 0x0C: printf("-"); break;
			case 0x1A: printf("["); break;
			case 0x1B: printf("]"); break;
			case 0x27: printf(";"); break;
			case 0x28: printf("\'"); break;
			case 0x0D: printf("="); break;
			case 0x1C: printf("\n"); break;
			case 0x39: printf(" "); break;
			case 0x0F: printf("\t"); break;
			case 0x45: case 0xC5: break;
			case 0x2A: break; // Shift.
			case 0x36: break; // Right Shift.
			case 0x1D: break; // Control.
			case 0x5B: break; // Left Meta.
			case 0x38: break; // Alt.
			case 0x3A: break; // Caps Lock.
			case 0x5C: break; // Right Meta.
			case 0x0E: break; // Backspace.
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
	}
	
	return esp;
}
