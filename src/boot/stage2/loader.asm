;Main Loader
[ORG 0x7E00]
[BITS 16]

section .data
    KERNEL_LOAD_SEG      equ 0x1000
    PHYSICAL_KERNEL_ADDR equ 0x10000
    KERNEL_START_ADDR    equ 0x100000
    KERNEL_START_SECTOR  equ 0x06
    KERNEL_SECTORS       equ 42
    KERNEL_OFFSET        equ 0x00
    KERNEL_SIZE          equ 21440

    KernelPacket:       times 16 db 0

section .text
    global _start

_start: 
    mov ax, 0x7E00
    mov ds, ax

    call print_press_key

    call wait_for_key

    call LBAKernel
    
    call print_start_crc32_message

    call CalcKernelCRC32

    call ReadPreBootCRC32

    call CompareCRC32

    jmp LoadPM

wait_for_key:
    mov ah, 0x00
    int 0x16

    cmp ah, 0x1C
    jne wait_for_key

    ret

LBAKernel:
    mov si, KernelPacket

    mov word[si]        , 0x10                 
    mov word[si + 2]    , KERNEL_SECTORS       

    mov word[si + 4]    , KERNEL_OFFSET        
    mov word[si + 6]    , KERNEL_LOAD_SEG      

    mov dword[si + 8]   , KERNEL_START_SECTOR  
    mov dword[si + 12]  , 0x00                 

    mov ah, 0x42                               
    
    int 0x13

    jc print_disk_error

    ret

print_press_key:
    mov si, press_load_kernel
    call print
    
    ret

print_start_crc32_message:
    mov si, checksum_start_msg
    call print
    
    ret

print_readkernel_msg:
    mov si, read_kernel_message
    call print

    ret

print_disk_error:
    mov si, disk_error_message
    call print             

    jmp $                        


press_load_kernel   db "[INFO] Press 'Enter' to Advance System Initialization..."  , 0x0D, 0x0A, 0x0D, 0x0A, 0
read_kernel_message db "[+] Accessing Kernel on disk for Integrity Check..."       , 0x0D, 0x0A, 0x0D, 0x0A, 0
checksum_start_msg  db "[+] Initiating Kernel CRC-32 Verifications..."             , 0x0D, 0x0A, 0x0D, 0x0A, 0
disk_error_message  db "[-][ERROR]: Reading Disk!"                                 , 0x0D, 0x0A, 0x0D, 0x0A, 0


%include "src/boot/stage2/include/crc32_verifier.asm"
%include "src/boot/stage2/include/initpm.asm"
%include "src/boot/print16.asm"


times 1520-($-$$) db 0x0
