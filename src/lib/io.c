#include <stdint.h>
#include <io.h>
#include <stddef.h>

void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

void outw(uint16_t port, uint16_t val)
{
    asm volatile("outw %w0, %w1" : : "a" (val), "Nd" (port));
}

void outl(uint16_t port, uint32_t val)
{
    asm volatile("outl %0, %w1" : : "a" (val), "Nd" (port));
}

uint8_t inb(uint16_t port)
{
    uint8_t data;
    asm volatile("inb %w1, %b0" : "=a" (data) : "Nd" (port));
    return data;
}

uint16_t inw(uint16_t port)
{
    uint16_t data;
    asm volatile("inw %w1, %w0" : "=a" (data) : "Nd" (port));
    return data;
}

uint32_t inl(uint16_t port)
{
    uint32_t data;
    asm volatile("inl %w1, %0" : "=a" (data) : "Nd" (port));
    return data;
}

void io_wait(void)
{
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}

void *memcpy (void *dest, const void *src, size_t len)
{
  char *d = dest;
  const char *s = src;
  while (len--)
    *d++ = *s++;
  return dest;
}

void *memset (void *dest, int val, size_t len)
{
  unsigned char *ptr = dest;
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}
