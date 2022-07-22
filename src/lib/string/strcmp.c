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

#include <stddef.h>

int strcmp(char *str1, char *str2)
{
  int i = 0;
  int failed = 0;
  while(str1[i] != '\0' && str2[i] != '\0')
  {
    if(str1[i] != str2[i])
    {
      failed = 1;
      break;
    }
    i++;
  }
  if((str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0'))
    failed = 1;

  return failed;
}
