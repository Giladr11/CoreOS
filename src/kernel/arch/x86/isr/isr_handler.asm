[BITS 32]

extern ISR_Handler

temp_eax_res resd 1
; cpu pushes to the stack: (ss, esp, eflags, cs, eip) to save the interrupted program context

%macro ISR_NOERROR_CODE 1

global ISR%1:
ISR%1:
    push 0              ; push dummy error code
    push %1             ; push interrupt number
    jmp isr_common

%endmacro 

%macro ISR_ERROR_CODE 1

global ISR%1:
ISR%1:
    push %1
    jmp isr_common

%endmacro

%include "src/kernel/include/arch/x86/isr/isrs_gen.inc"

isr_common:
    pusha               ; pushes edi, esi, ebp, esp, ebx, edx, ecx, eax (general purpose regs)

    xor eax, eax        ; push ds
    mov ax, ds  
    push eax    

    mov ax, 0x10        ; gdt ds
    mov ds, ax  
    mov es, ax   
    mov fs, ax  
    mov gs, ax 

    push esp            ; pass pointer to stack to C, so we can access all the pushed info

    call ISR_Handler

    add esp, 4
    
    mov [temp_eax_res], eax        ; set isr handler response value to ebx

    pop eax             ; restore old segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                ; pop general purpose regs

    mov eax, [temp_eax_res]        ; set isr handler response value from ebx back to eax so cdecl could read result

    add esp, 8          ; remove error code and interrupt number

    iret                ; return from isr and pop: (ss, esp, eflags, cs, eip)