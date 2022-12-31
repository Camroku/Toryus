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
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <toryus/toryus.h>
#include <toryus/terminal.h>

void panic(const char *message, const char *file, uint32_t line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    printf("PANIC(%s) at %s:%d\n", message, file, line);
    // Halt by going into an infinite loop.
    for (;;)
        ;
}

void panic_assert(const char *file, uint32_t line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    printf("ASSERTION-FAILED(%s) at %s:%d\n", desc, file, line);
    for (;;)
        ;
}

void log(const char *module, const char *message)
{
    write_serial('[');
    write_str_serial(module);
    write_str_serial("]: ");
    write_str_serial(message);
    write_serial('\n');
}

int logf(const char *module, const char *restrict format, ...)
{
    write_serial('[');
    write_str_serial(module);
    write_str_serial("]: ");
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0')
    {
        size_t maxrem = INT32_MAX - written;

        if (format[0] != '%' || format[1] == '%')
        {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;
            if (maxrem < amount)
            {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!write_str_serial_strict(format, amount))
                return -1;
            format += amount;
            written += amount;
            continue;
        }

        const char *format_begun_at = format++;

        if (*format == 'c')
        {
            format++;
            char c = (char)va_arg(parameters, int);
            if (!maxrem)
            {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!write_serial(c))
                return -1;
            written++;
        }
        else if (*format == 's')
        {
            format++;
            const char *str = va_arg(parameters, const char *);
            size_t len = strlen(str);
            if (maxrem < len)
            {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!write_str_serial(str))
                return -1;
            written += len;
        }
        else if (*format == 'x')
        {
            format++;
            int num = va_arg(parameters, int);
            size_t len = (size_t)write_hex_serial(num);
            written += len;
        }
        else if (*format == 'd')
        {
            format++;
            int num = va_arg(parameters, int);
            size_t len = (size_t)write_int_serial(num);
            written += len;
        }
        else
        {
            format = format_begun_at;
            size_t len = strlen(format);
            if (maxrem < len)
            {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!write_str_serial(format))
                return -1;
            written += len;
            format += len;
        }
    }

    va_end(parameters);
    write_serial('\n');
    return written;
}
