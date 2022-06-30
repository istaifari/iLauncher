#include "../include/idt.h"

idt_entry_t idt[256];
idtr_t idtr;
extern void _load_idt(uint32_t);

__attribute__((interrupt)) void irq_null(int_frame_t *r)
{
    PIC_End();
}

void load_idt(idtr_t r)
{
    _load_idt((uint32_t)&r);
}

void idt_set_irq(uint8_t *sel)
{
    uint8_t num;
    if (sel == "ME")
        num = 0;
    if (sel == "SE")
        num = 8;
    uint16_t port;
    uint8_t value;
    if (num < 8)
    {
        port = 0x21;
    }
    else
    {
        port = 0xA1;
        num -= 8;
    }
    value = inb(port) & ~(1 << num);
    outb(port, value);
}

void idt_set_gate(unsigned char num, void *base, unsigned short sel, unsigned char flags)
{
    idt[num].base_low = ((uint32_t)(base)&0xFFFF);
    idt[num].base_high = ((uint32_t)(base) >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].zero = 0;
    idt[num].flags = flags | 0x60;
}

void idt_install()
{
    idtr.limit = (sizeof(idt_entry_t) * 256) - 1;
    idtr.base = (uintptr_t)&idt;
    memset(&idt, 0, idtr.limit);
    idt_set_gate(32 + 0, HandleTimer, 0x08, 0x8E);
    idt_set_gate(32 + 1, HandlePS2Keyboard, 0x08, 0x8E);
    idt_set_gate(32 + 12, HandlePS2Mouse, 0x08, 0x8E);
    for (long i = 0; i < 15; i++)
        if (idt[32 + i].sel == 0)
            idt_set_gate(32 + i, irq_null, 0x08, 0x8E);
    InitPS2Mouse();
    InitTimer();
    RemapPIC();
    outb(0x21, 0b11111001);
    outb(0xA1, 0b11101111);
    idt_set_irq("ME");
    load_idt(idtr);
}