OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)

SECTIONS
{
    . = 0x00100000;

    .multiboot.text :
    {
        *(.multiboot.text)
    }

    .multiboot.data :
    {
        *(.multiboot.data)
    }

    . += 0xC0000000;

    .text ALIGN(4096) : AT(ADDR(.text) - 0xC0000000)
    {
        *(.text)
    }

    .data ALIGN (4096) : AT(ADDR(.data) - 0xC0000000)
    {
        *(.data)
        *(.rodata*)
    }

    .bss ALIGN (4096) : AT(ADDR(.bss) - 0xC0000000)
    {
        *(COMMON)
        *(.bss)
    }

    end = .;

    /DISCARD/ :
    {
        *(.note.gnu.property);
        *(.eh_frame);
        *(.note*);
        *(.iplt*);
        *(.igot*);
        *(.rel*);
        *(.comment*);
    }
}