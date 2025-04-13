#ifndef CLI_H
#define CLI_H

#define INPUT_BUFFER_SIZE 256

#define COMMANDS_TABLE_SIZE 5

#define CLEAR_CMD     "clear"
#define HELP_CMD      "help"
#define HEAP_DUMP_CMD "heap_dump"
#define ALLOC_CMD     "alloc"
#define FREE_CMD      "free"

typedef void (*CommandHandler)(void);

extern char input_buffer[INPUT_BUFFER_SIZE];
extern int input_index;

void CliHandleInput();

#endif
