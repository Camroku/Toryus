#include <stddef.h>
#include <string.h>

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

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

void strcpy(char *dest, const char *src)
{
  do
  {
    *dest++ = *src++;
  }
  while (*src != 0);
}

char *strcat(char *dest, const char *src)
{
  while (*dest != 0)
  {
    *dest = *dest + 1;
  }

  do
  {
    *dest++ = *src++;
  }
  while (*src != 0);
  return dest;
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
