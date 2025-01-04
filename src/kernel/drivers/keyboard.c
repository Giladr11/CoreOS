#include "include/drivers/keyboard.h"
#include "include/drivers/screen.h"
#include "include/cli/cli.h"
#include "include/arch/x86/cpu/io.h"

int g_CapsActive = 0;
int g_shiftActive = 0;

char input_buffer[INPUT_BUFFER_SIZE];
int input_index = 0;

typedef struct {
    int scancode;
    char lowerCase;
    char upperCase;
    char shiftSymbol;
} Keyboard;

Keyboard g_Keyboard[48] = {
    {0x02, '1', '1', '!'}, {0x03, '2', '2', '@'}, {0x04, '3', '3', '#'}, {0x05, '4', '4', '$'},
    {0x06, '5', '5', '%'}, {0x07, '6', '6', '^'}, {0x08, '7', '7', '&'}, {0x09, '8', '8', '*'},
    {0x0A, '9', '9', '('}, {0x0B, '0', '0', ')'}, {0x0C, '-', '-', '_'}, {0x0D, '=', '=', '+'},
    {0x10, 'q', 'Q', 'Q'}, {0x11, 'w', 'W', 'W'}, {0x12, 'e', 'E', 'E'}, {0x13, 'r', 'R', 'R'},
    {0x14, 't', 'T', 'T'}, {0x15, 'y', 'Y', 'Y'}, {0x16, 'u', 'U', 'U'}, {0x17, 'i', 'I', 'I'},
    {0x18, 'o', 'O', 'O'}, {0x19, 'p', 'P', 'P'}, {0x1A, '[', '[','{'}, {0x1B, ']', ']', '}'}, 
    {0x1E, 'a', 'A', 'A'}, {0x1F, 's', 'S', 'S'}, {0x20, 'd', 'D', 'D'}, {0x21, 'f', 'F', 'F'},
    {0x22, 'g', 'G', 'G'}, {0x23, 'h', 'H', 'H'}, {0x24, 'j', 'J', 'J'}, {0x25, 'k', 'K', 'K'},
    {0x26, 'l', 'L', 'L'}, {0x27, ';', ';', ':'}, {0x28, '\'', '\'', '"'}, {0x2C, 'z', 'Z', 'Z'}, 
    {0x2D, 'x', 'X', 'X'}, {0x2E, 'c', 'C', 'C'}, {0x2F, 'v', 'V', 'V'}, {0x30, 'b', 'B', 'B'}, 
    {0x31, 'n', 'N', 'N'}, {0x32, 'm', 'M', 'M'}, {0x33, ',', ',', '<'}, {0x34, '.', '.', '>'}, 
    {0x35, '/', '/', '?'}, {0x39, ' ', ' ', ' '}, {0x1C, '\n', '\n', '\n'}, {0x0E, '\b', '\b', '\b'}
    //, {0x0F, '\t', '\t', '\t'}
};

char GeneralKey(int scancode)
{
    char key = '\0';
    for (int i = 0; i < 48; i++)
    {
        if (g_Keyboard[i].scancode == scancode)
        {   
            if (g_shiftActive == 1)
            {
                key = g_Keyboard[i].shiftSymbol;
            }

            else if (g_CapsActive == 1)
            {
                key = g_Keyboard[i].upperCase;
            }

            else
            {
                key = g_Keyboard[i].lowerCase; 
            }
            break;
        }
    }
    return key;
}

void processKey()
{
    uint8_t scancode = inb(0x60);

    if (scancode == 0x2A || scancode == 0x36)
    {
        g_shiftActive = 1;
        return;
    }

    else if (scancode == 0xAA || scancode == 0xB6)
    {
        g_shiftActive = 0;
        return;
    }

    if (scancode == 0x3A) 
    {
        g_CapsActive = !g_CapsActive;  
        return;
    }

    if (scancode == 0xBA) 
    {
        return;
    }

    char key = GeneralKey(scancode);
    
    if (key == '\0')
    {
        return;
    }

    switch (key)
    {   
        case '\b':
            if (input_index > 0)
            {
                input_index--;
                input_buffer[input_index] = '\0';
                putc('\b');
            }
            break;
        
        case '\n':
            input_buffer[input_index] = '\0';
            input_index = 0;

            CliHandleInput(input_buffer);

            putc('\n');

            break;

        case ' ':
            if (input_index < INPUT_BUFFER_SIZE - 1)
            {
                input_buffer[input_index++] = ' ';
                input_buffer[input_index] = '\0';
                putc(' ');
            }
            break;

        // case '\t':
        //     putc('\t');
        //     break;

        default:
            if (input_index < INPUT_BUFFER_SIZE - 1)
            {
                input_buffer[input_index++] = key;
                input_buffer[input_index] = '\0';
            }
            putc(key);
            break;
    }
}
