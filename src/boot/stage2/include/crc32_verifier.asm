;CRC-32 Verifications
section .bss
    kernel_checksum_calc_buffer resd 0x01
    kernel_checksum_read_buffer resd 0x01

section .text
CalcKernelCRC32:
    mov esi, PHYSICAL_KERNEL_ADDR
    mov ecx, KERNEL_SIZE
    
    call _start_crc32
    
    mov [kernel_checksum_calc_buffer], ebx 

    ret

ReadPreBootCRC32:
    mov ax, [0x7DFA]

    xchg ah, al 

    mov bx, [0x7DFC]

    xchg bh, bl

    mov [kernel_checksum_read_buffer], bx
    mov [kernel_checksum_read_buffer + 2], ax

    ret

CompareCRC32:
    mov edx, [kernel_checksum_calc_buffer]
    mov ecx, [kernel_checksum_read_buffer]

    cmp edx, ecx
    jne print_not_match

    call print_match

    ret

print_not_match:
    mov si, checksums_not_match
    call print

    jmp $

print_match:
    mov si, checksums_match
    call print

    ret


checksums_not_match db "[-][ERROR]: Kernel CRC-32 Checksums Do not Match!"     , 0x0D, 0x0A, 0x0D, 0x0A, 0
checksums_match     db "[+] Kernel CRC-32 Verificitation has been Completed!"  , 0x0D, 0x0A, 0x0D, 0x0A, 0


%include "src/boot/stage2/include/crc32.asm"
%include "src/boot/print16.asm"
