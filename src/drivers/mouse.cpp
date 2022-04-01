#include <drivers/mouse.h>

using namespace drivers;
MouseEventHandler::MouseEventHandler() {}
void MouseEventHandler::OnMouseMove(i8 x, i8 y) {}
void MouseEventHandler::OnMouseButtonDown(u8 button) {}
void MouseEventHandler::OnMouseButtonUp(u8 button) {}
void MouseEventHandler::OnMouseSetup() {}

MouseDriver::MouseDriver(hardware::InterruptManager *manager, MouseEventHandler *handler) 
	: hardware::InterruptHandler(0x2C, manager),
		dataPort(0x60),
		commandPort(0x64) {
	this->handler = handler;
}

MouseDriver::~MouseDriver() {}

void MouseDriver::Activate() {
	handler->OnMouseSetup();
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
		if(buffer[1] != 0 || buffer[2] != 0) {
				handler->OnMouseMove((i8)buffer[1], -((i8)buffer[2]));
		}

		for(u8 i = 0; i < 3; i++) {
			if((buffer[0] & (0x1 << i)) != (buttons & (0x1 << i))) {
				if(buttons & (0x1 << i)) {
					handler->OnMouseButtonUp(i + 1);
				} else {
					handler->OnMouseButtonDown(i + 1);
				}
			}
		}

		buttons = buffer[0];
	}
	
	return esp;
}
