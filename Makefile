GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wwrite-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = \
	obj/loader.o \
	obj/hardware/gdt.o \
	obj/drivers/driver.o \
	obj/hardware/port.o \
	obj/hardware/interruptstubs.o \
	obj/hardware/interrupts.o \
	obj/hardware/pci.o \
	obj/drivers/keyboard.o \
	obj/drivers/mouse.o \
	obj/drivers/vga.o \
	obj/gui/widget.o \
	obj/gui/rectangle.o \
	obj/gui/button.o \
	obj/gui/text.o \
	obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GPPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '    multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '    boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso
	
run: mykernel.iso
	(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm myos &

vmr:
	(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm myos &

.PHONY: clean
clean:
	rm -rf obj mykernel.bin mykernel.iso
	mkdir obj
