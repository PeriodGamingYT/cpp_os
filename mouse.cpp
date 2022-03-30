#include "mouse.h"

MouseEventHandler::MouseEventHandler() {}
void MouseEventHandler::OnMouseBeforeMove(i8 x, i8 y) {}
void MouseEventHandler::OnMouseAfterMove(i8 x, i8 y) {}
void MouseEventHandler::OnMouseButtonDown(u8 button) {}

MouseDriver::MouseDriver(InterruptManager *manager, MouseEventHandler *handler) 
	: InterruptHandler(0x2C, manager),
		dataPort(0x60),
		commandPort(0x64) {
	this->handler = handler;
}

MouseDriver::~MouseDriver() {}

void MouseDriver::Activate() {
	static u16 *videoMemory = (u16*)0xb8000; // stopping weird stuff happening, maybe with qemu?
	videoMemory[0] = 
						  ((videoMemory[0] & 0xF000) >> 4)
						| ((videoMemory[0] & 0x0F00) << 4)
						| (videoMemory[0] & 0x00FF);
	offset = 0;
	buttons = 0;
	commandPort.Write(0xA8); // Activate Mouse Interrupts.
	commandPort.Write(0x20); // Give the current state of the Mouse.
	u8 status = dataPort.Read() | 2;
	commandPort.Write(0x60); // Set Mouse State.
	dataPort.Write(status);
	commandPort.Write(0xD4);
	dataPort.Write(0xF4);
	dataPort.Read();
}

void printf(const char *);
void printChar(char);
void printGoBack(u32);


u32 MouseDriver::HandleInterrupt(u32 esp) {
	u8 status = commandPort.Read();
	if(!(status & 0x20)) {
		printf("waiting for mouse data ");
		return esp;	
	}

	buffer[offset] = dataPort.Read();
	offset = (offset + 1) % 3;
	if(offset == 0) {
		handler->OnMouseBeforeMove(x, y);
		x += buffer[1];
		x = x < 0 ? 0 : x;
		x = x >= 80 ? 79 : x;
		y -= buffer[2];
		y = y < 0 ? 0 : y;
		y = y >= 24 ? 23 : y;
		handler->OnMouseAfterMove(x, y);
		for(u8 i = 0; i < 3; i++) {
			if((buffer[0] & (0x01 << i)) != (buttons & (0x01 << i))) {
				handler->OnMouseButtonDown(i);
			}
		}

		buttons = buffer[0];
	}
	
	return esp;
}
