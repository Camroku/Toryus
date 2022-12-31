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

#include <stdbool.h>

int serial_init();
char read_serial();
bool write_serial(char a);
bool write_str_serial(const char *data);
bool write_str_serial_strict(const char *data, size_t len);
int write_int_serial(uint32_t n);
int write_hex_serial(uint32_t n);
