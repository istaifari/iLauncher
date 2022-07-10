#pragma once
#include <kernel.h>

typedef struct idt_entry_t
{
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed)) idt_entry_t;

typedef struct idtr_t
{
    uint16_t limit;
    uintptr_t base;
} __attribute__((packed)) idtr_t;

typedef struct int_frame
{
    uint32_t ds;                                     // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no, err_code;                       // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
} int_frame_t;

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

extern __attribute__((interrupt)) void irq_null(int_frame_t *r);
extern __attribute__((interrupt)) void PageFault_Handler(int_frame_t *r);
extern __attribute__((interrupt)) void TimerInt_Handler(int_frame_t *r);
extern __attribute__((interrupt)) void KeyboardInt_Handler(int_frame_t *r);
extern __attribute__((interrupt)) void MouseInt_Handler(int_frame_t *r);

extern idt_entry_t idt[256];

void idt_set_irq(uint8_t *sel);
void idt_set_gate(unsigned char num, void *base, unsigned short sel, unsigned char flags);
void idt_install();