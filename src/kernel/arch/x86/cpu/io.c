#include "include/arch/x86/cpu/io.h"

#define UNUSED_PORT 0x80

void IOWait() 
{
    outb(UNUSED_PORT, 0);
}