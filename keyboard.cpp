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
			case 0xFA: break;
			case 0x45: case 0xC5: break;
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
