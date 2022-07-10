C_SOURCES := $(wildcard *.c)\
			 $(wildcard drivers/*.c)\
			 $(wildcard lib/*.c)\
			 $(wildcard fs/*.c)
GCC_ARCH := m32
GCC_FLAGS := -ffreestanding\
			 -fno-common\
			 -mno-sse\
			 -fno-stack-protector\
			 -mno-red-zone\
			 -fno-exceptions\
			 -fno-pie\
			 -fomit-frame-pointer\
			 -fno-leading-underscore\
			 -mgeneral-regs-only\
			 -I include\
			 -c\
			 -Ofast
LD_SOURCES := *.o
LD_ARCH := melf_i386
ASM_ARCH := felf32 -O0
QEMU_ARCH := i386 -machine pc -serial stdio -bios ../bios.bin -soundhw pcspk -m 8M #-usbdevice mouse

default: emu

all:
	@nasm -$(ASM_ARCH) boot.asm || nasm -$(ASM_ARCH) boot.S || echo Unknown Error!
	@gcc $(GCC_FLAGS) $(C_SOURCES) -$(GCC_ARCH) || echo Unknown Error!
	@ld -$(LD_ARCH) -Tlinker.ld $(LD_SOURCES) --allow-multiple-definition -o kernel.bin || rm *.o *.bin
	@cp kernel.bin ../OS_Files/boot/kernel.bin
	@rm ../ilauncher.iso || echo Okay!
	@make initrd || echo Unknown Error!
	@grub-mkrescue -o ../ilauncher.iso ../OS_Files || echo Unknown Error!
	@rm *.o *.bin *.img || echo Unknown Error!
	@make emu || echo Unknown Error!

#tar cvf ../OS_Files/initrd.tar --directory=../ initrd
initrd:
	echo 0

emu:
	@qemu-system-$(QEMU_ARCH) -cdrom ../ilauncher.iso || echo 404
	@qemu-system-$(QEMU_ARCH) -cdrom ../ilauncher.iso -vnc 127.0.0.1:2 || echo 404
	@qemu-system-$(QEMU_ARCH) -kernel ../OS_Files/boot/kernel.bin || echo 404
	@qemu-system-$(QEMU_ARCH) -kernel ../OS_Files/boot/kernel.bin -vnc 127.0.0.1:2 || echo 404