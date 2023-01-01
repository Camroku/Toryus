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

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <toryus/serial.h>
#include <toryus/io.h>

#define COM1 0x3f8

int serial_init()
{
    outb(COM1 + 1, 0x00); // Disable all interrupts
    outb(COM1 + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(COM1 + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1 + 1, 0x00); //                  (hi byte)
    outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
    outb(COM1 + 4, 0x1E); // Set in loopback mode, test the serial chip
    outb(COM1 + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (inb(COM1 + 0) != 0xAE)
    {
        return 1;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(COM1 + 4, 0x0F);
    return 0;
}

int serial_received()
{
    return inb(COM1 + 5) & 1;
}

char read_serial()
{
    while (serial_received() == 0)
        ;

    return inb(COM1);
}

int is_transmit_empty()
{
    return inb(COM1 + 5) & 0x20;
}

bool write_serial(char data)
{
    while (is_transmit_empty() == 0);

    outb(COM1, data);
    return 1; // was too lazy to change code in logf
}

bool write_str_serial(const char *data)
{
    for (size_t i = 0; i < strlen(data); i++)
    {
        if (data[i] == EOF)
        {
            return false;
            break;
        }
        write_serial(data[i]);
    }
    return true;
}

bool write_str_serial_strict(const char *data, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (data[i] == EOF)
        {
            return false;
            break;
        }
        write_serial(data[i]);
    }
    return true;
}

int write_int_serial(uint32_t n)
{
    if (n == 0)
    {
        write_serial('0');
        return 1;
    }

    int32_t acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc % 10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while (i >= 0)
    {
        c2[i--] = c[j++];
    }
    write_str_serial(c2);
    return strlen(c2);
}

int write_hex_serial(uint32_t n)
{
    int32_t tmp;
    int len = 0;
    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            write_serial(tmp - 0xA + 'a');
        }
        else
        {
            noZeroes = 0;
            write_serial(tmp + '0');
        }
        len++;
    }

    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        write_serial(tmp - 0xA + 'a');
    }
    else
    {
        write_serial(tmp + '0');
    }
    len++;
    return i;
}
