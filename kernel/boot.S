extern _kernel

section .text

global _start
global _poweroff
global _reboot
global _krand
global _ksleep
bits 32

_start:
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