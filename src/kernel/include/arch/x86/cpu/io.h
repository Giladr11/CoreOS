#ifndef IO_H
#define IO_H

#include "include/stdint.h"

void __attribute__((cdecl)) outb(uint16_t port, uint8_t value);

uint8_t __attribute__((cdecl)) inb(uint16_t port);

uint8_t __attribute__((cdecl)) EnableInterrupts();

uint8_t __attribute__((cdecl)) DisableInterrupts();

void IOWait();

void __attribute__((cdecl)) KERNEL_PANIC();

#endif