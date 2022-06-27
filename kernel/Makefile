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
			 -c\
			 -Ofast
LD_SOURCES := *.o
LD_ARCH := melf_i386
ASM_ARCH := felf
QEMU_ARCH := i386 -machine pc -serial stdio -bios ../bios.bin -soundhw pcspk -m 16M #-usbdevice mouse

default: emu

# @genisoimage -quiet -R -b boot/.boot -no-emul-boot -boot-load-size 4 -boot-info-table -o ../ilauncher.iso ../OS_Files || echo Unknown Error!
# @grub-mkrescue -o ../ilauncher.iso ../OS_Files || echo Unknown Error! --allow-multiple-definition

all:
	@nasm -$(ASM_ARCH) boot.S || nasm -$(ASM_ARCH) kernel_asm.S || echo Unknown Error!
	@nasm -$(ASM_ARCH) idt.S || echo Unknown Error!
	@gcc $(GCC_FLAGS) $(C_SOURCES) -$(GCC_ARCH) || echo Unknown Error!
	@ld -$(LD_ARCH) -Tlinker.ld $(LD_SOURCES) -o kernel.bin || rm *.o *.bin
	@cp kernel.bin ../OS_Files/boot/kernel.bin
	@rm ../ilauncher.iso || echo Okay!
	@make initrd || echo Unknown Error!
	@genisoimage -quiet -R -b boot/.boot -no-emul-boot -boot-load-size 4 -boot-info-table -o ../ilauncher.iso ../OS_Files || echo Unknown Error!
	@rm *.o *.bin *.img || echo Unknown Error!
	@make emu || echo Unknown Error!
	@cat bootloader.bin kernel.bin > ../OS_Files/boot/kernel.bin || echo Unknown Error!

#tar cvf ../OS_Files/initrd.tar --directory=../ initrd
initrd:
	echo 0

emu:
	@qemu-system-$(QEMU_ARCH) -cdrom ../ilauncher.iso || echo 404
	@qemu-system-$(QEMU_ARCH) -cdrom ../ilauncher.iso -vnc 127.0.0.1:2 || echo 404
	@qemu-system-$(QEMU_ARCH) -kernel ../OS_Files/boot/kernel.bin || echo 404
	@qemu-system-$(QEMU_ARCH) -kernel ../OS_Files/boot/kernel.bin -vnc 127.0.0.1:2 || echo 404