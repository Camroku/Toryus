#include <terminal.h>
#include <dt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <timer.h>
#include <keyboard.h>
#include <serial.h>

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
  terminal_print("Version 1, Build 0.0.1\n");
  serial_log("pit", "Initializing");
  timer_init(100);
  serial_log("kbd", "Initializing");
  keyboard_init();
  serial_log("knl", "Initialized");
  terminal_print("> ");
  char theinput[25];
  keyboard_input(24, theinput);
  terminal_print("You said this: ");
  terminal_print(theinput);
  terminal_print("\nTest interrupt:\n");
  asm volatile ("int $0x13");
  for (;;){
    asm volatile("hlt");
  }
}
/*

*/