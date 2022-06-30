#pragma once
#include "../kernel.h"

typedef struct idt_entry_t
{
    unsigned short base_low;
    unsigned short sel;
    unsigned char zero;
    unsigned char flags;
    unsigned short base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct idtr_t
{
    unsigned short limit;
    uintptr_t base;
} __attribute__((packed)) idtr_t;

typedef struct int_frame int_frame_t;
extern __attribute__((interrupt)) void HandleTimer(int_frame_t *r);
extern __attribute__((interrupt)) void HandlePS2Keyboard(int_frame_t *r);
extern __attribute__((interrupt)) void HandlePS2Mouse(int_frame_t *r);

void idt_install();