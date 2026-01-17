; CLI Syscalls Executions

; HEAP Dump
global heap_dump_syscall
heap_dump_syscall:
    mov eax, 1

    int 0x80

    ret

; HEAP Alloc
global heap_alloc_syscall
heap_alloc_syscall:
    mov eax, 2
    mov ebx, [esp + 4]
    int 0x80
    ret

; HEAP Free
global heap_free_syscall
heap_free_syscall:
    mov eax, 3
    mov ebx, [esp + 4]
    int 0x80
    ret
   
; PC Speaker Beep
global beep_syscall
beep_syscall:
    mov eax, 4        ; SYS_BEEP
    mov ebx, 300      ; frequency = 1000 Hz
    mov ecx, 300      ; duration = 500 ms

    int 0x80          ; trigger the syscall

    ret               ; return to caller

; Playing song
global play_melody_syscall
play_melody_syscall:
    mov eax, 5        ; SYS_PLAY_MELODY

    int 0x80

    ret
