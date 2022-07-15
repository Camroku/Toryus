#pragma once
#include <stdint.h>
#include <vga.h>
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
