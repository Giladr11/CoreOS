;Main BootSec
[BITS 16]
[ORG 0x7C00]

section .data
    STAGE2_LOAD_SEG         equ 0x7E00
    STAGE2_OFFSET           equ 0x00
    STAGE2_START_SECTOR     equ 0x01
    STAGE2_SECTORS          equ 0x05

    Stage2Packet:           times 16 db 0

section .text
    global _start

_start:
    xor ax, ax                
    mov ds, ax
    mov es, ax                    
    mov ss, ax

    mov sp, 0x7BFF

    call print_boot_msg
    
    call print_press_key

    call wait_for_key
    
    call LBAStage2

    call print_execute_stage2

    jmp STAGE2_LOAD_SEG
    
wait_for_key:
    mov ah, 0x00
    int 0x16

    cmp ah, 0x1C
    jne wait_for_key

    ret

LBAStage2:
    call print_stage2_msg

    mov si, Stage2Packet  
                
    mov word[si]        , 0x10                  ; Packet Size (16-Bytes)
    mov word[si + 2]    , STAGE2_SECTORS        ; Number of Sectors to Read

    mov word[si + 4]    , STAGE2_LOAD_SEG       ; Load Address for Stage 2
    mov word[si + 6]    , STAGE2_OFFSET         ; Segment of Stage 2

    mov dword[si + 8]   , STAGE2_START_SECTOR   ; First Sector to Read (LBA 1 = Sector 2)
    mov dword[si + 12]  , 0x00                  ; Sets the highest 32-Bits of the LBA to 0

    mov ah, 0x42                                ; Extended Read (LBA) Opertaion         

    int 0x13

    jc print_disk_error

    ret

print_boot_msg:
    mov si, init_boot_message
    call print
    
    ret

print_press_key:
    mov si, press_load_stage2
    call print
    
    ret

print_stage2_msg:
    mov si, stage2_message
    call print
    
    ret

print_execute_stage2:
    mov si, stage2_execute
    call print

    ret

print_disk_error:
    mov si, disk_error_message
    call print           

    jmp $                         


init_boot_message  db "[BOOT] Initializing Booting Process..."   , 0x0D, 0x0A, 0x0D, 0x0A, 0
press_load_stage2  db "[INFO] Press 'Enter' to Load Stage 2..."  , 0x0D, 0x0A, 0x0D, 0x0A, 0
stage2_message     db "[+] Loading Stage 2..."                   , 0x0D, 0x0A, 0x0D, 0x0A, 0 
stage2_execute     db "[+] Executing Stage 2..."                 , 0x0D, 0x0A, 0x0D, 0x0A, 0 
disk_error_message db "[-][ERROR]: Reading Disk!"                , 0x0D, 0x0A, 0x0D, 0x0A, 0


%include "src/boot/print16.asm"


times 510-($-$$) db 0x0
dw 0xAA55
