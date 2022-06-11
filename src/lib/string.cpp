#include <stddef.h>
#include <string.hpp>

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
    *dest = *dest++;
  }

  do
  {
    *dest++ = *src++;
  }
  while (*src != 0);
  return dest;
}
