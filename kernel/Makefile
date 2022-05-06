C_SOURCES_1 := $(wildcard *.c)
C_SOURCES_2 := $(wildcard drivers/*.c)
C_SOURCES_3 := $(wildcard lib/*.c)
GCC_ARCH := m32
LD_SOURCES := *.o
LD_ARCH := melf_i386
ASM_ARCH := felf32
ASM_BOOT_ARCH := fbin
QEMU_ARCH := i386# -usbdevice keyboard

default: emu

#grub-mkrescue -o ../ilauncher.iso ../OS_Files || echo Unknown Error!

all:
	nasm -$(ASM_BOOT_ARCH) ../boot/bootloader.S -o bootloader.bin || echo Unknown Error!
	nasm -$(ASM_ARCH) boot.S || echo Unknown Error!
	gcc -ffreestanding -fno-stack-protector -fno-pie -c -g -Os $(C_SOURCES_1) -$(GCC_ARCH) || echo Unknown Error!
	gcc -ffreestanding -fno-stack-protector -fno-pie -c -g -Os $(C_SOURCES_2) -$(GCC_ARCH) || echo Unknown Error!
	gcc -ffreestanding -fno-stack-protector -fno-pie -c -g -Os $(C_SOURCES_3) -$(GCC_ARCH) || echo Unknown Error!
	ld -$(LD_ARCH) -Tlinker.ld -nostdlib --oformat binary $(LD_SOURCES) -o kernel.bin || echo Unknown Error!
	cat bootloader.bin kernel.bin > ../OS_Files/boot.img || echo Unknown Error!
	rm *.o *.bin || echo Unknown Error!
	make emu || echo Unknown Error!

emu:
	qemu-system-$(QEMU_ARCH) -cdrom ../ilauncher.iso || qemu-system-$(QEMU_ARCH)  ../OS_Files/boot.img || echo 404: ilauncher.iso
	qemu-system-$(QEMU_ARCH) -cdrom ../ilauncher.iso -vnc 127.0.0.1:2 || qemu-system-$(QEMU_ARCH)  ../OS_Files/boot.img -vnc 127.0.0.1:2 || echo 404: ilauncher.iso