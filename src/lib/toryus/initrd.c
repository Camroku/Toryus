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

char initrd_content[1024 * 1024];
uint32_t _initrd_size;

void initrd_init(uint32_t start, uint32_t end)
{
    _initrd_size = end - start;
    memcpy(initrd_content, (char *)start, _initrd_size);
    initrd_content[_initrd_size] = '\0';
}

char *initrd_read()
{
    return initrd_content;
}

uint32_t initrd_size()
{
    return _initrd_size;
}
