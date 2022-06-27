MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER
section .text
bits 32
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

global _start
global _load_gdt
global _load_tss
global _load_idt
global _reboot
extern irq_handler, isr_handler
extern PIC_End
extern HandlePS2Keyboard, HandlePS2Mouse
global irq_common_stub, isr_common_stub

_start:
	cli
	mov esp, _sys_stack
	jmp .stublet
.stublet:
	extern _kernel
	push ebx
	call _kernel
	jmp $

_load_gdt:
	mov	eax, [esp+4]
	lgdt [eax]
	mov	ax, 0x10
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	ss, ax
	jmp	0x08:.flush
.flush:
	ret

_load_tss:
    mov ax, 0x28
    ltr ax
    ret

_load_idt:
	mov	eax, [esp+4]
	lidt [eax]
	sti
	ret

_reboot:
    cli
    mov al,0FEh
    out 64h,al
.halt:
    hlt
    jmp .halt

section .data
gdt_start:
    gdt_null:
        dd 0x00
        dd 0x00
    gdt_code:
        dw 0xffff
        dw 0x0000
        db 0x00
        db 0x9a
        db 0xcf
        db 0x00
    gdt_data:
        dw 0xffff
        dw 0x0000
        db 0x00
        db 0x92
        db 0xcf
        db 0x00
gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start
    
gdt_codeseg equ gdt_code - gdt_start
gdt_dataseg equ gdt_data - gdt_start


section .bss
    resb 8192
_sys_stack: