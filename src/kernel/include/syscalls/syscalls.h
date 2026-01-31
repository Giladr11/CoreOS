#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "include/arch/x86/isr/isr.h"
#include "include/heap/heap.h"

#define SYS_HEAP_DUMP   0x01 
#define SYS_HEAP_ALLOC  0x02 
#define SYS_HEAP_FREE   0x03 
#define SYS_BEEP        0x04 
#define SYS_PLAY_MELODY 0x05 

// Syscalls Functions
Block* sys_heap_dump(Registers* regs);
uint32_t sys_heap_alloc(Registers* regs);
void sys_heap_free(Registers* regs);

// Some fun syscalls
void sys_beep(Registers* regs);
void sys_play_melody(Registers* regs);

typedef struct {
    uint32_t value;     // actual return value
    void*    list;      // heap_dump blocks list
    uint8_t  status;    // 0 = fail, 1 = ok
} SyscallResult;

typedef void (*syscall_t)();

extern syscall_t syscall_table[256];

// Syscalls Handling Functions
void init_syscalls_table();
SyscallResult syscalls_handler(Registers* regs);

#endif