MAGIC_NUMBER equ 0x1BADB002 ; 1 bad boot
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER
section .text
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM
KERNEL_STACK_SIZE equ 8192  ; 8kb of stack

extern gp
global gdt_flush

extern _kernel
section .text

global _start
global _poweroff
global _reboot
global _krand
global _ksleep
bits 32

_start:
    mov esp, stack + KERNEL_STACK_SIZE
    call _kernel
    jmp $

_krand:
    push ebp
    mov ebp, esp
    RDTSC
    mov ebx,dword [ebp+8]
    mov ecx,dword [ebp+12]
    xor edx,edx
    sub ecx,ebx
    div ecx
    add edx,ebx
    mov eax,edx
    mov esp, ebp
    pop ebp
    ret

_ksleep:
    push ebp
    mov ebp, esp
    mov ecx,dword [ebp+8]
    imul ecx,1000
.lp:
    out 0x80,al
    loop .lp
    mov esp, ebp
    pop ebp
    ret

_poweroff:
    hlt

_reboot:
    cli
    mov al,0FEh
    out 64h,al
.halt:
    hlt
    jmp .halt

gdt_flush:
    lgdt [gp]        ; Load the GDT with our '_gp' which is a special pointer
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2   ; 0x08 is the offset to our code segment: Far jump!
flush2:
    ret               ; Returns back to the C code!

section .bss
align 8
stack:
    resb KERNEL_STACK_SIZE