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

// Get the content until a seperator
void get_until(char *dest, char *src, char sep)
{
    int i = 0;
    while (src[i] != sep && src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

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
  char theinput[129];
  char command[129];
  int stat = 0;
  while (true)
  {
    memset(theinput, 0, 129);
    memset(command, 0, 129);
    terminal_print("$ ");
    stat = keyboard_input(128, theinput);
    if (stat == 1)
    {
      terminal_print("\n");
      continue;
    }
    get_until(command, theinput, ' ');
    if (strcmp(command, "clear") == 0)
    {
      terminal_clear();
    } else if (strcmp(command, "echo") == 0)
    {
      terminal_print(theinput + 5);
      terminal_print("\n");
    } else if (strcmp(command, "help") == 0)
    {
      terminal_print("Available commands:\n");
      terminal_print("  clear\n");
      terminal_print("  echo <message>\n");
      terminal_print("  help\n");
    } else
    {
      terminal_print("Unknown command.\n");
    }
  }
}
/*

*/