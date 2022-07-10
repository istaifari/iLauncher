MBALIGN  equ  1<<0
MEMINFO  equ  1<<1
FLAGS    equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)
VM_BASE     equ 0xC0000000
PDE_INDEX   equ (VM_BASE >> 22)
PSE_BIT     equ 0x00000010
PG_BIT      equ 0x80000000
section .multiboot.data
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
stack_end equ 16*1024

section .multiboot.text
global _start

_start:
	mov ecx, (TEMP_PAGE_DIRECTORY - VM_BASE)
    mov cr3, ecx
    mov ecx, cr4;
    or ecx, PSE_BIT
    mov cr4, ecx
    mov ecx, cr0
    or ecx, PG_BIT
    mov cr0, ecx
    lea ecx, [_start_v86m]
    jmp ecx

section .text
extern _kernel
global _load_gdt
global _load_tss
global _load_idt
global _reboot
global bios32_helper
global bios32_helper_end
global asm_gdt_ptr
global asm_gdt_entries
global asm_idt_ptr
global asm_in_reg_ptr
global asm_out_reg_ptr
global asm_intnum_ptr
extern new_gdt_entries
extern new_gdt_ptr
extern new_idt_ptr
extern new_reg_ptr
extern new_intnum_ptr

%define REBASE(x)                              (((x) - bios32_helper) + 0x7c00)
%define GDTENTRY(x)                            ((x) << 3)
%define CODE32                                 GDTENTRY(1)  ; 0x08
%define DATA32                                 GDTENTRY(2)  ; 0x10
%define CODE16                                 GDTENTRY(6)  ; 0x30
%define DATA16                                 GDTENTRY(7)  ; 0x38


PG_BIT_OFF equ 0x7fffffff
PG_BIT_ON equ 0x80000000

_start_v86m:
    cli
	mov dword[TEMP_PAGE_DIRECTORY], 0
    invlpg[0]
    mov esp, stack_start+stack_end
	push ebx
	call _kernel
loop:
    jmp loop

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
	ret

_load_tss:
    mov ax, 0x28
    ltr ax
    ret

_reboot:
    cli
    mov al,0FEh
    out 64h,al
.halt:
    hlt
    jmp .halt

bios32_helper: use32
    pusha
    mov edx, esp
    ; Now in 32bit protected mode
    ; Disable interrupts
    cli
    ; Turn off paging
    mov ecx, cr0
    and ecx, PG_BIT_OFF
    mov cr0, ecx
    ; Zero cr3(save it in ebx before zeroing it)
    xor ecx, ecx
    mov ebx, cr3
    mov cr3, ecx

    ; Load new gdt
    lgdt [REBASE(asm_gdt_ptr)]

    ; Load idt
    lidt [REBASE(asm_idt_ptr)]
   
    jmp CODE16:REBASE(protected_mode_16)
protected_mode_16:use16
    ; Now in 16bit protected mode
    ; Update data segment selector
    mov ax, DATA16
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Turn off protected mode
    mov eax, cr0
    and  al,  ~0x01
    mov cr0, eax

    jmp 0x0:REBASE(real_mode_16)
real_mode_16:use16
    ; 16 bit real mode data segment
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov sp, 0x8c00

    sti

    ; ### Save current context ###
    pusha
    mov cx, ss
    push cx
    mov cx, gs
    push cx
    mov cx, fs
    push cx
    mov cx, es
    push cx
    mov cx, ds
    push cx
    pushf

    mov ax, sp
    mov edi, temp_esp
    stosw

    ; ### Load the given context from asm_in_reg_ptr ###
    ; Temporaril change esp to asm_in_reg_ptr
    mov esp, REBASE(asm_in_reg_ptr)

    ; only use some general register from the given context
    popa

    ; set a new stack for bios interrupt
    mov sp, 0x9c00
    ; opcode for int
    db 0xCD
asm_intnum_ptr:
    ; put the actual interrupt number here
    db 0x00
    ; ### Write current context to asm_out_reg_ptr ###
    mov esp, REBASE(asm_out_reg_ptr)
    add sp, 28

    pushf
    mov cx, ss
    push cx
    mov cx, gs
    push cx
    mov cx, fs
    push cx
    mov cx, es
    push cx
    mov cx, ds
    push cx
    pusha

    ; ### Restore current context ###
    mov esi, temp_esp
    lodsw
    mov sp, ax

    popf
    pop cx
    mov ds, cx
    pop cx
    mov es, cx
    pop cx
    mov fs, cx
    pop cx
    mov gs, cx
    pop cx
    mov ss, cx
    popa

    mov eax, cr0
    inc eax
    mov cr0, eax
    jmp CODE32:REBASE(protected_mode_32)
protected_mode_32:use32
    mov ax, DATA32
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; restore cr3
    mov cr3, ebx

    ; Turn on paging
    mov ecx, cr0
    or ecx, PG_BIT_ON
    mov cr0, ecx
    
    ; restore esp
    mov esp, edx

    sti
    popa
    ret
padding:
    db 0x0
    db 0x0
    db 0x0
asm_gdt_entries:
    ; 8 gdt entries
    resb 64
asm_gdt_ptr:
    dd 0x00000000
    dd 0x00000000
asm_idt_ptr:
    dd 0x00000000
    dd 0x00000000
asm_in_reg_ptr:
    resw 14
asm_out_reg_ptr:
    dd 0xaaaaaaaa
    dd 0xaaaaaaaa
    dd 0xaaaaaaaa
    dd 0xaaaaaaaa
    dd 0xaaaaaaaa
    dd 0xaaaaaaaa
    dd 0xaaaaaaaa
temp_esp:
    dw 0x0000
bios32_helper_end:

section .data
align 4096
global TEMP_PAGE_DIRECTORY
TEMP_PAGE_DIRECTORY:
    dd 0x00000083
    times(PDE_INDEX - 1) dd 0
    dd 0x00000083
    times(1024 - PDE_INDEX - 1) dd 0 

section .bss
align 32
stack_start:
    resb stack_end