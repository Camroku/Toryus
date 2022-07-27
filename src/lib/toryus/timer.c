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

#include <stdint.h>
#include <string.h>
#include <toryus/timer.h>
#include <toryus/isr.h>
#include <toryus/terminal.h>
#include <toryus/io.h>
#include <toryus/serial.h>
#include <toryus/toryus.h>

MODULE("timer");

uint32_t tick = 0;

static void timer_callback(registers_t *regs)
{
    (void)regs; // `regs` is not used
    tick++;
}

uint32_t timer_tick()
{
    return tick;
}

void timer_init(uint32_t frequency)
{
    LOG("Initializing");
    // Firstly, register our timer callback.
    register_interrupt_handler(32, &timer_callback);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint32_t divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
    LOG("Initializied");
}