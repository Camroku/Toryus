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

#include <toryus/terminal.h>
#include <toryus/isr.h>
#include <toryus/io.h>

isr_t interrupt_handlers[256];

void isr_handler(registers_t regs)
{
  terminal_print("recieved interrupt: ");
  terminal_print_hex(regs.int_no);
  if (regs.err_code != 0) {
    terminal_print(", errno: ");
    terminal_print_hex(regs.err_code);
  }
  terminal_print("\n");
  asm volatile ("cli");
}

void irq_handler(registers_t regs)
{
  if (regs.int_no >= 40)
  {
    outb(0xA0, 0x20);
  }
  outb(0x20, 0x20);
  if (interrupt_handlers[regs.int_no] != 0)
  {
    isr_t handler = interrupt_handlers[regs.int_no];
    handler(&regs);
  }
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
  interrupt_handlers[n] = handler;
} 