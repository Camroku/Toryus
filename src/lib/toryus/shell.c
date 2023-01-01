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

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <toryus/shell.h>
#include <toryus/vga.h>
#include <toryus/terminal.h>
#include <toryus/keyboard.h>
#include <toryus/timer.h>
#include <toryus/toryus.h>
#include <toryus/initrd.h>

MODULE("shell");

int stat = 0;

void get_until(char *dest, char *src, char sep)
{
    int i = 0;
    while (src[i] != sep && src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

bool uptime_valid(char mode)
{
    char validmodes[] = "AaBbCcD";
    /*
    A = all
    a = seconds
    B = minutes
    b = hours
    C = days
    c = months
    D = years
    */
    for (int i = 0; i < 7; i++)
    {
        if (mode == validmodes[i])
            return true;
    }
    return false;
}

void print_uptime_unpretty(void)
{
    int uptime = timer_tick();
    printf("%d\n", uptime);
}

void print_uptime_pretty(int level)
{
    int seconds = 0, minutes = 0, hours = 0, days = 0, months = 0, years = 0;
    int uptime = timer_tick();
    seconds = uptime / 1000;
    if (seconds > 60)
    {
        minutes = seconds / 60;
        seconds = seconds % 60;
    }
    if (minutes > 60)
    {
        hours = minutes / 60;
        minutes = minutes % 60;
    }
    if (hours > 24)
    {
        days = hours / 24;
        hours = hours % 24;
    }
    if (days > 30)
    {
        months = days / 30;
        days = days % 30;
    }
    if (months > 12)
    {
        years = months / 12;
        months = months % 12;
    }
    printf("uptime: ");
    bool printedanything = false;
    if (level == 5 || (years > 0 && level == 6))
    {
        printf("%d years", years);
        printedanything = true;
    }
    if (level == 4 || (months > 0 && level == 6))
    {
        if (printedanything)
            printf(", ");
        printf("%d months", months);
        printedanything = true;
    }
    if (level == 3 || (days > 0 && level == 6))
    {
        if (printedanything)
            printf(", ");
        printf("%d days", days);
        printedanything = true;
    }
    if (level == 2 || (hours > 0 && level == 6))
    {
        if (printedanything)
            printf(", ");
        printf("%d hours", hours);
        printedanything = true;
    }
    if (level == 1 || (minutes > 0 && level == 6))
    {
        if (printedanything)
            printf(", ");
        printf("%d minutes", minutes);
        printedanything = true;
    }
    if (level == 0 || (seconds > 0 && level == 6))
    {
        if (printedanything)
            printf(", ");
        printf("%d seconds", seconds);
        printedanything = true;
    }
    printf("\n");
    stat = 0;
}

void print_uptime(char mode)
{
    switch (mode)
    {
    case 'A':
        print_uptime_pretty(6);
        break;
    case 'D':
        print_uptime_pretty(5);
        break;
    case 'c':
        print_uptime_pretty(4);
        break;
    case 'C':
        print_uptime_pretty(3);
        break;
    case 'b':
        print_uptime_pretty(2);
        break;
    case 'B':
        print_uptime_pretty(1);
        break;
    case 'a':
        print_uptime_pretty(0);
        break;
    default:
        print_uptime_unpretty();
        break;
    }
}

void shell_line(char *line, int len)
{
    char command[len];
    memset(command, 0, len);
    get_until(command, line, ' ');
    if (strcmp(command, "clear") == 0)
    {
        terminal_clear();
        stat = 0;
    }
    else if (strcmp(command, "echo") == 0)
    {
        printf("%s\n", line + 5);
        stat = 0;
    }
    else if (strcmp(command, "stat") == 0)
    {
        printf("stat: %d\n", stat);
        stat = 0;
    }
    else if (strcmp(command, "uptime") == 0)
    {
        if (line[7] == '-')
        {
            if (uptime_valid(line[8]))
            {
                print_uptime(line[8]);
            }
            else if (line[8] == 'h')
            {
                printf(
                    "uptime: prints the uptime of the system\n"
                    "uptime [-mode/-h]: prints the uptime of the system in a certain mode\n"
                    "    -h: prints this help message\n\n"
                    "uptime modes:\n"
                    "    -A: all\n"
                    "    -a: seconds\n"
                    "    -B: minutes\n"
                    "    -b: hours\n"
                    "    -C: days\n"
                    "    -c: months\n"
                    "    -D: years\n");
                stat = 0;
            }
            else
            {
                printf("uptime: invalid mode\n");
                stat = 1;
            }
        }
        else
            print_uptime_unpretty();
    }
    else if (strcmp(command, "initrd") == 0)
    {
        printf("initrd: %d bytes\n%s\n", initrd_size(), initrd_read());
        stat = 0;
    }
    else if (strcmp(command, "modules") == 0)
    {
        printf("Module count: %d\n", mb_module_count);
        if (strcmp(line + 8, "list") == 0)
        {
            for (uint32_t i = 0; i < mb_module_count; i++)
            {
                printf("Module %d: %s\n", i, mb_module_names[i]);
            }
        }
        stat = 0;
    }
    else if (strcmp(command, "help") == 0)
    {
        printf("Available commands:\n"
               "    clear\t\tClear screen\n"
               "    echo [<text>]\tPrint text\n"
               "    stat\t\tPrints the current status\n"
               "    uptime\t\tPrints the uptime of the system\n"
               "    initrd\t\tPrints the initrd\n"
               "    modules [list]\tPrints the module count\n"
               "    help\t\tPrints this help message\n");
        stat = 0;
    }
    else
    {
        printf("Unknown command '%s'\n", command);
        stat = 127;
    }
}

void shell_exec(void)
{
    printf(
        "Toryus Shell\n"
        "Type 'help' for a list of commands.\n");
    char theinput[129];
    int issigint;
    while (true)
    {
        memset(theinput, 0, 129);
        printf("\033[0;32m$ \033[1;37m");
        issigint = keyboard_input(128, theinput);
        if (issigint == 0)
        {
            shell_line(theinput, 129);
        }
        else
        {
            printf("\n");
            stat = 2;
        }
    }
}
