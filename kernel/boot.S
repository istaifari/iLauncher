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
global read_eip
global copy_page_physical
global _reboot

_start:
	cli
	mov esp, _sys_stack
	jmp .stublet
.stublet:
	extern _kernel
	push esp
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

_load_idt:
	mov	eax, [esp+4]
	lidt [eax]
    sti
	ret

_load_tss:
    mov ax, 0x28
    ltr ax
    ret

read_eip:
    pop eax
    jmp eax

copy_page_physical:
    push ebx
    pushf
    cli
    mov ebx, [esp+12]
    mov ecx, [esp+16]
    mov edx, cr0
    and edx, 0x7fffffff
    mov cr0, edx
    mov edx, 1024
.loop:
    mov eax, [ebx]
    mov [ecx], eax
    add ebx, 4
    add ecx, 4
    dec edx
    jnz .loop
    mov edx, cr0
    or edx, 0x80000000
    mov cr0, edx
    popf
    pop ebx
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