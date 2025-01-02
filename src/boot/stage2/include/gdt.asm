;GDT Settings
GDT_START:

    GDT_NULL:
    dd 0x0
    dd 0x0

    GDT_CODE:
    dw 0xFFFF                       ; Limit (lower 16 bits) = 0xFFFF (64 KB)
    dw 0x0000                       ; Base (lower 16 bits) = 0 (starting from 0) 
    db 0x00                         ; Base (middle 8 bits) = 0
    db 0b10011010                   ; Access byte: (present, executable, readable, privilege level 0)
    db 0b11001111                   ; Granularity: (limit is in 4 KB pages, 32-bit segment)
    db 0x00                         ; Base (upper 8 bits) = 0

    GDT_DATA:
    dw 0xFFFF                       ; Limit (lower 16 bits) = 0xFFFF (64 KB)
    dw 0x0000                       ; Base (lower 16 bits) = 0 (starting from 0) 
    db 0x00                         ; Base (middle 8 bits) = 0
    db 0b10010010                   ; Access byte: (present, writable, readable, privilege level 0)
    db 0b11001111                   ; Granularity: (limit is in 4 KB pages, 32-bit segment)
    db 0x00                         ; Base (upper 8 bits) = 0

GDT_END:

GDT_DESC:
    dw GDT_END - GDT_START - 1      
    dd GDT_START

CODE_SEG equ GDT_CODE - GDT_START
DATA_SEG equ GDT_DATA - GDT_START
