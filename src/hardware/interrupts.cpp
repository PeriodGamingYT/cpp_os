#include <hardware/interrupts.h>
#include <common/types.h>

using namespace hardware;
void printf(const char *);
void printfHex(u8);

InterruptHandler::InterruptHandler(u8 interruptNumber, InterruptManager *interruptManager) {
	this->interruptNumber = interruptNumber;
	this->interruptManager = interruptManager;
	interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler() {
	if(interruptManager->handlers[interruptNumber] == this) {
		interruptManager->handlers[interruptNumber] = 0;
	}
}

u32 InterruptHandler::HandleInterrupt(u32 esp) {
	return esp;
}

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager *InterruptManager::ActiveInterruptManager = 0;

void InterruptManager::SetInterruptDescriptorTableEntry(
	u8 interruptNumber,
	u16 codeSegmentSelectorOffset,
	void (*handler)(),
	u8 DescriptorPrivilegeLevel,
	u8 DescriptorType) {
	const u8 IDT_DESC_PRESENT = 0x80;
	interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((u32)handler) & 0xFFFF;
	interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((u32)handler) >> 16) & 0xFFFF;
	interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
	interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | (DescriptorPrivilegeLevel << 5);
	interruptDescriptorTable[interruptNumber].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable *gdt) 
	: picMasterCommand(0x20),
		picMasterData(0x21),
		picSlaveCommand(0xA0),
		picSlaveData(0xA1) {
		u16 CodeSegment = gdt->CodeSegmentSelector();
		const u8 IDT_INTERRUPT_GATE = 0xE;
		for(u16 i = 0; i < 256; i++) {
			SetInterruptDescriptorTableEntry(i, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
			handlers[i] = 0;
		}

		SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
		SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
		SetInterruptDescriptorTableEntry(0x2C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
		picMasterCommand.Write(0x11);
		picMasterData.Write(0x20);
		picSlaveCommand.Write(0x11);
		picSlaveData.Write(0x28);
		picMasterData.Write(0x04);
		picSlaveData.Write(0x02);
		picMasterData.Write(0x01);
		picSlaveData.Write(0x01);
		picMasterData.Write(0x00);
		picSlaveData.Write(0x00);
		InterruptDescriptorTablePointer idt;
		idt.size = 256 * sizeof(GateDescriptor) - 1;
		idt.base = (u32)interruptDescriptorTable;
		asm volatile("lidt %0" : : "m" (idt));
}

void InterruptManager::Activate() {
	if(ActiveInterruptManager != 0) {
		ActiveInterruptManager->Deactivate();	
	}
	
	ActiveInterruptManager = this;
	asm("sti");
}

void InterruptManager::Deactivate() {
	if(ActiveInterruptManager == this) {
		ActiveInterruptManager = 0;
		asm("cli");
	}
}

InterruptManager::~InterruptManager() {}

u32 InterruptManager::HandleInterrupt(u8 interruptNumber, u32 esp) {
	if(ActiveInterruptManager != 0) {
		return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);		
	}

	printf("no active interrupt manager ");
	return esp;
}


u32 InterruptManager::DoHandleInterrupt(u8 interruptNumber, u32 esp) {
	if(handlers[interruptNumber] != 0) {
		esp = handlers[interruptNumber]->HandleInterrupt(esp);
	} else if (interruptNumber != 0x20) {
		printf("UNHANDLED INTERRUPT: ");
		printfHex(interruptNumber);
		printf(".\n");
	}
	
	if(0x20 <= interruptNumber && interruptNumber < 0x30) {
		picMasterCommand.Write(0x20);
		if(0x28 <= interruptNumber) {
			picSlaveCommand.Write(0x20);
		}
	}
	
	return esp;
}
