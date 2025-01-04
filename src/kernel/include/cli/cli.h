#ifndef CLI_H
#define CLI_H

#define INPUT_BUFFER_SIZE 256

#define CLEAR_CMD "clear"
#define HELP_CMD  "help"

void CliHandleInput(char buffer[INPUT_BUFFER_SIZE]);

#endif
