#include "include/heap/heap.h"
#include "include/drivers/screen.h"
#include "include/arch/x86/cpu/io.h"


uint32_t heap_start = HEAP_START;
uint32_t heap_end   = HEAP_START + HEAP_SIZE;
Block *blocks_list = NULL;                                  // The Head of the Blocks list

void heap_init()
{
    blocks_list = (Block*)heap_start;
    blocks_list->size = HEAP_SIZE - sizeof(Block);          // Total size available
    blocks_list->next = NULL;                               // There aren't any other Blocks yet
    blocks_list->free = 1;                                  // Marks the entire Heap as free
}

void heap_dump()
{
    Block *current = blocks_list;

    Modify_VGA_Attr(0x06);
    printf("\n\nThe Current Heap:\n");
    printf("----------------------\n");

    int pos = 0;
    while (current != NULL)
    {
        printf("\nMetadata Addr: %p, User Memory Address: %p, Size: %d, Free: %d\n"
                , current
                , (void*)((uintptr_t)current + sizeof(Block))
                , current->size, current->free);
                
        pos++;
        current = current->next;
    }
}

// Function to split a block into two smaller blocks according the the requested size
void split_block(Block *block, uint32_t size)
{
    // Create a new block to represent the remaining free space after allocation
    Block *new_block = (Block*)((uintptr_t)block + sizeof(Block) + size);
    new_block->size = block->size - size - sizeof(Block);
    new_block->free = 1;
    new_block->next = block->next;

    // Update the current block to reflect the allocated size
    block->size = size;
    block->free = 0;
    
    // Update the next pointer of the allocated block to point to the new free block
    block->next = new_block;
}

// Function to Allocate Memory
void *malloc(uint32_t size)
{
    DisableInterrupts();

    Modify_VGA_Attr(0x06);

    if (size == 0)
    {
        EnableInterrupts();
        return NULL;
    }

    Block *current = blocks_list;
    while (current != NULL) 
    {   
        printf("\nChecking Block at %p with the size of %d Bytes\n", current, current->size);
        if (current->free == 1 && current->size >= size + sizeof(Block))
        {
            if (current->size > size + sizeof(Block))
            {
                split_block(current, size);                       // Split the block if it's large enough
            }

            current->free = 0;                                   // Mark the block as allocated
            EnableInterrupts();
            return (void*)((uintptr_t)current + sizeof(Block));  // Return the allocated memory (skipping the Block header)
        }

        current = current->next; 
    }

    printf("\nNo suitable Block found\n");

    EnableInterrupts();

    return NULL;  
}

// Function to Handle Fragmantation
void coalesce_free_blocks()
{
    Block *current = blocks_list;
    while (current != NULL && current->next != NULL)
    {
        if (current->free == 1 && current->next->free == 1)
        {
            current->size += sizeof(Block) + current->next->size; // Merge the blocks
            current->next = current->next->next;                  // Skip over the merged block
        }
        else
        {
            current = current->next;
        }
    }
}

// Function to Deallocate Memory
void free(void *ptr)
{
    DisableInterrupts();

    Modify_VGA_Attr(0x06);

    if (ptr == NULL)
    {   
        printf("\nAttempted to free a NULL pointer.\n");
        EnableInterrupts();
        return;
    }

    // Debugging the input pointer
    printf("\nFreeing pointer: %p\n", ptr);

    Block *block = (Block*)((uintptr_t)ptr - sizeof(Block));

    // Validate if the block exists in the heap
    Block *current = blocks_list;
    int block_found = 0;
    while (current != NULL)
    {
        if (current == block)
        {
            block_found = 1;
            break;
        }
        current = current->next;
    }

    if (!block_found)
    {
        Modify_VGA_Attr(0x04); // Set error attribute
        printf("\nError: Pointer %p is not a valid allocated block.\n", ptr);
        EnableInterrupts();
        return;
    }

    printf("\nFreeing Block at %p with Size %d\n", block, block->size);
    
    block->free = 1;        // Mark the block as free
    
    coalesce_free_blocks(); // Merge adjacent free blocks

    printf("The Heap After Coalescing Free Blocks:\n");
    current = blocks_list;
    while (current != NULL)
    {
        printf("\n  Block at %p, Size: %d, Free: %d\n", current, current->size, current->free);
        current = current->next;
    }

    Modify_VGA_Attr(0x02); // Set success attribute
    printf("\nSuccessfully Freed %p\n", ptr);
    EnableInterrupts();
}


