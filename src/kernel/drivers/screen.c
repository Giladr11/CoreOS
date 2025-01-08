#include "include/drivers/screen.h"
#include "include/arch/x86/cpu/io.h"

#define VGA_ADDRESS 0xB8000
#define VGA_ROWS 25
#define VGA_COLS 80

int vga_attr = 0x0C; // Light Red
int prefix_line = 0;
char* dir = "home";

char* vga_buffer = (char*)VGA_ADDRESS;
int cursor_position = 0;
int input_start = 0;

int enter_in_cli = 0;

void Modify_VGA_Attr(int attr)
{
    vga_attr = attr;
}

void Enable_Prefix_Line()
{
    prefix_line = 1;
}

void Disable_Prefix_Line()
{
    prefix_line = 0;
}

void move_cursor() 
{
    unsigned short position = cursor_position;
    
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

void Enter_In_Cli()
{
    enter_in_cli = 1;
}

void Disable_Enter_In_Cli()
{
    enter_in_cli = 0;
}

void print_prefix_line()
{
    uint8_t original_attr = vga_attr;

    Modify_VGA_Attr(0x0A); // Light green
    printf("Gilad11@Gilad11-CoreOS");
    Modify_VGA_Attr(0x0F); // White
    putc(':');
    Modify_VGA_Attr(0x09); // Light blue
    putc('/');
    printf("%s", dir);
    Modify_VGA_Attr(0x0F); // Light gray
    printf("$ ");

    input_start = cursor_position;
}

// Set every position in the VGA buffer to a space character with the current attribute
void clear_screen() {
    for (int i = 0; i < VGA_ROWS * VGA_COLS; i++) {
        vga_buffer[i * 2] = ' '; 
        vga_buffer[i * 2 + 1] = vga_attr;
    }
    cursor_position = 0;
    move_cursor();

    Enter_In_Cli();
    Modify_VGA_Attr(0x05);
    printf("Type 'help' To See The Available Commands\n");
    Disable_Enter_In_Cli();
}

void putc(char c)
{
    // Handle scrolling when cursor exceeds screen bounds
    if (cursor_position >= VGA_ROWS * VGA_COLS) {
        // Scroll the screen
        for (int i = 0; i < (VGA_ROWS - 1) * VGA_COLS; i++)
        {
            vga_buffer[i * 2] = vga_buffer[(i + VGA_COLS) * 2];
            vga_buffer[i * 2 + 1] = vga_buffer[(i + VGA_COLS) * 2 + 1];
        }
        // Clear the last line
        for (int i = (VGA_ROWS - 1) * VGA_COLS; i < VGA_ROWS * VGA_COLS; i++)
        {
            vga_buffer[i * 2] = ' ';
            vga_buffer[i * 2 + 1] = vga_attr;
        }   

        // Reset cursor to start of the last line
        cursor_position = (VGA_ROWS - 1) * VGA_COLS;
    }

    if (c == '\n') 
    {
        cursor_position += VGA_COLS - (cursor_position % VGA_COLS); // Move to next line
        if (prefix_line == 1 && enter_in_cli == 0)
        {
            print_prefix_line();
        }
    }
    else if (c == '\b')  // Backspace
    {
        if (cursor_position > input_start) 
        {
            cursor_position--;
            vga_buffer[cursor_position * 2] = ' '; 
            vga_buffer[cursor_position * 2 + 1] = vga_attr;
        }
    }
    else if (c == '\t') // Tab
    {
        int spaces = 4 - (cursor_position % 4);
        for (int i = 0; i < spaces; i++) 
        {
            putc(' ');
        }
    }
    else // For all other characters including spaces
    {
        vga_buffer[cursor_position * 2] = c;
        vga_buffer[cursor_position * 2 + 1] = vga_attr;
        cursor_position++;
    }

    move_cursor();
}

void puts(const char *str) 
{
    while (*str)
    {
        putc(*str);
        str++;
    }
}

void printi(int value)
{
    if (value < 0)
    {
        putc('-');
        value = -value;
    }
    char buffer[10];
    int i = 0;

    if (value == 0)
    {
        putc('0');
        return;
    }

    while (value > 0) {
        buffer[i++] = (value % 10) + '0';
        value /= 10;
    }

    for (int j = i - 1; j >= 0; j--)
    {
        putc(buffer[j]);
    }
}

void printh(unsigned int value)
{
    if (value == 0)
    {
        putc('0');
        return;
    }

    char hex_digits[] = "0123456789abcdef";
    char buffer[8];
    int i = 0;

    while (value > 0)
    {
        buffer[i++] = hex_digits[value % 16];
        value /= 16;
    }

    for (int j = (i - 1); j >= 0; j--)
    {
        putc(buffer[j]);
    }
}


void printf(const char* fmt, ...)
{
    const char *ptr = fmt;
    int *arg_ptr = (int*)(&fmt + 1);
    static int cursor_position = 0;

    while (*ptr) 
    {
        if (*ptr == '%')
        {
            ptr++;
            switch (*ptr)
            {
                case 'c':
                    putc((char)*arg_ptr);
                    arg_ptr++;
                    break;
                
                case 's':
                    puts((char*)*arg_ptr);
                    arg_ptr++;
                    break;

                case 'd':
                    printi(*arg_ptr);
                    arg_ptr++;
                    break;

                case 'x':
                    printh(*arg_ptr);
                    arg_ptr++;
                    break;

                case 'p':
                    putc('0');
                    putc('x');
                    printh(*arg_ptr);
                    arg_ptr++;
                    break;

                default:
                    putc('%');
                    putc(*ptr);
                    break;
            }
        }
        else 
        {
            putc(*ptr);
        }
        ptr++;
    }
}


