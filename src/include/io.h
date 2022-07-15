#pragma once
#include <stdint.h>
#include <stddef.h>

void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
void outl(uint16_t port, uint32_t val);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);
void io_wait(void);
void *memcpy(void *dest, const void *src, size_t len);
void *memset(void *dest, int val, size_t len);
