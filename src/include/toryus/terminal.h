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

#pragma once
#include <stdint.h>
#include <toryus/vga.h>
#include <stddef.h>

void terminal_initialize(void);
void terminal_setcolor(enum vga_color fore, enum vga_color back);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_move_cursor();
void terminal_scroll();
void terminal_putchar(char c);
void terminal_write(const char *data, size_t size);
void terminal_print(const char *data);
void terminal_clear();
void terminal_print_dec(uint32_t n);
void terminal_handle_backspace();
void terminal_print_hex(uint32_t n);
