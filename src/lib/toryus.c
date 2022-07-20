#include <toryus.h>
#include <stdint.h>
#include <terminal.h>

extern void panic(const char *message, const char *file, uint32_t line)
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

extern void panic_assert(const char *file, uint32_t line, const char *desc)
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
