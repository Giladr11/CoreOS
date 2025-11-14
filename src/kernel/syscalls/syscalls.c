#include "include/syscalls/syscalls.h"
#include "include/arch/x86/cpu/io.h"
#include "include/stdint.h"

#define PIT_CHANNEL_2 0x42
#define PIT_COMMAND   0x43
#define SPEAKER_PORT  0x61
#define PIT_FREQUENCY 1193180
#define DELAY_MULTIPLIER 280000  // tuned for QEMU

//////////////////////////////Mission Impossible Melody////////////////////////////////////
struct Note {
    uint32_t freq; // Hz
    uint32_t ms;   // duration in milliseconds
};

struct Note melody[] = {
    {392, 250},{392,250},{392,250},{392,250},  
    {466,250},{262,250},{392,250},{392,250},   
    {392,250},{392,250},{349,250},{370,250},   
    {392,250},{0,500},                         // <-- long rest
    {392,250},{392,500},{392,250},{392,250},   
    {466,250},{262,250},{392,250},{392,250},   
    {392,250},{392,250},{349,250},{370,250},   
    {392,500},{0,500}                          
};

#define MELODY_LENGTH (sizeof(melody)/sizeof(melody[0]))

////////////////////////////////////////////////////////////////////

syscall_t syscall_table[256] = {0};

void init_syscalls_table() 
{
    syscall_table[SYS_BEEP] = sys_beep;
    syscall_table[SYS_PLAY_MELODY] = sys_play_melody;
    //...
}

uint32_t syscalls_handler(Registers* regs)
{
    void* syscall_func_ptr = (void*)syscall_table[regs->eax]; // Assigning the syscall function to a pointer

    if (syscall_func_ptr == NULL)
    {
        return 0;
    }

    ((void(*)())syscall_func_ptr)(regs); // Executing the syscall function

    return 1;
}

void wait_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 280000; i++);
}

void sys_beep(Registers* regs) {
    uint32_t freq = regs->ebx;
    uint32_t ms   = regs->ecx;

    if (freq < 20) freq = 20;
    if (freq > 20000) freq = 20000;

    uint16_t divider = PIT_FREQUENCY / freq;

    outb(PIT_COMMAND, 0xB6);
    outb(PIT_CHANNEL_2, divider & 0xFF);
    outb(PIT_CHANNEL_2, divider >> 8);

    uint8_t tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp | 0x03); // enable speaker

    wait_ms(ms); // <-- use wait_ms

    tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp & ~0x03); // disable speaker
}

void sys_play_melody(Registers* regs) {
    (void)regs;

    for (int i = 0; i < MELODY_LENGTH; i++) {
        if (melody[i].freq == 0) {
            wait_ms(melody[i].ms);  // rest
        } else {
            Registers fake;
            fake.ebx = melody[i].freq;
            fake.ecx = melody[i].ms;
            sys_beep(&fake);

            // small gap (~12.5% of note duration)
            wait_ms(melody[i].ms / 8);
        }
    }
}
