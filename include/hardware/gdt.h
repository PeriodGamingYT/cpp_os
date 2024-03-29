#ifndef __INCLUDE_HARDWARE_GDT_H
#define __INCLUDE_HARDWARE_GDT_H

#include <common/types.h>

namespace hardware {
	class GlobalDescriptorTable {
			public:
				class SegmentDescriptor {
					private:
						u16 limit_lo;
						u16 base_lo;
						u8 base_hi;
						u8 type;
						u8 flags_limit_hi;
						u8 base_vhi;
					public:
						SegmentDescriptor(u32 base, u32 limit, u8 type);
						u32 Base();
						u32 Limit();
				} __attribute__((packed));

				SegmentDescriptor nullSegmentSelector;
				SegmentDescriptor unusedSegmentSelector;
				SegmentDescriptor codeSegmentSelector;
				SegmentDescriptor dataSegmentSelector;

			// public:
				GlobalDescriptorTable();
				~GlobalDescriptorTable();
				u16 CodeSegmentSelector();
				u16 DataSegmentSelector();
	};
}

#endif
