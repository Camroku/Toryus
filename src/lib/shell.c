#include <shell.h>
#include <vga.h>
#include <stdbool.h>
#include <terminal.h>
#include <keyboard.h>
#include <string.h>
#include <timer.h>

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
        if (mode == validmodes[i]) return true;
    }
    return false;
}

void print_uptime_unpretty(void)
{
    int uptime = timer_tick();
    terminal_print_dec(uptime);
    terminal_print("\n");
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
    terminal_print("uptime: ");
    bool printedanything = false;
    if (level == 5 || (years > 0 && level == 6))
    {
        terminal_print_dec(years);
        terminal_print(" years");
        printedanything = true;
    }
    if (level == 4 || (months > 0 && level == 6))
    {
        if (printedanything) terminal_print(", ");
        terminal_print_dec(months);
        terminal_print(" months");
        printedanything = true;
    }
    if (level == 3 || (days > 0 && level == 6))
    {
        if (printedanything) terminal_print(", ");
        terminal_print_dec(days);
        terminal_print(" days");
        printedanything = true;
    }
    if (level == 2 || (hours > 0 && level == 6))
    {
        if (printedanything) terminal_print(", ");
        terminal_print_dec(hours);
        terminal_print(" hours");
        printedanything = true;
    }
    if (level == 1 || (minutes > 0 && level == 6))
    {
        if (printedanything) terminal_print(", ");
        terminal_print_dec(minutes);
        terminal_print(" minutes");
        printedanything = true;
    }
    if (level == 0 || (seconds > 0 && level == 6))
    {
        if (printedanything) terminal_print(", ");
        terminal_print_dec(seconds);
        terminal_print(" seconds");
        printedanything = true;
    }
    terminal_print("\n");
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
    memset(command, 0, len); // i have trust issues
    get_until(command, line, ' ');
    if (strcmp(command, "clear") == 0)
    {
        terminal_clear();
        stat = 0;
    }
    else if (strcmp(command, "echo") == 0)
    {
        terminal_print(line + 5);
        terminal_print("\n");
        stat = 0;
    }
    else if (strcmp(command, "stat") == 0)
    {
        terminal_print("stat: ");
        terminal_print_dec(stat);
        terminal_print("\n");
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
                terminal_print("uptime: prints the uptime of the system\n");
                terminal_print("uptime [-mode/-h]: prints the uptime of the system in a certain mode\n");
                terminal_print("    -h: prints this help message\n\n");
                terminal_print("uptime modes:\n");
                terminal_print("    -A: all\n");
                terminal_print("    -a: seconds\n");
                terminal_print("    -B: minutes\n");
                terminal_print("    -b: hours\n");
                terminal_print("    -C: days\n");
                terminal_print("    -c: months\n");
                terminal_print("    -D: years\n");
                stat = 0;
            }
            else
            {
                terminal_print("uptime: invalid mode\n");
                stat = 1;
            }
        }
        else print_uptime('a');
    }
    else if (strcmp(command, "help") == 0)
    {
        terminal_print("Available commands:\n");
        terminal_print("    clear\n");
        terminal_print("    echo <text>\n");
        terminal_print("    stat\n");
        terminal_print("    uptime\n");
        terminal_print("    help\n");
        stat = 0;
    }
    else
    {
        terminal_print("Unknown command '");
        terminal_print(command);
        terminal_print("'\n");
        stat = 127;
    }
}

void shell_exec(void)
{
    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_print("Toryus Shell\n");
    terminal_print("Type 'help' for a list of commands.\n");
    char theinput[129];
    int issigint;
    while (true)
    {
        memset(theinput, 0, 129);
        terminal_print("$ ");
        issigint = keyboard_input(128, theinput);
        if (issigint == 0)
        {
            shell_line(theinput, 129);
        }
        else
        {
            terminal_print("\n");
            stat = 2;
        }
    }
}