OUTPUT_FORMAT("elf32-i386");
ENTRY(_start);

SECTIONS
{
    .multiboot ALIGN (0x1000) : AT(0x100000)
    {
        *(.multiboot)
    }

    .text 0x100000 :
    {
        code = .; _code = .; __code = .;
        *(.text)
        . = ALIGN(4096);
    }

    .data :
    {
        data = .; _data = .; __data = .;
        *(.data)
        *(.rodata)
        . = ALIGN(4096);
    }

    .bss :
    {
        bss = .; _bss = .; __bss = .;
        *(.bss)
        . = ALIGN(4096);
    }

    /DISCARD/ :
    {
        *(.note.gnu.property)
        *(.eh_frame)
        *(.note*)
        *(.iplt*)
        *(.igot*)
        *(.rel*)
        *(.comment)
    }
    end = .; _end = .; __end = .;
}