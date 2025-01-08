#ifndef HEAP_H
#define HEAP_H

#include "include/stdint.h"

#define HEAP_SIZE 1024 * 1024 // The size of 1 MB
#define HEAP_START 0x1000000 // Starting Address of the Heap

typedef struct Block{
    uint32_t size;
    struct Block *next;
    int free;
} Block;

extern uint32_t heap_start;     // The start of the Heap
extern uint32_t heap_end;       // The end of the Heap
extern Block *blocks_list;      // The Head of the Blocks list

void heap_init();
void heap_dump();
void *malloc(uint32_t size);
void free(void *ptr);

#endif
