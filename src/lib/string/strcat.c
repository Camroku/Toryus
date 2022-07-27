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

char *strcat(char *dest, const char *src)
{
    while (*dest != 0)
    {
        *dest = *dest + 1;
    }

    do
    {
        *dest++ = *src++;
    } while (*src != 0);
    return dest;
}