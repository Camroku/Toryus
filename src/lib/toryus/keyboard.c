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

#include <stdbool.h>
#include <toryus/keyboard.h>
#include <toryus/isr.h>
#include <toryus/io.h>
#include <toryus/terminal.h>
#include <toryus/serial.h>
#include <toryus/toryus.h>

MODULE("keyboard");

char lastchar = 0;
bool gotchar = false;
unsigned char scancode;
bool shift = false;
bool caps = false;
bool ctrl = false;
unsigned char shifts[12] = {
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')'
};

unsigned char keyboard_us[128] =
    {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	'9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, // Control
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0, // Left shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',	'm', ',', '.', '/',   0, // Right shift
    '*', 0, // Alt
    ' ', 0, // Caps lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1 - F10
    0,	// 69 - Num lock
    0,	// Scroll Lock
    0,	// Home key
    0,	// Up Arrow
    0,	// Page Up
        '-',
    0,	// Left Arrow
        0,
    0,	// Right Arrow
        '+',
    0,	// 79 - End key
    0,	// Down ArroW
    0,	// Page Down
    0,	// Insert Key
    0,	// Delete Key
    0,   0,   0,
    0, 0,	// F11 - F12
    0,	// All other keys are undefined
};

char scancodetoascii(unsigned char scancode)
{
    if (scancode < 0x80)
    {
        if (ctrl)
        {
            return keyboard_us[scancode] - 'a' + 1;
        }
        else if (caps)
        {
            if (keyboard_us[scancode] <= 'z' && keyboard_us[scancode] >= 'a')
                return keyboard_us[scancode] + 'A' - 'a';
            else if (keyboard_us[scancode] <= 'Z' && keyboard_us[scancode] >= 'A')
                return keyboard_us[scancode] - 'A' + 'a';
            else
                return keyboard_us[scancode];
        }
        else if (shift)
        {
            if (keyboard_us[scancode] <= 'z' && keyboard_us[scancode] >= 'a')
                return keyboard_us[scancode] + 'A' - 'a';
            else if (keyboard_us[scancode] <= 'Z' && keyboard_us[scancode] >= 'A')
                return keyboard_us[scancode] - 'A' + 'a';
            else if (keyboard_us[scancode] <= '9' && keyboard_us[scancode] >= '0')
                return shifts[scancode - 2];
            else if (keyboard_us[scancode] == ',')
                return '<';
            else if (keyboard_us[scancode] == '.')
                return '>';
            else if (keyboard_us[scancode] == '/')
                return '?';
            else if (keyboard_us[scancode] == '`')
                return '~';
            else if (keyboard_us[scancode] == '-')
                return '_';
            else if (keyboard_us[scancode] == '=')
                return '+';
            else if (keyboard_us[scancode] == '[')
                return '{';
            else if (keyboard_us[scancode] == ']')
                return '}';
            else if (keyboard_us[scancode] == '\\')
                return '|';
            else if (keyboard_us[scancode] == ';')
                return ':';
            else if (keyboard_us[scancode] == '\'')
                return '"';
            else
                return keyboard_us[scancode];
        }
        else
        {
            return keyboard_us[scancode];
        }
    }
    else
    {
        return 0;
    }
}

static void keyboard_handler(registers_t *regs)
{
    (void)regs; // `regs` is not used
    scancode = inb(0x60);
    if (scancode > 0x80)
    {
        if ((scancode - 0x80) == 42 || (scancode - 0x80) == 54)
        {
            shift = false;
        }
        else if ((scancode - 0x80) == 29)
        {
            ctrl = false;
        }
    }
    else
    {
        if (scancode == 42 || scancode == 54)
        {
            shift = true;
        }
        else if (scancode == 58)
        {
            caps = !caps;
        }
        else if (scancode == 29)
        {
            ctrl = true;
        }
        lastchar = scancodetoascii(scancode);
        if (lastchar != 0)
            gotchar = true;
    }
}

void keyboard_init(void)
{
    LOG("Initializing");
    register_interrupt_handler(33, &keyboard_handler);
    LOG("Initialized");
}

char keyboard_getchar()
{
    lastchar = 0; // set last character to null character
    gotchar = false;
    while (gotchar == false)
        io_wait();   // wait until key press
    gotchar = false; // set gotchar to false
    return lastchar; // return the character
}

int keyboard_input(unsigned int input_length, char *theinput)
{
    int last_position = input_length - 1;
    int position = 0;
    char character = 0;
    while (character != '\n')
    { // until the user presses enter
        character = keyboard_getchar();
        if (character == 3)
        { // if the user presses ctrl-c
            terminal_putchar(character);
            return 1;
        }
        if (character == '\b')
        {
            if (position != 0)
            {
                position--;
                theinput[position] = 0;
                terminal_putchar(character);
            }
        }
        else if (position != last_position)
        {
            if (character != '\n')
                theinput[position] = character;
            position++;
            terminal_putchar(character);
        }
        if (position == last_position && character == '\n')
            terminal_putchar(character);
    }
    return 0;
}