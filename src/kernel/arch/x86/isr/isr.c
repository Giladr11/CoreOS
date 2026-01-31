#include "include/arch/x86/isr/isr.h"
#include "include/arch/x86/idt/idt.h"
#include "include/arch/x86/cpu/io.h"
#include "include/drivers/screen.h"
#include "include/syscalls/syscalls.h"

ISRHandler g_ISRHandlers[256];

void ISR_InitializeGates();

void ISR_Initialize() {
    ISR_InitializeGates();
    for (int i = 0; i < 256; i++)
    {
        IDT_EnableGate(i);
    }
}

uint32_t __attribute__((cdecl)) ISR_Handler(Registers* regs) 
{
    if (g_ISRHandlers[regs->interrupt] != NULL) {
        g_ISRHandlers[regs->interrupt](regs);
    }

    //syscalls depends on eax
    else if (regs->interrupt == 0x80)
    {
         SyscallResult response = syscalls_handler(regs);

         if (response.status == 1)
         {
            Modify_VGA_Attr(0x02); // green color
            
            printf("\nsyscall %d was succesfully executed!", regs->eax);

            if (response.value != 0xFFFFFFFF){
                 return response.value;
            }
         }
         else {
            Modify_VGA_Attr(0x04); // red color

            printf("\nError: syscall %d has failed!", regs->eax);
         }
         return 0;
         Modify_VGA_Attr(0x0F); // white color
    }

    else if (regs->interrupt >= 0x20){
        Modify_VGA_Attr(0x04); // red color

        printf("\nError: Unhandled Interrupt %d!\n", regs->interrupt);

        Modify_VGA_Attr(0x0F); // white color
    }

    else {
        Enter_In_Cli();
        Modify_VGA_Attr(0x04); // red color

        printf("\nError: Unhandled exception %d %s\n", regs->interrupt, g_Exceptions[regs->interrupt]);
        
        printf("  eax=%x  ebx=%x  ecx=%x  edx=%x  esi=%x  edi=%x\n",
               regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi);

        printf("  esp=%x  ebp=%x  eip=%x  eflags=%x  cs=%x  ds=%x  ss=%x\n",
               regs->esp, regs->ebp, regs->eip, regs->eflags, regs->cs, regs->ds, regs->ss);

        printf("  interrupt=%x  errorcode=%x\n", regs->interrupt, regs->error);

        printf("KERNEL PANIC!\n");

        Modify_VGA_Attr(0x0F); // white color

        KERNEL_PANIC();
    }
}

void ISR_RegistersHandler(int interrupt, ISRHandler handler)
{
    g_ISRHandlers[interrupt] = handler;
    IDT_EnableGate(interrupt);
}

