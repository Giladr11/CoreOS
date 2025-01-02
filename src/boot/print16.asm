;Print (16-bit) Real-Mode
%ifndef PRINT16_ASM
%define PRINT16_ASM

print:
    mov ah, 0x0E

.printchar:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .printchar

.done:
    ret

%endif
