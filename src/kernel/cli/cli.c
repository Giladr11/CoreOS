#include "include/stdint.h"
#include "include/cli/cli.h"
#include "include/heap/heap.h"
#include "include/drivers/screen.h"

int alloc_active = 0;
int free_active =  0;

int strcmp(const char* str1, const char* str2)
{
    while (*str1 && (*str1 == *str2)) 
    {
        str1++;
        str2++;
    }

    return (unsigned char)(*str1) - (unsigned char)(*str2);
}

void rtrim(char *str) {
    if (!str) return;  // safety check

    // Find the end of the string
    char *end = str;
    while (*end != '\0') {
        end++;
    }

    // Move backwards and remove spaces
    while (end > str && *(end - 1) == ' ') {
        end--;
    }

    *end = '\0';
}


// Handle clear screen command
void handle_clear()
{
    clear_screen();
}

// Handle help command
void handle_help()
{
    Enter_In_Cli();
    Modify_VGA_Attr(0x0E); // yellow
    printf("\nThe Available Commands:");
    printf("\n-----------------------");
    printf("\nclear       - Clears the screen");
    printf("\nheap_dump   - Displays the current Heap");
    printf("\nalloc       - Allocates memory to the Heap");
    printf("\nfree        - Frees up memory from the Heap");
    printf("\nbeep        - Executes A syscall to make the PC speaker beep");
    printf("\nplay_melody - Executes A syscall to play a melody");
    Disable_Enter_In_Cli();
}

// Converts String to Integer
uint32_t atoi(const char* str) {
    uint32_t result = 0;
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

uintptr_t hex_to_int(const char* hex_str) {
    uintptr_t result = 0;

    // If the string starts with "0x" or "0X", skip the prefix
    if (hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
        hex_str += 2;
    }

    // Iterate through each character
    while (*hex_str) {
        char c = *hex_str++;
        result <<= 4; // Shift result left by 4 bits to make room for the next digit

        if (c >= '0' && c <= '9') {
            result += c - '0'; // Convert '0' to '9'
        } else if (c >= 'a' && c <= 'f') {
            result += 10 + (c - 'a'); // Convert 'a' to 'f'
        } else if (c >= 'A' && c <= 'F') {
            result += 10 + (c - 'A'); // Convert 'A' to 'F'
        } else {
            // Handle invalid character
            return 0; // You could also return an error code or handle this differently
        }
    }

    return result;
}

void* hex_to_pointer(const char* hex_str) {
    return (void*)hex_to_int(hex_str);
}

void handle_display_heap()
{
    Enter_In_Cli();
    Modify_VGA_Attr(0x06); // brown color


    Block* current =  heap_dump_syscall();

    while (current != NULL)
    {
        printf("\nId: %d, Metadata Addr: %p, User Memory Addr: %p, Size: %d, Free: %d\n"
                 , current->id
                 , current
                 , (void*)((uintptr_t)current + sizeof(Block))
                 , current->size, current->free);

        current = current->next;
    }
    
    Disable_Enter_In_Cli();
}

// Handle Alloc Input
void handle_alloc_input()
{
    Enter_In_Cli();

    Modify_VGA_Attr(0x0B);
    printf("\nEnter the Number of Bytes to Allocate:");
    Modify_VGA_Attr(0x03);

    alloc_active = 1;

    input_start = cursor_position;
}

// Handle Allocation
void handle_allocation()
{
    uint32_t size;

    size = atoi(input_buffer);

    uint32_t id = heap_alloc_syscall(size);

    if (id)
    {   
        Modify_VGA_Attr(0x02);
        printf("\nSuccessfully Allocated %d Bytes into block Id: %d\n", size, id);
    }
    else {
        Modify_VGA_Attr(0x04);
        printf("\nMemory Allocation Has Failed!\n");
    }

    Disable_Enter_In_Cli();
}

// Handle Deallocation Input
void handle_dealocation_input()
{
    Enter_In_Cli();

    Modify_VGA_Attr(0x0B);
    printf("\nEnter the Id of the Block to Free:");
    Modify_VGA_Attr(0x03);

    free_active = 1;

    input_start = cursor_position;
}

// Handle Deallocation
void handle_dealocation()
{
    // Instead -> check the memory block id received as input
    void* ptr = (void*)hex_to_pointer(input_buffer);

    if (ptr) {
        //free(ptr);
    } 

    Disable_Enter_In_Cli();
}

void handle_beep_syscall()
{
    Enter_In_Cli();

    beep_syscall();

    Disable_Enter_In_Cli();
}

void handle_play_melody_syscall()
{
    Enter_In_Cli();

    play_melody_syscall();

    Disable_Enter_In_Cli();
}

// Handle Unknown Commands
void handle_unknown(char input_buffer[INPUT_BUFFER_SIZE])
{
    Enter_In_Cli();
    printf("\n%s : command not found", input_buffer);
    Disable_Enter_In_Cli();
}

typedef struct {
    const char* command;
    CommandHandler handler;
} Commands;

Commands commands_table[] = 
{
    {CLEAR_CMD     , handle_clear              },
    {HELP_CMD      , handle_help               },
    {HEAP_DUMP_CMD , handle_display_heap       },
    {ALLOC_CMD     , handle_alloc_input        },
    {FREE_CMD      , handle_dealocation_input  },
    {BEEP_CMD      , handle_beep_syscall       },
    {PLAY_MELODY   , handle_play_melody_syscall}
};

void CliHandleInput()
{
    rtrim(input_buffer);

    if (alloc_active == 1)
    {
        handle_allocation();
        alloc_active = 0;
        return;
    }
    if (free_active == 1)
    {
        handle_dealocation();
        free_active = 0;
        return;
    }
    for (int i = 0; i < COMMANDS_TABLE_SIZE; i++)
    {   
        if (strcmp(input_buffer, commands_table[i].command) == 0)
        {
            commands_table[i].handler();
            return;
        }
    }
    handle_unknown(input_buffer);
}
