#include "include/arch/x86/hal/hal.h"

void HAL_Initialize() 
{
    IDT_Initialize();
    ISR_Initialize();
    IRQ_Initialize();
}