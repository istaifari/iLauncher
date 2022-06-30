#include "../include/string.h"

uint32_t last_alloc = 0;
uint32_t heap_end = 0;
uint32_t heap_begin = 0;
uint32_t pheap_begin = 0;
uint32_t pheap_end = 0;
uint8_t *pheap_desc = 0;
uint32_t memory_used = 0;

typedef struct alloc_t
{
    uint8_t status;
    uint32_t size;
} alloc_t;

int strlen(char *str)
{
    int len = 0;
    while ((*str++) != '\0')
        len++;
    return len;
}

void *negative_to_positive(long num)
{
    if (num < 0)
        return -num;
    else
        return num;
}

void *malloc(size_t size)
{
    if (size <= 0)
        return 0;
    void *mem = (void *)(size);
    return mem;
}

void *memcpy(void *dest, void *src, size_t n)
{
    uint32_t num_dwords = n / 4;
    uint32_t num_bytes = n % 4;
    uint32_t *dest32 = (uint32_t *)dest;
    uint32_t *src32 = (uint32_t *)src;
    uint8_t *dest8 = ((uint8_t *)dest) + num_dwords * 4;
    uint8_t *src8 = ((uint8_t *)src) + num_dwords * 4;
    uint32_t i;
    for (i = 0; i < num_dwords; i++)
    {
        if (dest32[i] != src32[i])
            dest32[i] = src32[i];
    }
    for (i = 0; i < num_bytes; i++)
    {
        if (dest8[i] != src8[i])
            dest8[i] = src8[i];
    }
    return dest;
}

void *memset(void *dest, int val, size_t n)
{
    uint_fast32_t num_dwords = n / 4;
    uint_fast32_t num_bytes = n % 4;
    uint_fast32_t *dest32 = (uint_fast32_t *)dest;
    uint_fast8_t *dest8 = ((uint_fast8_t *)dest) + num_dwords * 4;
    uint_fast8_t val8 = (uint_fast8_t)val;
    uint_fast32_t val32 = val | (val << 8) | (val << 16) | (val << 24);
    uint_fast32_t i;

    for (i = 0; i < num_dwords; i++)
    {
        dest32[i] = val32;
    }
    for (i = 0; i < num_bytes; i++)
    {
        dest8[i] = val8;
    }
    return dest;
}

void *sbrk(int increment)
{
    static char global_mem[20000] = {0};
    static char *p_break = global_mem;

    char *const limit = global_mem + 20000;
    char *const original = p_break;

    if (increment < global_mem - p_break || increment >= limit - p_break)
    {
        return (void *)-1;
    }
    p_break += increment;

    return original;
}

/*void memmove(void *dest, void *src, size_t n)
{
    char *csrc = (char *)src;
    char *cdest = (char *)dest;
    char *temp = char[n];
    for (int i = 0; i < n; i++)
    {
        temp[i] = csrc[i];
    }
    for (int i = 0; i < n; i++)
    {
        cdest[i] = temp[i];
    }

    delete[] temp;
}*/

int strcmp(char *str1, char *str2)
{
    int len = strlen(str1);

    if (len != strlen(str2))
    {

        return 1;
    }

    else
    {

        for (int i = 0; i < len; i++)
        {
            if (str1[i] != str2[i])
            {

                return 1;
            }
        }
    }

    return 0;
}

uint8_t *strdup(uint8_t *str)
{
    uint8_t *new_str;

    if (str == NULL)
        return NULL;

    new_str = (uint8_t *)kmalloc(strlen(str) + 1);

    if (new_str == NULL)
        return NULL;

    strcpy(new_str, str);

    return new_str;
}

void strcpy(char *dest, char *src)
{
    do
    {
        *dest++ = *src++;
    } while (*src != 0);
}

void strncpy(char dest[], const char source[], int size)
{
    int i = 0, sz = size;
    while (1)
    {
        dest[i] = source[i];
        if (dest[i] == '\0')
        {
            break;
        }
        if (i >= size)
        {
            dest[i] = '\0';
            break;
        }
        i++;
    }
}

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

char *strncat(char *destination, const char *source, size_t num)
{
    char *ptr = destination + strlen(destination);
    while (*source != '\0' && num--)
    {
        *ptr++ = *source++;
    }
    *ptr = '\0';
    return destination;
}

int strstr(char *str, char *strSub)
{
    int i = 0, j = 0;
    int nTemp = i;
    int nStrLen = strlen(str);
    int nStrSubLen = strlen(strSub);
    for (i = 0; i < nStrLen - nStrSubLen; i++)
    {
        nTemp = i;
        for (j = 0; j < nStrSubLen; j++)
        {

            if (str[nTemp] == strSub[j])
            {
                if (j == nStrSubLen - 1)
                    return 1;
                nTemp++;
            }
            else
                break;
        }
    }
    return 0;
}