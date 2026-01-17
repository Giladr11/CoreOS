#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "include/arch/x86/isr/isr.h"

#define SYS_HEAP_DUMP   0x01 // 1
#define SYS_HEAP_ALLOC  0x02 // 2
#define SYS_HEAP_FREE   0x03 // 3
#define SYS_BEEP        0x04 // 4
#define SYS_PLAY_MELODY 0x05 // 5

// Syscalls Functions
void sys_heap_dump(Registers* regs);
uint32_t sys_heap_alloc(Registers* regs);
void sys_heap_free(Registers* regs);
void sys_beep(Registers* regs);
void sys_play_melody(Registers* regs);

typedef struct {
    uint32_t value;     // actual return  value
    uint8_t  status;    // 0 = fail, 1 = ok
} SyscallResult;

typedef void (*syscall_t)();

extern syscall_t syscall_table[256];

// Syscalls Handling Functions
void init_syscalls_table();
SyscallResult syscalls_handler(Registers* regs);

#endif