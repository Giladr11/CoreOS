#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "include/arch/x86/isr/isr.h"

#define SYS_BEEP 0x0b
#define SYS_PLAY_MELODY 0x0c

// Syscalls Functions
void sys_beep(Registers* regs);
void sys_play_melody();

typedef void (*syscall_t)();

extern syscall_t syscall_table[256];

// Syscalls Handling Functions
void init_syscalls_table();
uint32_t syscalls_handler(Registers* regs);

#endif