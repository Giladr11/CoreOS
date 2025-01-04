#ifndef CLI_H
#define CLI_H

#define CLEAR_CMD "clear"
#define HELP_CMD  "help"

#define INPUT_BUFFER_SIZE 256

void CliHandleInput(char buffer[INPUT_BUFFER_SIZE]);

#endif
