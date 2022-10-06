#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

bool cmp_int(elem_t int1, elem_t int2)
{
  return ((int1.i - int2.i) == 0);
}

bool cmp_str(elem_t str1, elem_t str2)
{
  return (strcmp(str1.str, str2.str) == 0);
}


int extract_int_hash_key(elem_t key)
{
  return key.i;
}

unsigned long string_knr_hash(const char *str)
{
  unsigned long result = 0;
  do
    {
      result = result * 31 + *str;
    }
  while (*++str != '\0');
  return result;
}
