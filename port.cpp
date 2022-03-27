#include "port.h"
#include "types.h"

Port::Port(u16 portNumber) {
	this->portNumber = portNumber;
}

Port::~Port() {}

Port8Bit::Port8Bit(u16 portNumber)
	: Port(portNumber) {}

Port8Bit::~Port8Bit() {}

void Port8Bit::Write(u8 data) {
	asm volatile("outb %0, %1" : : "a" (data), "Nd" (portNumber));
}

u8 Port8Bit::Read() {
	u8 result;
	asm volatile("inb %1, %0" : "=a" (result) : "Nd" (portNumber));
	return result;
}

Port8BitSlow::Port8BitSlow(u16 portNumber)
	: Port8Bit(portNumber) {}

Port8BitSlow::~Port8BitSlow() {}

void Port8BitSlow::Write(u8 data) {
	asm volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (portNumber));
}

u8 Port8BitSlow::Read() {
	u8 result;
	asm volatile("inb %1, %0" : "=a" (result) : "Nd" (portNumber));
	return result;
}

Port16Bit::Port16Bit(u16 portNumber)
	: Port(portNumber) {}

Port16Bit::~Port16Bit() {}

void Port16Bit::Write(u16 data) {
	asm volatile("outw %0, %1" : : "a" (data), "Nd" (portNumber));
}

u16 Port16Bit::Read() {
	u16 result;
	asm volatile("inw %1, %0" : "=a" (result) : "Nd" (portNumber));
	return result;
}

Port32Bit::Port32Bit(u16 portNumber)
	: Port(portNumber) {}

Port32Bit::~Port32Bit() {}

void Port32Bit::Write(u32 data) {
	asm volatile("outl %0, %1" : : "a" (data), "Nd" (portNumber));
}

u32 Port32Bit::Read() {
	u32 result;
	asm volatile("inl %1, %0" : "=a" (result) : "Nd" (portNumber));
	return result;
}
