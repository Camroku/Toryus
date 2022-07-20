#pragma once
#include <stdint.h>
#include <stddef.h>

size_t strlen(const char *str);
int strcmp(char *str1, char *str2);
void strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
void *memcpy(void *dest, const void *src, size_t len);
void *memset(void *dest, int val, size_t len);
