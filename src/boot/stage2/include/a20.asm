;Enabling A20 Line
initA20:
    call A20_enable_msg

    in al, 0x92
    or al, 0x02
    out 0x92, al

    in al, 0x92
    test al, 0x02
    jz A20_failed

    call A20_success_msg

    ret

A20_enable_msg:    
    mov si, enable_messsage
    call print

    ret

A20_success_msg:    
    mov si, success_message
    call print

    ret

A20_failed:
    mov si, error_messsage
    call print
    
    jmp $


enable_messsage db "[+] Enabling A20 Line..."                , 0x0D, 0x0A, 0x0D, 0x0A, 0
success_message db "[+] Successfully Enabled A20 Line..."    , 0x0D, 0x0A, 0x0D, 0x0A, 0
error_messsage  db "[-][ERROR]: Failed to Enable A20 Line!"  , 0x0D, 0x0A, 0x0D, 0x0A, 0


%include "src/boot/print16.asm"
