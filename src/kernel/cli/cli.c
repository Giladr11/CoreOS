#include "include/cli/cli.h"
#include "include/drivers/screen.h"

int strcmp(const char* str1, const char* str2)
{
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }

    // Return the difference between the first differing characters (or 0 if equal)
    return (unsigned char)(*str1) - (unsigned char)(*str2);
}

void handle_help()
{
    Enter_In_Cli();
    printf("\nThe Active Commands Are:");
    printf("\n------------------------------");
    printf("\n    clear - clears the screen");
    Disable_Enter_In_Cli();
}

void CliHandleInput(char input_buffer[INPUT_BUFFER_SIZE])
{
    if (strcmp(input_buffer, CLEAR_CMD) == 0)
    {
        printf("%s", input_buffer);
        clear_screen();
    }

    else if (strcmp(input_buffer, HELP_CMD) == 0)
    {
        handle_help();
    }

    else 
    {
        Enter_In_Cli();
        printf("\n%s : command not found", input_buffer);
        Disable_Enter_In_Cli();
    }
}
