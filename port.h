#ifndef __PORT_H
#define __PORT_H

#include "types.h"

class Port {
	protected:
		u16 portNumber;
		Port(u16 portNumber);
		~Port();
};

class Port8Bit : public Port {
	public:
		Port8Bit(u16 portNumber);
		~Port8Bit();
		virtual void Write(u8 data);
		virtual u8 Read();
};

class Port8BitSlow : public Port8Bit {
	public:
		Port8BitSlow(u16 portNumber);
		~Port8BitSlow();
		virtual void Write(u8 data);
		virtual u8 Read();
};

class Port16Bit : public Port {
	public:
		Port16Bit(u16 portNumber);
		~Port16Bit();
		virtual void Write(u16 data);
		virtual  u16 Read();
};

class Port32Bit : public Port {
	public:
		Port32Bit(u16 portNumber);
		~Port32Bit();
		virtual void Write(u32 data);
		virtual u32 Read();
};

#endif
