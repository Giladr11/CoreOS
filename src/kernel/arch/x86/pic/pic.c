#include "include/arch/x86/pic/pic.h" 
#include "include/arch/x86/cpu/io.h"   

#define PIC1_COMMAND_PORT           0x20                            // I/O port for PIC1 (Primary PIC command register)
#define PIC1_DATA_PORT              0x21                            // I/O port for PIC1 (Primary PIC data register)
#define PIC2_COMMAND_PORT           0xA0                            // I/O port for PIC2 (Secondary PIC command register)
#define PIC2_DATA_PORT              0xA1                            // I/O port for PIC2 (Secondary PIC data register)

// Enumeration for the Initialization Command Word 1 (ICW1) flags
enum {
    PIC_ICW1_ICW4           = 0x01,                                 // Must be set to indicate ICW4 follows
    PIC_ICW1_SINGLE         = 0x02,                                 // Set to single PIC mode (no cascading)
    PIC_ICW1_INTERVAL4      = 0x04,                                 // Set for 4 byte interval for cascading
    PIC_ICW1_LEVEL          = 0x08,                                 // Edge triggering for IRQs
    PIC_ICW1_INITIALIZE     = 0x10                                  // Initialize PIC (set this flag)
} PIC_ICW1;

// Enumeration for the Initialization Command Word 4 (ICW4) flags
enum {
    PIC_ICW4_8086           = 0x1,                                  // Set for 8086/88 mode (instead of 80486 mode)
    PIC_ICW4_AUTO_EOI       = 0x2,                                  // Auto End-of-Interrupt (EOI) mode
    PIC_ICW4_BUFFER_MASTER  = 0x4,                                  // Master PIC in buffered mode
    PIC_ICW4_BUFFER_SLAVE   = 0x0,                                  // Slave PIC in buffered mode
    PIC_ICW4_BUFFERRED      = 0x8,                                  // Buffered mode (both PICs)
    PIC_ICW4_SFNM           = 0x10                                  // Special Fully Nested Mode
} PIC_ICW4;

// Enumeration for the PIC commands
enum {
    PIC_CMD_END_OF_INTERRUPT    = 0x20,                             // Command to signal end of interrupt (EOI)
    PIC_CMD_READ_IRR            = 0x0A,                             // Command to read Interrupt Request Register (IRR)
    PIC_CMD_READ_ISR            = 0x0B,                             // Command to read In-Service Register (ISR)
} PIC_CMD;

// Function to configure the PICs
void PIC_Configure(uint8_t offsetPic1, uint8_t offsetPic2)
{
    // Send the ICW1 for both PIC1 and PIC2 to initialize them
    outb(PIC1_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);  // Set ICW1 for PIC1 (initialize)
    IOWait();  // Wait for I/O operation to complete
    outb(PIC2_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);  // Set ICW1 for PIC2 (initialize)
    IOWait();  

    // Send the offsets for PIC1 and PIC2
    outb(PIC1_DATA_PORT, offsetPic1);                              // Set the vector offset for PIC1
    IOWait();                              
    outb(PIC2_DATA_PORT, offsetPic2);                              // Set the vector offset for PIC2
    IOWait(); 

    // Set the cascade identity of the PICs
    outb(PIC1_DATA_PORT, 0x4);                                     // PIC1 is connected to IRQ2 (cascade line)
    IOWait();                                  
    outb(PIC2_DATA_PORT, 0x2);                                     // PIC2 is connected to IRQ1 (cascade line)
    IOWait(); 

    // Set PICs to 8086 mode (for compatibility)
    outb(PIC1_DATA_PORT, PIC_ICW4_8086);                           // Set PIC1 to 8086 mode
    IOWait();                          
    outb(PIC2_DATA_PORT, PIC_ICW4_8086);                           // Set PIC2 to 8086 mode
    IOWait();  

    // Finalize the initialization
    outb(PIC1_DATA_PORT, 0);                                       // Send ICW3 to indicate no IRQ3, IRQ4, etc.
    IOWait();                                      
    outb(PIC2_DATA_PORT, 0);                                       // Send ICW3 for PIC2
    IOWait();  
}

// Function to send End-of-Interrupt (EOI) signal for a specific IRQ
void PIC_SendEndOfInterrupt(int irq)
{
    if (irq >= 8)                                                  // If the IRQ is >= 8, it corresponds to the secondary PIC (PIC2)
    {
        outb(PIC2_COMMAND_PORT, PIC_CMD_END_OF_INTERRUPT);         // Send EOI to PIC2
    }      
    outb(PIC1_COMMAND_PORT, PIC_CMD_END_OF_INTERRUPT);             // Send EOI to PIC1
}

// Function to disable all interrupts on both PICs (by masking all IRQs)
void PIC_Disable()
{
    outb(PIC1_DATA_PORT, 0xFF);                                    // Mask all IRQs on PIC1
    IOWait();                                
    outb(PIC2_DATA_PORT, 0xFF);                                    // Mask all IRQs on PIC2
    IOWait();  
}

// Function to mask a specific IRQ (disable it)
void PIC_Mask(int irq)
{
    uint8_t port;

    // Determine which PIC (1 or 2) the IRQ belongs to
    if (irq < 8) 
    {
        port = PIC1_DATA_PORT;                                     // IRQ < 8 means it belongs to PIC1
    }
    else
    {
        irq -= 8;                                                  // Adjust IRQ number for PIC2
        port = PIC2_DATA_PORT;                                     // IRQ >= 8 means it belongs to PIC2
    }

    // Read the current mask, then mask the specific IRQ by setting the corresponding bit
    uint8_t mask = inb(port);                                      // Read the current mask from the specified port
    outb(port, mask | (1 << irq));                                 // Set the bit corresponding to the IRQ to 1 (disable it)
}   

// Function to unmask a specific IRQ (enable it)
void PIC_Unmask(int irq)
{
    uint8_t port;

    // Determine which PIC (1 or 2) the IRQ belongs to
    if (irq < 8) 
    {
        port = PIC1_DATA_PORT;                                     // IRQ < 8 means it belongs to PIC1
    }
    else
    {
        irq -= 8;                                                  // Adjust IRQ number for PIC2
        port = PIC2_DATA_PORT;                                     // IRQ >= 8 means it belongs to PIC2
    }

    // Read the current mask, then unmask the specific IRQ by clearing the corresponding bit
    uint8_t mask = inb(port);                                      // Read the current mask from the specified port
    outb(port, mask & ~(1 << irq));                                // Clear the bit corresponding to the IRQ (enable it)
}

// Function to read the Interrupt Request Register (IRR) from both PICs
uint16_t PIC_ReadIrqRequestRegister()
{
    // Send command to both PICs to read the IRR
    outb(PIC1_COMMAND_PORT, PIC_CMD_READ_IRR);
    outb(PIC2_COMMAND_PORT, PIC_CMD_READ_IRR);

    // Combine the 8-bit values from both PICs into a 16-bit value and return
    return ((uint16_t)inb(PIC2_COMMAND_PORT)) | (((uint16_t)inb(PIC2_COMMAND_PORT)) << 8);
}

// Function to read the In-Service Register (ISR) from both PICs
uint16_t PIC_ReadInServiceRegister()
{
    // Send command to both PICs to read the ISR
    outb(PIC1_COMMAND_PORT, PIC_CMD_READ_ISR);
    outb(PIC2_COMMAND_PORT, PIC_CMD_READ_ISR);

    // Combine the 8-bit values from both PICs into a 16-bit value and return
    return ((uint16_t)inb(PIC2_COMMAND_PORT)) | (((uint16_t)inb(PIC2_COMMAND_PORT)) << 8);
}
