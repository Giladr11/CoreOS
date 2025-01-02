#include "include/arch/x86/idt/idt.h"

IDTEntry g_IDT[IDT_ENTRIES];

IDTDesc g_IDTDesc = { sizeof(g_IDT) - 1, g_IDT }; 

void IDT_Initialize() {
    IDT_Load(&g_IDTDesc);
}

void IDT_EnableGate(int interrupt) {
    FLAG_SET(g_IDT[interrupt].Flags, IDT_FLAG_PRESENT);
}

void IDT_DisableGate(int interrupt) {
    FLAG_UNSET(g_IDT[interrupt].Flags, IDT_FLAG_PRESENT);
}

void IDT_SetGate(int interrupt, void* base, uint16_t segmentDesc, uint8_t flags) {
    g_IDT[interrupt].BaseLow = (uint32_t)base;
    g_IDT[interrupt].SegmentSelector = segmentDesc;
    g_IDT[interrupt].Reserved = 0;
    g_IDT[interrupt].Flags = flags;
    g_IDT[interrupt].BaseHigh = ((uint32_t)base >> 16) & 0xFFFF;
}