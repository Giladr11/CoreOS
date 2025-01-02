[BITS 32]

global IDT_Load

IDT_Load:
    ;Setting up a new stack frame
    push ebp
    mov ebp, esp

    ;Loading IDT
    mov eax, [ebp + 8]
    lidt [eax]

    ;Returning to the previous stack frame
    mov esp, ebp
    pop ebp
    ret
