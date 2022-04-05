#include <common/types.h>
#include <hardware/gdt.h>
#include <drivers/driver.h>
#include <hardware/interrupts.h>
#include <hardware/pci.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/widget.h>
#include <gui/rectangle.h>
#include <gui/button.h>
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

		if(y >= 25) {
			for(y = 0; y < 25; y++) {
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

#ifndef GRAPHICS_MODE

class PrintfKeyboardEventHandler : public drivers::KeyboardEventHandler {
	public:
		void OnKeyDown(char c) {
			if(c == (int)drivers::keys::ESCAPE) {
				printfGoBack(1);
				printfChar(' ');
				printfGoBack(1);
			} else {
				printfChar(c);
			}
		}
};

#endif

#ifndef GRAPHICS_MODE

class PrintfMouseEventHandler : public drivers::MouseEventHandler {
	protected:
		i8 x, y;
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
			videoMemory[80*this->y + this->x] = 
				(videoMemory[80 * this->y + this->x] & 0x0F00) << 4
				| (videoMemory[80 * this->y +this->x] & 0xF000) >> 4
				| (videoMemory[80 * this->y + this->x] & 0x00FF);

			this->x += x;
			this->y += y;
			this->x = this->x < 0 ? 0 : this->x;
			this->y = this->y < 0 ? 0 : this->y;
			// One less because of a minor bug.
			this->x = this->x > 79 ? 79 : this->x;
			this->y = this->y > 24 ? 24 : this->y;
			videoMemory[80*this->y + this->x] = 
				(videoMemory[80 * this->y + this->x] & 0x0F00) << 4
				| (videoMemory[80 * this->y +this->x] & 0xF000) >> 4
				| (videoMemory[80 * this->y + this->x] & 0x00FF);
		}
};

#endif

extern "C" void kernelMain(void *multiboot_structure, unsigned int magic_number) {
	printf("Initalizing Hardware Stage 1: Bare Bones.\n");
	hardware::GlobalDescriptorTable gdt;
	hardware::InterruptManager interrupts(&gdt);
	printf("Initalizing Hardware Stage 2: Driver Abstractions.\n");
	#ifdef GRAPHICS_MODE
		drivers::VideoGraphicsArray vga;
		vga.SetMode(320, 200, 8);
		gui::MasterWidget desktop(&vga, 0, 0, 0xA8);
	#endif

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
	printf("Initalizing Hardware Stage 3: Activating Driver Manager & PCI.\n");
	hardware::PeripheralComponentInterconnectController pciController;
	pciController.SelectDrivers(&driverManager, &interrupts);
	driverManager.ActivateAll();
	interrupts.Activate();
	#ifdef GRAPHICS_MODE
		gui::WidgetCollection workspace(&desktop);
		gui::Rectangle rectangle(&workspace, 5, 30, 50, 10, 0, 0xA8, 0);
		gui::Button button(&workspace, 5, 5, 50, 10, 0xA8, 0, 0);
		desktop.ChangeFocusedWidgetCollection(0);
		workspace.SetupWidgets();
	#endif

	while(1);
}
