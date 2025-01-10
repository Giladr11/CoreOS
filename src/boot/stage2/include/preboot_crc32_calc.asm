;Pre-Boot CRC-32 Calculation
section .data
    KERNEL_SIZE equ 19632
    STDOUT_RESULT_SIZE equ 0x0A    

    kernel_filename db  "build/kernel/kernel.bin", 0
    file_desc dd 0

    stdout_result db "0x00000000", 0        
    hex_digits db "0123456789ABCDEF", 0    

section .bss
    crc32_result resd 0x01
    kernel_buffer resb KERNEL_SIZE

section .text
    global _start

_start:
    call OpenKernel

    call ReadKernel

    call CloseKernel

    mov esi, kernel_buffer
    mov ecx, KERNEL_SIZE

    call _start_crc32

    mov [crc32_result], ebx

    mov esi, stdout_result              
    call convert_crc32_to_hex

    call WriteStdout

    jmp _exit

OpenKernel:
    mov eax, 0x05                  
    mov ebx, kernel_filename       
    mov ecx, 0x00    

    int 0x80

    mov [file_desc], eax 

    ret

ReadKernel:
    mov eax, 0x03                   
    mov ebx, [file_desc]             
    mov ecx, kernel_buffer          
    mov edx, KERNEL_SIZE   

    int 0x80

    ret

CloseKernel:
    mov eax, 0x06                   
    mov ebx, [file_desc]  

    int 0x80

    ret

convert_crc32_to_hex:
    mov byte [esi], '0'
    mov byte [esi + 1], 'x'

    add esi, 0x02                      ; Move to the position after "0x"
    mov ecx, 0x08                      ; 8 hex digits

    mov edx, ebx                       ; Preserve CRC32 result in edx for shifting
convert_loop:
    mov eax, edx                       ; Copy CRC32 value from edx
    and eax, 0xF                       ; Mask for the last 4 bits
    mov al, [hex_digits + eax]         ; Convert to hex char
    mov [esi + ecx - 1], al            ; Store in result buffer
    shr edx, 4                         ; Shift edx right to next hex digit
    loop convert_loop                  ; Repeat for all 8 digits
    ret

WriteStdout:
    mov eax, 0x04                   
    mov ebx, 0x01                   
    mov ecx, stdout_result          
    mov edx, STDOUT_RESULT_SIZE    
    int 0x80 
    
    ret

_exit:
    mov eax, 0x01                  
    xor ebx, ebx                   
    int 0x80


%include "src/boot/stage2/include/crc32.asm"
