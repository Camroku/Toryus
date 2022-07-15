#pragma once

int serial_init();
char read_serial();
void write_serial(char a);
void write_str_serial(const char *a);
void serial_log(const char *from, const char *state);
