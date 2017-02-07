#ifndef _LIB_H_
#define _LIB_H_

int my_atoi(const char* string);
int my_strlen(const char* string);
int my_strcmp(const char* a, const char* b);
void* my_memcpy(void* dst, void* src, size_t count);
int hex_to_array(const char* hex, unsigned char* bytes);
char* itoa(int val, int base);

#endif /* _LIB_H_ */
