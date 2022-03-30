#ifndef __INCLUDE_HARDWARE_INTERRUPTS_H
#define __INCLUDE_HARDWARE_INTERRUPTS_H

#include <common/types.h>
#include <hardware/port.h>
#include <hardware/gdt.h>

namespace hardware {
	class InterruptManager;

	class InterruptHandler {
		protected:
			u8 interruptNumber;
			InterruptManager *interruptManager;
			InterruptHandler(u8 interruptNumber, InterruptManager *interruptManager);
			~InterruptHandler();
		public:
			virtual u32 HandleInterrupt(u32 esp);
	};

	class InterruptManager {
		friend class InterruptHandler;
		protected:
			static InterruptManager *ActiveInterruptManager;
			InterruptHandler *handlers[256];
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
			void Deactivate();
			static u32 HandleInterrupt(u8 interruptNumber, u32 esp);
			u32 DoHandleInterrupt(u8 interruptNumber, u32 esp);
			static void InterruptIgnore();
			static void HandleInterruptRequest0x00();
			static void HandleInterruptRequest0x01();
			static void HandleInterruptRequest0x0C();
	};
}

#endif
