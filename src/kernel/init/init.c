#include "include/arch/x86/hal/hal.h"
#include "include/drivers/screen.h"
#include "include/drivers/keyboard.h"

void timer(Registers* regs)
{
    return;
}

void delay(int count) {
    volatile int i;
    for (i = 0; i < count; i++) {
        __asm__ volatile ("nop");
    }
}

void VGA_Kernel_Entry()
{
    printf("\n");
    printf("       //\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\ \n");
    delay(23000000); 
    printf("     //|||||__________||||_____________|||___________||||________|||||\\\\       \n");
    delay(23000000); 
    printf("    //                               HELLO :-)                          \\\\     \n");
    delay(23000000); 
    printf("   //====================================================================\\\\    \n");
    delay(23000000);
    printf("  //                 *** !WELCOME TO THE CoreOS KERNEL! ***               \\\\   \n");
    delay(23000000);    
    printf(" //========================================================================\\\\  \n");
    delay(23000000);    
    printf(" \\\\                             EXPLORE CoreOS -->                         //  \n");
    delay(23000000);    
    printf("   \\\\|||||__________||||______________|||______________||||__________|||||//   \n");
    printf("\n");
    
    Enable_Prefix_Line();
    printf("\n");
    Modify_VGA_Attr(0x0F);
}

void kernel_main() {
    HAL_Initialize();

    IRQ_RegistersHandler(0, timer); 
    IRQ_RegistersHandler(1, processKey);     

    VGA_Kernel_Entry();
}
   

    
