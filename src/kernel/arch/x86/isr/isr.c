#include "include/arch/x86/isr/isr.h"
#include "include/arch/x86/idt/idt.h"
#include "include/arch/x86/cpu/io.h"
#include "include/drivers/screen.h"

ISRHandler g_ISRHandlers[256];

void ISR_InitializeGates();

void ISR_Initialize() {
    ISR_InitializeGates();
    for (int i = 0; i < 256; i++)
    {
        IDT_EnableGate(i);
    }
}

void __attribute__((cdecl)) ISR_Handler(Registers* regs) 
{
    if (g_ISRHandlers[regs->interrupt] != NULL) {
        g_ISRHandlers[regs->interrupt](regs);
    }

    //syscalls depends on eax
    else if (regs->interrupt == 128)
    {
         printf("0x80 is a Syscalls Interrupt\n");
    }

    else if (regs->interrupt >= 32){
        printf("Unhandled Interrupt %d!\n", regs->interrupt);
    }

    else {
        Enter_In_Cli();

        printf("Unhandled exception %d %s\n", regs->interrupt, g_Exceptions[regs->interrupt]);
        
        printf("  eax=%x  ebx=%x  ecx=%x  edx=%x  esi=%x  edi=%x\n",
               regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi);

        printf("  esp=%x  ebp=%x  eip=%x  eflags=%x  cs=%x  ds=%x  ss=%x\n",
               regs->esp, regs->ebp, regs->eip, regs->eflags, regs->cs, regs->ds, regs->ss);

        printf("  interrupt=%x  errorcode=%x\n", regs->interrupt, regs->error);

        printf("KERNEL PANIC!\n");
        KERNEL_PANIC();
    }
}

void ISR_RegistersHandler(int interrupt, ISRHandler handler)
{
    g_ISRHandlers[interrupt] = handler;
    IDT_EnableGate(interrupt);
}

