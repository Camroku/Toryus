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
#include <stdio.h>
#include <string.h>
#include <toryus/terminal.h>
#include <toryus/dt.h>
#include <toryus/timer.h>
#include <toryus/keyboard.h>
#include <toryus/serial.h>
#include <toryus/io.h>
#include <toryus/shell.h>
#include <toryus/toryus.h>
#include <toryus/multiboot.h>
#include <toryus/initrd.h>

MODULE("kernel");

/* Toryus Kernel main function */
void kernel_main(uint32_t magic, uint32_t addr)
{
    serial_init();
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        LOG("Multiboot magic number is invalid!");
        for (;;)
            ;
    }
    multiboot_info_t *mbi_ptr = (multiboot_info_t *)addr;

    multiboot_module_t *module_ptr = (multiboot_module_t *)mbi_ptr->mods_addr;
    mb_module_count = mbi_ptr->mods_count;
    for (uint32_t i = 0; i < mb_module_count; i++)
    {
        mb_module_names[i] = (char *)module_ptr[i].cmdline;
        if (strcmp((char *)module_ptr->cmdline, "initrd") == 0)
        {
            initrd_init(module_ptr->mod_start, module_ptr->mod_end);
        }
    }
    LOG("Initializing");
    init_descriptor_tables();
    terminal_initialize();
    timer_init(1000);
    keyboard_init();
    LOG("Initialized");

    // Toryus logo
    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    printf(
        "                ##                \n"
        "              ##  ##     ####");
    terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    printf("###  \n");
    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    printf("            ##      ##  #    ");
    terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    printf("#### \n");
    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    printf("          ##          ###     ");
    terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    printf("### \n");
    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    printf(
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
        "Welcome to Toryus!\n");
    LOG("Starting shell");
    shell_exec();
}
