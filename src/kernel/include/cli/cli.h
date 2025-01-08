#ifndef CLI_H
#define CLI_H

#define INPUT_BUFFER_SIZE 256

#define COMMANDS_TABLE_SIZE 5

#define CLEAR_CMD "clear"
#define HELP_CMD  "help"

typedef void (*CommandHandler)(void);

extern char input_buffer[INPUT_BUFFER_SIZE];
extern int input_index;

void CliHandleInput();

#endif
