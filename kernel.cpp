#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"

static u8 x = 0, y = 0;
void printf(const char *str) {
	unsigned short *videoMemory = (unsigned short*)0xb8000;
	for(int i = 0; str[i] != '\0'; i++) {
		switch(str[i]) {
			case '\t':
				x += 4;
				break;
			case '\n':
				y++;
				x = 0;
				break;
			default:
				videoMemory[80*y+x] = (videoMemory[80*y+x] & 0xFF00) | str[i];
				x++;
				break;
		}

		if(x >= 80) {
			y++;
			x = 0;
		}

		if(y >= 24) {
			for(y = 0; y < 24; y++) {
				for(x = 0; x < 24; x++) {
					videoMemory[80*y+x] = (videoMemory[80*y+x] & 0xFF00) | ' ';
				}	
			}
			
			x = 0, y = 0;
		}
	}
}

void printChar(char c) {
	char *s = " ";
	s[0] = c;
	printf(s);
}

extern "C" void kernelMain(void *multiboot_structure, unsigned int magic_number) {
	printf("Hello, \n\tWorld!");
	GlobalDescriptorTable gdt;
	InterruptManager interrupts(&gdt);
	KeyboardDriver keyboard(&interrupts);
	interrupts.Activate();
	while(1);
}
