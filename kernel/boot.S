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

section .bss
    resb 8192
_sys_stack: