#ifndef HAL_H
#define HAL_H

#include "include/arch/x86/idt/idt.h"
#include "include/arch/x86/isr/isr.h"
#include "include/arch/x86/cpu/io.h"
#include "include/arch/x86/pic/irq.h"

void HAL_Initialize();

#endif