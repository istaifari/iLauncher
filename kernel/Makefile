#I love Makefiles!
#nasm -$(ASM_ARCH) multiboot.S || echo Unknown Error!

C_SOURCES_1 := $(wildcard *.c)
C_SOURCES_2 := $(wildcard drivers/*.c)
C_SOURCES_3 := $(wildcard lib/*.c)
GCC_ARCH := m32
LD_SOURCES := *.o
LD_ARCH := melf_i386
ASM_ARCH := felf32
ASM_BOOT_ARCH := fbin
QEMU_ARCH := i386

default: emu

all:
	nasm -$(ASM_ARCH) boot.S || echo Unknown Error!
	gcc -ffreestanding -fno-stack-protector -fno-pie -c $(C_SOURCES_1) -$(GCC_ARCH) || echo Unknown Error!
	gcc -ffreestanding -fno-stack-protector -fno-pie -c $(C_SOURCES_2) -$(GCC_ARCH) || echo Unknown Error!
	gcc -ffreestanding -fno-stack-protector -fno-pie -c $(C_SOURCES_3) -$(GCC_ARCH) || echo Unknown Error!
	ld -o kernel.bin -$(LD_ARCH) $(LD_SOURCES) -T linker.ld || echo Unknown Error!
	cp kernel.bin ../OS_Files/boot/kernel.bin || echo Unknown Error!
	grub-mkrescue -o ../ilauncher.iso ../OS_Files || echo Unknown Error!
	rm *.bin *.o || echo Unknown Error!
	make emu || echo Unknown Error!

all-NoEmu:
	nasm -$(ASM_ARCH) boot.S || echo Unknown Error!
	gcc -ffreestanding -fno-stack-protector -fno-pie -c $(C_SOURCES_1) -$(GCC_ARCH) || echo Unknown Error!
	gcc -ffreestanding -fno-stack-protector -fno-pie -c $(C_SOURCES_2) -$(GCC_ARCH) || echo Unknown Error!
	gcc -ffreestanding -fno-stack-protector -fno-pie -c $(C_SOURCES_3) -$(GCC_ARCH) || echo Unknown Error!
	ld -o kernel.bin -$(LD_ARCH) $(LD_SOURCES) -T linker.ld || echo Unknown Error!
	cp kernel.bin ../OS_Files/boot/kernel.bin || echo Unknown Error!
	grub-mkrescue -o ../ilauncher.iso ../OS_Files || echo Unknown Error!
	rm *.bin *.o || echo Unknown Error!

emu:
	qemu-system-$(QEMU_ARCH) -cdrom ../ilauncher.iso || qemu-system-$(QEMU_ARCH) ../ilauncher.img || echo 404: ilauncher.iso