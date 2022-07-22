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

#include <stdint.h>
#include <toryus/toryus.h>
#include <toryus/terminal.h>

void panic(const char *message, const char *file, uint32_t line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    terminal_print("PANIC(");
    terminal_print(message);
    terminal_print(") at ");
    terminal_print(file);
    terminal_print(":");
    terminal_print_dec(line);
    terminal_print("\n");
    // Halt by going into an infinite loop.
    for(;;);
}

void panic_assert(const char *file, uint32_t line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    terminal_print("ASSERTION-FAILED(");
    terminal_print(desc);
    terminal_print(") at ");
    terminal_print(file);
    terminal_print(":");
    terminal_print_dec(line);
    terminal_print("\n");
    // Halt by going into an infinite loop.
    for(;;);
}
