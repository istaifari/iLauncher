[org 0x7c00]

jmp load

bpbBytesPerSector:  	DW 512
bpbSectorsPerCluster: 	DB 1
bpbReservedSectors: 	DW 1
bpbNumberOfFATs: 	    DB 2
bpbRootEntries: 	    DW 224
bpbTotalSectors: 	    DW 2880
bpbMedia: 	            DB 0xf0
bpbSectorsPerFAT: 	    DW 9
bpbSectorsPerTrack: 	DW 18
bpbHeadsPerCylinder: 	DW 2
bpbHiddenSectors: 	    DD 0
bpbTotalSectorsBig:     DD 0
bsDriveNumber: 	        DB 0
bsUnused: 	            DB 0
bsExtBootSignature: 	DB 0x29
bsSerialNumber:	        DD 0xa0a1a2a3
bsVolumeLabel: 	        DB "iLauncher"
bsFileSystem: 	        DB "FAT12"
bsInit:                 times 0Bh-$+bsInit DB 0

load:
    mov ebp, 0x9100
    mov esp, ebp
    mov ah,00
    int 13h

read_sector:
    pusha
    mov ax, 0x0
    mov es, ax
    mov bx, 0x1000
    mov ah, 02
    mov al, 15
    mov ch, 00
    mov cl, 02
    mov dh, 00
    int 13h
    popa
    cli
    xor ax,ax
    mov ds, ax
    lgdt [gdt_descriptor]
    mov eax , cr0
    or eax , 0x1
    mov cr0 , eax
    jmp gdt_codeseg:init_pm

[bits 32]

init_pm:
    mov ax, gdt_dataseg
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov ebp, 0x9000
    mov esp, ebp
    call 0x1000
    jmp $

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
times 510 -( $ - $$ ) db 0 
dw 0xaa55