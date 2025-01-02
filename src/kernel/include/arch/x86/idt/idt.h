#ifndef IDT_H
#define IDT_H

#include "include/stdint.h"

#define IDT_ENTRIES 256
#define FLAG_SET(x, flag) x |= (flag)
#define FLAG_UNSET(x, flag) x &= ~(flag)

typedef struct {
    uint16_t BaseLow;               // Lower 16-Bits of ISR address
    uint16_t SegmentSelector;       // Code segment for this ISR
    uint8_t  Reserved;              // Set to 0 
    uint8_t  Flags;                 // Attributes
    uint16_t BaseHigh;              // Upper 16-Bits of ISR address

} __attribute__((packed)) IDTEntry;

typedef struct {
    uint16_t Limit;
    IDTEntry* IDTPtr;
    
} __attribute__((packed)) IDTDesc;

typedef enum {
    IDT_FLAG_GATE_TASK        = 0x5,
    IDT_FLAG_GATE_16BIT_INT   = 0x6,
    IDT_FLAG_GATE_16BIT_TRAP  = 0x7,
    IDT_FLAG_GATE_32BIT_INT   = 0xE,
    IDT_FLAG_GATE_32BIT_TRAP  = 0xF,

    IDT_FLAG_RING0            = (0 << 5),
    IDT_FLAG_RING1            = (1 << 5),
    IDT_FLAG_RING2            = (2 << 5),
    IDT_FLAG_RING3            = (3 << 5),

    IDT_FLAG_PRESENT          = 0x80,

} IDT_FLAGS;

void __attribute__((cdecl)) IDT_Load(IDTDesc* idtDesc);
void IDT_Initialize(void);
void IDT_EnableGate(int interrupt);
void IDT_DisableGate(int interrupt);
void IDT_SetGate(int interrupt, void* base, uint16_t segmentDesc, uint8_t flags);

#endif