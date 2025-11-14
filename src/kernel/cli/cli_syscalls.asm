; CLI Syscalls Executions

; PC Speaker Beep
global beep_syscall
beep_syscall:
    mov eax, 11       ; SYS_BEEP
    mov ebx, 300      ; frequency = 1000 Hz
    mov ecx, 300      ; duration = 500 ms

    int 0x80          ; trigger the syscall

    ret               ; return to caller

; Playing song
global play_melody_syscall
play_melody_syscall:
    mov eax, 12     ; SYS_PLAY_MELODY

    int 0x80

    ret
    