#ifndef IRQ_H
#define IRQ_H

#include "include/arch/x86/isr/isr.h"

typedef void (*IRQHandler)(Registers* regs);

void Allow_Typing();

void IRQ_Initialize();

void IRQ_RegistersHandler(int irq, IRQHandler handler);

#endif