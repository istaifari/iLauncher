#include "../include/tss.h"

tss_entry_t kernel_tss;

extern void _load_tss();

void tss_install()
{
    kernel_tss.ss0 = 0x10;
    asm("movl %%esp, %0;"
        : "=m"(kernel_tss.esp0)::"%eax");
    asm("movl %%cr3, %%eax; movl %%eax, %0;"
        : "=m"(kernel_tss.cr3)::"%eax");
    kernel_tss.cs = 0x08;
    kernel_tss.ss = kernel_tss.ds = kernel_tss.es = kernel_tss.fs = kernel_tss.gs = 0x10;
    kernel_tss.iomap_base = 104;
}

void tss_set_esp0(uint32_t esp0)
{
    kernel_tss.esp0 = esp0;
}
