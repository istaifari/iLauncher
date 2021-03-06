#include <tss.h>

tss_entry_t kernel_tss;
extern void _load_tss();

void tss_install(uint32_t idx, uint32_t kss, uint32_t kesp)
{
    uint32_t base = (uint32_t)&kernel_tss;
    gdt_set_gate(idx, base, base + sizeof(tss_entry_t), 0xE9, 0);
    memset(&kernel_tss, 0, sizeof(tss_entry_t));
    kernel_tss.ss0 = kss;
    kernel_tss.esp0 = kesp;
    kernel_tss.cs = 0x0b;
    kernel_tss.ds = 0x13;
    kernel_tss.es = 0x13;
    kernel_tss.fs = 0x13;
    kernel_tss.gs = 0x13;
    kernel_tss.ss = 0x13;
    _load_tss();
}

void tss_set_stack(uint32_t kss, uint32_t kesp)
{
    kernel_tss.ss0 = kss;
    kernel_tss.esp0 = kesp;
}
