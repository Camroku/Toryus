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

#pragma once
#include <stdint.h>
#include <toryus/serial.h>

void panic(const char *message, const char *file, uint32_t line);
void panic_assert(const char *file, uint32_t line, const char *desc);
void log(const char *module, const char *message);
int logf(const char *module, const char* restrict format, ...);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))
#define MODULE(name) __attribute__((unused)) static char *__MODULE_NAME = name;
#define LOG(message) log(__MODULE_NAME, message)
#define LOGF(message, ...) logf(__MODULE_NAME, message, ##__VA_ARGS__)

uint32_t mb_module_count;
char **mb_module_names;
