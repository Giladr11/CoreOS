#ifndef CLI_H
#define CLI_H

#define INPUT_BUFFER_SIZE 256

#define COMMANDS_TABLE_SIZE 7

#define CLEAR_CMD        "clear"
#define HELP_CMD         "help"
#define HEAP_DUMP_CMD    "heap_dump"
#define ALLOC_CMD        "alloc"
#define FREE_CMD         "free"
#define BEEP_CMD         "beep"
#define PLAY_MELODY      "play_melody"

typedef void (*CommandHandler)(void);

extern char input_buffer[INPUT_BUFFER_SIZE];
extern int input_index;

// SYSCALLS
void __attribute__((cdecl)) heap_dump_syscall();
uint32_t __attribute__((cdecl)) heap_alloc_syscall(uint32_t size);
void __attribute__((cdecl)) heap_free_syscall(uint32_t id);
void __attribute__((cdecl)) beep_syscall();
void __attribute__((cdecl)) play_melody_syscall();

void CliHandleInput();

#endif
