#include <hal.h>
#include <shell.h>
#include <chprintf.h>
#include "lib.h"


/**
 * Converts string to integer or returns 0.
 */
int my_atoi(const char* string) {
  int pos = 0, result = 0, sign = 0;
  char s = '\0';
  if (string[pos] == '-')
    sign = 1;
  while ((s = string[pos++]) != 0x00) {
    if (s < '0' || s > '9')
      return 0;
    result = result * 10 + (s - '0');
  }
  return (sign == 0) ? result : -result;
}

/**
 * Returns the quantity of symbols in a string.
 */
int my_strlen(const char* string) {
  int pos = 0;
  while (string[pos] != '\0') { pos ++; }
  return pos;
}


/**
 * Compares two strings, see original strcmp
 */
int my_strcmp(const char* a, const char* b) {
  int i = 0;
  while (a[i] == b[i] && a[i] != '\0') { i++; }
  if (a[i] == b[i] && a[i] == '\0')
    return 0;
  return (a[i] > b[i]) ? 1 : -1;
}


/**
 * Copies data in memory, see original memcpy
 */
void* my_memcpy(void* dst, void* src, size_t count) {
  unsigned char* dstb = (unsigned char*) dst;
  unsigned char* srcb = (unsigned char*) src;
  while (count--) {
    *dstb++ = *srcb++;
  }
  return dst;
}


/**
 * Converts HEX string into the appropriate byte array.
 */
int hex_to_array(const char* hex, unsigned char* bytes) {
  int len = my_strlen(hex), count = len / 2;
  if (len % 2 != 0)
    return -1;
  if (len >= 16)
    return -2;
  for (int i=0, j=0; i<len; i+=2, j++) {
    char a = hex[i], b = hex[i+1];
    bytes[j] = (unsigned char)                           \
      (((a >= 'A') ? (a - 'A' + 10) : (a - '0')) << 4) + \
      (((b >= 'A') ? (b - 'A' + 10) : (b - '0')));
  }
  return count;
}


/**
 * Converts integer into string using base.
 */
char* itoa(int val, int base){
    static char buf[32] = {0};
    int i = 30;
    for(; val && i ; --i, val /= base)
        buf[i] = "0123456789abcdef"[val % base];
    return &buf[i+1];

}
