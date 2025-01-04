#include "include/arch/x86/pic/irq.h"
#include "include/arch/x86/pic/pic.h"
#include "include/arch/x86/cpu/io.h"
#include "include/drivers/screen.h"
#include "include/drivers/keyboard.h"

#define PIC_REMAP_OFFSET 0x20

IRQHandler g_IRQHandlers[16];

void IRQ_Handler(Registers* regs)
{
    int irq = regs->interrupt - PIC_REMAP_OFFSET;
    
    uint8_t pic_isr = PIC_ReadInServiceRegister();
    uint8_t pic_irr = PIC_ReadIrqRequestRegister();

    if (g_IRQHandlers[irq] != NULL)
    {
        g_IRQHandlers[irq](regs);
    }

    else
    {
        printf("\nUnhandled IRQ %d\n", irq);
    }

    PIC_SendEndOfInterrupt(irq);
}

void IRQ_Initialize()
{
    PIC_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);
    PIC_Unmask(1);

    for (int i = 0; i < 16; i++)
    {
        ISR_RegistersHandler(PIC_REMAP_OFFSET + i, IRQ_Handler);
    }
    EnableInterrupts();
}

void IRQ_RegistersHandler(int irq, IRQHandler handler)
{
    g_IRQHandlers[irq] = handler;
}

