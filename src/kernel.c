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
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <toryus/terminal.h>
#include <toryus/dt.h>
#include <toryus/timer.h>
#include <toryus/keyboard.h>
#include <toryus/serial.h>
#include <toryus/io.h>
#include <toryus/shell.h>
#include <toryus/toryus.h>

MODULE("kernel");

/* Toryus Kernel main function */
void kernel_main(void)
{
  serial_init();
  LOG("Initializing");
  init_descriptor_tables();
  terminal_initialize();
  timer_init(1000);
  keyboard_init();
  LOG("Initialized");

  // Toryus logo
  terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  terminal_print(
    "                ##                \n"
    "              ##  ##     ####");
  terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
  terminal_print("###  \n");
  terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  terminal_print("            ##      ##  #    ");
  terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
  terminal_print("#### \n");
  terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  terminal_print("          ##          ###     ");
  terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
  terminal_print("### \n");
  terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  terminal_print(
    "        ##              ##     #  \n"
    "      ##                  #####   \n"
    "    ##                      ##    \n"
    "  ##                          ##  \n"
    "##              ##              ##\n"
    "  ##              ##          ##  \n"
    "    ##              ##      ##    \n"
    "      ##              ##  ##      \n"
    "        ##              ##        \n"
    "          ##          ##          \n"
    "            ##      ##            \n"
    "              ##  ##              \n"
    "                ##                \n"
    "Welcome to Toryus!\n"
  );
  LOG("Starting shell");
  shell_exec();
}
/*

*/