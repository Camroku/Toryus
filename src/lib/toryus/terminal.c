/*
    This file is part of Toryus.

    Toryus is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version
    3 of the License, or (at your option) any later version.

    Toryus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with Toryus. If not, see
    <https://www.gnu.org/licenses/>.
*/

#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <toryus/io.h>
#include <toryus/vga.h>
#include <toryus/terminal.h>
#include <toryus/serial.h>
#include <toryus/toryus.h>

MODULE("terminal");

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
int cursor_y = 0;
int cursor_x = 0;
int line_length[VGA_HEIGHT];
uint8_t backColour = 0;
uint8_t foreColour = 15;

size_t row;
size_t column;
uint8_t color;
uint16_t *buffer;

enum vga_color colorlist[] = {
    VGA_COLOR_BLACK,
    VGA_COLOR_RED,
    VGA_COLOR_GREEN,
    VGA_COLOR_BROWN,
    VGA_COLOR_BLUE,
    VGA_COLOR_MAGENTA,
    VGA_COLOR_CYAN,
    VGA_COLOR_LIGHT_GREY};

enum vga_color boldcolorlist[] = {
    VGA_COLOR_DARK_GREY,
    VGA_COLOR_LIGHT_RED,
    VGA_COLOR_LIGHT_GREEN,
    VGA_COLOR_LIGHT_BROWN,
    VGA_COLOR_LIGHT_BLUE,
    VGA_COLOR_LIGHT_MAGENTA,
    VGA_COLOR_LIGHT_CYAN,
    VGA_COLOR_WHITE};

void terminal_initialize(void)
{
    LOG("Initializing");
    row = 0;
    column = 0;
    color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    buffer = (uint16_t *)0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            buffer[index] = vga_entry(' ', color);
        }
    }
    LOG("Initialized");
}

void terminal_setcolor(enum vga_color fore, enum vga_color back)
{
    color = fore | back << 4;
    backColour = back;
    foreColour = fore;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    buffer[index] = vga_entry(c, color);
}

// Updates the hardware cursor.
void terminal_move_cursor()
{
    // The screen is 80 characters wide...
    uint16_t cursorLocation = cursor_y * 80 + cursor_x;
    outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
    outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
void terminal_scroll()
{

    // Get a space character with the default colour attributes.
    uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

    // Row 25 is the end, this means we need to scroll up
    if (cursor_y >= 25)
    {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0 * 80; i < 24 * 80; i++)
        {
            buffer[i] = buffer[i + 80];
        }

        // The last line should now be blank. Do this by writing
        // 80 spaces to it.
        for (i = 24 * 80; i < 25 * 80; i++)
        {
            buffer[i] = blank;
        }
        // The cursor should now be on the last line.
        cursor_y = 24;
    }
}

// Writes a single character out to the screen.
void terminal_putchar(char c)
{
    // The background colour is black (0), the foreground is white (15).

    // The attribute byte is made up of two nibbles - the lower being the
    // foreground colour, and the upper the background colour.
    uint8_t attributeByte = (backColour << 4) | (foreColour & 0x0F);
    // The attribute byte is the top 8 bits of the word we have to send to the
    // VGA board.
    uint16_t attribute = attributeByte << 8;
    uint16_t *location;

    // Handle a backspace
    if (c == 0x08)
    {
        terminal_handle_backspace();
    }

    // Handle a tab by increasing the cursor's X, but only to a point
    // where it is divisible by 8.
    else if (c == 0x09)
    {
        cursor_x = (cursor_x + 8) & ~(8 - 1);
    }

    // Handle carriage return
    else if (c == '\r')
    {
        cursor_x = 0;
    }

    // Handle newline by moving cursor back to left and increasing the row
    else if (c == '\n')
    {
        line_length[cursor_y] = cursor_x;
        cursor_x = 0;
        cursor_y++;
    }
    else if (c >= 1 && c <= 31)
    {
        terminal_putchar('^');
        terminal_putchar(c + 'A' - 1);
    }
    // Handle any other printable character.
    else if (c >= ' ')
    {
        location = buffer + (cursor_y * 80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    // Check if we need to insert a new line because we have reached the end
    // of the screen.
    if (cursor_x >= 80)
    {
        line_length[cursor_y] = cursor_x;
        cursor_x = 0;
        cursor_y++;
    }

    // Scroll the screen if needed.
    terminal_scroll();
    // Move the hardware cursor.
    terminal_move_cursor();
}

bool terminal_write(const char *text, size_t size)
{
    char *data = (char *)text;
    for (size_t i = 0; i < size; i++)
    {
        if (data[i] == EOF)
        {
            return false;
        }
        if (data[i] == '\033' && data[i + 1] == '[')
        {
            i += 2;
            bool is_bold = (data[i] == '1');
            if (data[i] != '0' && data[i] != '1')
            {
                i -= 2;
            }
            i++;
            do
            {
                i++;
                bool isforeground = (data[i] == '3');
                i++;
                int colour = data[i] - '0';
                i++;
                if (isforeground)
                {
                    if (is_bold)
                    {
                        foreColour = boldcolorlist[colour];
                    }
                    else
                    {
                        foreColour = colorlist[colour];
                    }
                }
                else
                {
                    if (is_bold)
                    {
                        backColour = boldcolorlist[colour];
                    }
                    else
                    {
                        backColour = colorlist[colour];
                    }
                }
                color = foreColour | backColour << 4;
                if (data[i] == 'm')
                {
                    break;
                }
            } while (data[i] == ';');
        }

        else
        {
            terminal_putchar(data[i]);
        }
    }
    terminal_scroll();
    terminal_move_cursor();
    return true;
}

void terminal_print(const char *data) { terminal_write(data, strlen(data)); }

// Clears the screen, by copying lots of spaces to the framebuffer.
void terminal_clear()
{
    // Make an attribute byte for the default colours
    uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

    int i;
    for (i = 0; i < 80 * 25; i++)
    {
        buffer[i] = blank;
    }

    // Move the hardware cursor back to the start.
    cursor_x = 0;
    cursor_y = 0;
    terminal_move_cursor();
}

int terminal_print_dec(uint32_t n)
{
    if (n == 0)
    {
        terminal_print("0");
        return 1;
    }

    int32_t acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc % 10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while (i >= 0)
    {
        c2[i--] = c[j++];
    }
    terminal_print(c2);
    return strlen(c2);
}

void terminal_handle_backspace()
{
    uint8_t attributeByte = (backColour << 4) | (foreColour & 0x0F);
    uint16_t attribute = attributeByte << 8;
    uint16_t *location;

    if (cursor_x > 0)
    {
        cursor_x--;
    }
    else if (cursor_y > 0)
    {
        cursor_y--;
        cursor_x = line_length[cursor_y];
    }
    location = buffer + (cursor_y * 80 + cursor_x);
    *location = ' ' | attribute;
}

int terminal_print_hex(uint32_t n)
{
    int32_t tmp;
    terminal_print("0x");
    int len = 2;
    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            terminal_putchar(tmp - 0xA + 'a');
        }
        else
        {
            noZeroes = 0;
            terminal_putchar(tmp + '0');
        }
        len++;
    }

    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        terminal_putchar(tmp - 0xA + 'a');
    }
    else
    {
        terminal_putchar(tmp + '0');
    }
    len++;
    return i;
}