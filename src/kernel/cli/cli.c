#include "include/cli/cli.h"
#include "include/drivers/screen.h"

void CliHandleInput(char input_buffer[INPUT_BUFFER_SIZE])
{
    printf("%s", input_buffer);
}
