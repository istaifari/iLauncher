section ._multiboot_header
MAGIC_NUMBER equ 0x1BADB002
FLAGS equ 0x0
CHECKSUM equ -MAGIC_NUMBER
KERNEL_STACK_SIZE equ 4096
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

section .text
extern _kernel
global _start
global _poweroff
global _reboot
global _krand
global _ksleep
bits 32

_poweroff:
    hlt

_reboot:
    cli
    mov al,0FEh
    out 64h,al
.halt:
    hlt
    jmp .halt

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

_start:
    mov esp, _stack + KERNEL_STACK_SIZE
    call _kernel

section .bss
align 4
_stack:
    resb KERNEL_STACK_SIZE
