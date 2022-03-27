#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

class InterruptManager {
	protected:
		struct GateDescriptor {
			u16 handlerAddressLowBits;
			u16 gdt_codeSegmentSelector;
			u8 reserved;
			u8 access;
			u16 handlerAddressHighBits;
		} __attribute__((packed));

		static GateDescriptor interruptDescriptorTable[256];

		struct InterruptDescriptorTablePointer {
			u16 size;
			u32 base;
		} __attribute__((packed));
		
		static void SetInterruptDescriptorTableEntry(
			u8 interruptNumber,
			u16 codeSegmentSelectorOffset,
			void (*handler)(),
			u8 DescriptorPrivilegeLevel,
			u8 DescriptorType
		);

		Port8BitSlow picMasterCommand;
		Port8BitSlow picMasterData;
		Port8BitSlow picSlaveCommand;
		Port8BitSlow picSlaveData;
	public:
		InterruptManager(GlobalDescriptorTable *gdt);
		~InterruptManager();
		void Activate();
		
		static u32 HandleInterrupt(u8 interruptNumber, u32 esp);
		static void InterruptIgnore();
		static void HandleInterruptRequest0x00();
		static void HandleInterruptRequest0x01();
};

#endif
