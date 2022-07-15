#include <terminal.hpp>
#include <dt.hpp>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <timer.hpp>
#include <keyboard.hpp>
#include <serial.hpp>

/* Toryus Kernel main function */
extern "C" void kernel_main(void)
{
  serial::init();
  serial::log("knl", "Initializing");
  serial::log("dts", "Initializing");
  init_descriptor_tables();
  asm volatile ("sti");
  serial::log("tty", "Initializing");
  terminal::initialize();

  // Toryus logo
  terminal::setcolor(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
  terminal::print(
    "                ##                \n"
    "              ##  ##     #######  \n"
    "            ##      ##  #    #  # \n"
    "          ##          ###     ### \n"
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

  terminal::setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  terminal::print("Welcome to Toryus!\n");
  terminal::print("Version 1, Build 0.0.1\n");
  serial::log("pit", "Initializing");
  timer::init(100);
  serial::log("kbd", "Initializing");
  keyboard::init();
  serial::log("knl", "Initialized");
  terminal::print("> ");
  char theinput[25];
  keyboard::input(24, theinput);
  terminal::print("You said this: ");
  terminal::print(theinput);
  terminal::print("\nTest interrupt:\n");
  asm volatile ("int $0x13");
  for (;;){
    asm volatile("hlt");
  }
}
/*

*/