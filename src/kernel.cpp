#include <common/types.h>
#include <hardware/gdt.h>
#include <drivers/driver.h>
#include <hardware/interrupts.h>
#include <hardware/pci.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/widget.h>
#include <gui/window.h>
#include <gui/desktop.h>
#define GRAPHICS_MODE

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
				for(x = 0; x < 80; x++) {
					videoMemory[80*y+x] = (videoMemory[80*y+x] & 0xFF00) | ' ';
				}	
			}
			
			x = 0, y = 0;
		}
	}
}

void printfHex(u8 number) {
	char *message = "0x00";
	const char *hex = "0123456789ABCDEF";
	message[2] = hex[(number >> 4) & 0xF];
	message[3] = hex[number & 0xF];
	printf(message);
}

void printfChar(char c) {
	char *s = " ";
	s[0] = c;
	printf(s);
}

void printfGoBack(u32 amount) {
	if(amount > 80 * 24) {
		return; // Can't overflow back to normal.
	}
	
	y -= (u32)(amount / 80);
	if(y > 24) { // Unsigned int overflow.
		x = 0;
		return;
	}

	x -= amount - ((u32)(amount / 80) * 80);
}

class PrintfKeyboardEventHandler : public drivers::KeyboardEventHandler {
	public:
		void OnKeyDown(char c) {
			printfChar(c);
		}

		void OnDelete() {
			printfGoBack(1);
			printfChar(' ');
			printfGoBack(1);
		}
};

class PrintfMouseEventHandler : public drivers::MouseEventHandler {
	protected:
		i8 oldX, oldY, x, y;
	public:
		void OnMouseSetup() {
			u16* videoMemory = (u16*)0xb8000;
			videoMemory[0] = 
				(videoMemory[0] & 0x0F00) << 4
				| (videoMemory[0] & 0xF000) >> 4
				| (videoMemory[0] & 0x00FF);
		}
		
		void OnMouseMove(i8 x, i8 y) {
			u16* videoMemory = (u16*)0xb8000;
			videoMemory[80*oldY+oldX] = 
				(videoMemory[80*oldY+oldX] & 0x0F00) << 4
				| (videoMemory[80*oldY+oldX] & 0xF000) >> 4
				| (videoMemory[80*oldY+oldX] & 0x00FF);

			this->x += x;
			this->y += y;
			videoMemory[80*y+x] = 
				(videoMemory[80*y+x] & 0x0F00) << 4
				| (videoMemory[80*y+x] & 0xF000) >> 4
				| (videoMemory[80*y+x] & 0x00FF);
		}
};

extern "C" void kernelMain(void *multiboot_structure, unsigned int magic_number) {
	printf("Initalizing Hardware Stage 1: Bare Bones.\n");
	hardware::GlobalDescriptorTable gdt;
	hardware::InterruptManager interrupts(&gdt);
	printf("Initalizing Hardware Stage 2: Driver Abstractions.\n");
	gui::Desktop desktop(320, 200, 0x00, 0x00, 0xA8);
	drivers::DriverManager driverManager;
	#ifdef GRAPHICS_MODE
		drivers::KeyboardDriver keyboard(&interrupts, &desktop);
	#else
		PrintfKeyboardEventHandler keyboardHandler;
		drivers::KeyboardDriver keyboard(&interrupts, &keyboardHandler);
	#endif

	driverManager.AddDriver(&keyboard);
	#ifdef GRAPHICS_MODE
		drivers::MouseDriver mouse(&interrupts, &desktop);
	#else
		PrintfMouseEventHandler mouseHandler;
		drivers::MouseDriver mouse(&interrupts, &mouseHandler);
	#endif

	driverManager.AddDriver(&mouse);
	#ifdef GRAPHICS_MODE
		drivers::VideoGraphicsArray vga;
	#endif

	printf("Initalizing Hardware Stage 3: Activating Driver Manager & PCI.\n");
	hardware::PeripheralComponentInterconnectController pciController;
	pciController.SelectDrivers(&driverManager, &interrupts);
	driverManager.ActivateAll();
	interrupts.Activate();
	#ifdef GRAPHICS_MODE
		vga.SetMode(320, 200, 8);
		gui::Window window1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
		desktop.AddChild(&window1);
		gui::Window window2(&desktop, 40, 15, 30, 30, 0x00, 0xA8, 0x00);
		desktop.AddChild(&window2);
	#endif

	while(1) {
		#ifdef GRAPHICS_MODE
			desktop.Draw(&vga);
		#endif
	}
}
