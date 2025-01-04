#ifndef SCREEN_H
#define SCREEN_H

#include "include/stdint.h"

void Enable_Prefix_Line();

void Disable_Prefix_Line();

void Modify_VGA_Attr(int attr);

void move_cursor();

void Enter_In_Cli();

void Disable_Enter_In_Cli();

void clear_screen();

void putc(char c);

void puts(const char *str);

void printi(int value);

void printh(uint32_t value);

void printf(const char* fmt, ...);

#endif