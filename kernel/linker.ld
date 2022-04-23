OUTPUT_FORMAT("elf32-i386");
ENTRY(_start);
SECTIONS
{
    . = 0x1000;
    .text :
    {
        *(.text);
    }

    .data :
    {
        *(.data);
        *(.rodata);
    }

    .bss :
    {
        *(.bss);
    }

    /DISCARD/ :
    {
        *(.note.gnu.property);
        *(.eh_frame);
        *(.note*);
        *(.iplt*);
        *(.igot*);
        *(.rel*);
        *(.comment);
    }
}