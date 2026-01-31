#ifndef ISR_H
#define ISR_H

#include "include/stdint.h"

typedef struct {
    // In reverse order because the stack grows downwards

    uint32_t ds;                                             // data segment is pushed
    uint32_t edi, esi, ebp, kernel_esp, ebx, edx, ecx, eax;  // pusha
    uint32_t interrupt, error;                               // push interrupt, error
    uint32_t eip, cs, eflags, esp, ss;                       // pushed automatically by the CPU
    
} __attribute__((packed)) Registers;

typedef union {
    uint32_t u32;
    void* ptr;
} ISRResult;

typedef void (*ISRHandler)(Registers* regs);

static const char* const g_Exceptions[] = {
    "Divide by zero error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception ",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
};

void ISR_Initialize();
void ISR_RegistersHandler(int interrupt, ISRHandler handler);

#endif
