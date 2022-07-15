#include <terminal.h>
#include <dt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <timer.h>
#include <keyboard.h>
#include <serial.h>
#include <string.h>
#include <io.h>
#include <shell.h>

/* Toryus Kernel main function */
void kernel_main(void)
{
  serial_init();
  serial_log("knl", "Initializing");
  serial_log("dts", "Initializing");
  init_descriptor_tables();
  asm volatile ("sti");
  serial_log("tty", "Initializing");
  terminal_initialize();

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
  );
  // Print an ASCII art square rotated.

  terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  terminal_print("Welcome to Toryus!\n");
  serial_log("pit", "Initializing");
  timer_init(100);
  serial_log("kbd", "Initializing");
  keyboard_init();
  serial_log("knl", "Initialized");
  shell_exec();
}
/*

*/