#include <shell.h>
#include <vga.h>
#include <stdbool.h>
#include <terminal.h>
#include <keyboard.h>
#include <io.h>
#include <string.h>

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
    else if (strcmp(command, "help") == 0)
    {
        terminal_print("Available commands:\n");
        terminal_print("  clear\n");
        terminal_print("  echo <message>\n");
        terminal_print("  stat\n");
        terminal_print("  help\n");
        stat = 0;
    }
    else
    {
        terminal_print("Unknown command.\n");
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