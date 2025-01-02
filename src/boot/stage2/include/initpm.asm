;Protected Mode Initialization
section .text
LoadPM:    
    call initA20

    call print_setting_gdt

    call print_pm_msg

    call print_kernel_exe_msg

    call pause

    call SetVideoMode

    cli 

    lgdt [GDT_DESC]

    mov eax, cr0
    or eax, 0x01                 
    mov cr0, eax                

    jmp CODE_SEG:PModeMain     

[BITS 32]     
PModeMain:
    mov eax, cr0
    test eax, 0x1
    jz hlting

    mov ax, DATA_SEG                
    mov ds, ax                      
    mov es, ax                      
    mov ss, ax                      
    mov fs, ax                      
    mov gs, ax                      

    mov esp, 0x9C00 

    mov esi, PHYSICAL_KERNEL_ADDR
    mov edi, KERNEL_START_ADDR
    mov ecx, KERNEL_SIZE

    rep movsb

    jmp CODE_SEG:KERNEL_START_ADDR

[BITS 16]
SetVideoMode:
    mov ax, 0x03

    int 0x10

    ret

pause:
    mov bx, 0x2FFF            
outer_loop:
    mov cx, 0xFFFF          
inner_loop:
    loop inner_loop         
    dec bx                  
    jnz outer_loop  

    ret

print_setting_gdt:
    mov si,setting_gdt_message
    call print

    ret

print_pm_msg:
    mov si, protected_mode_message
    call print

    ret

print_kernel_exe_msg:
    mov si, kernel_execution_message
    call print

    ret

hlting:
    hlt


protected_mode_message   db "[+] Transitioning into Protected Mode..." , 0x0D, 0x0A, 0x0D, 0x0A, 0
setting_gdt_message      db "[+] Setting Up GDT..."                    , 0x0D, 0x0A, 0x0D, 0x0A, 0
kernel_execution_message db "[+] Initiating Kernel Execution..."       , 0x0D, 0x0A, 0x0D, 0x0A, 0


%include "src/boot/stage2/include/gdt.asm"
%include "src/boot/stage2/include/a20.asm"
%include "src/boot/print16.asm"
