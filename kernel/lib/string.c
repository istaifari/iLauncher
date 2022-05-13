#include "../include/string.h"

typedef struct free_block
{
    size_t size;
    struct free_block *next;
} free_block;

static free_block free_block_list_head = {0, 0};

// static const size_t overhead = sizeof(size_t);

static const size_t align_to = 16;

int strlen(char *str)
{
    int len = 0;
    while ((*str++) != '\0')
        len++;
    return len;
}

/*
void memcpy(void *dest, void *src, size_t n)
{
    char *csrc = (char *)src;
    char *cdest = (char *)dest;
    for (int i = 0; i < n; i++)
    {
        cdest[i] = csrc[i];
    }
}

void *memset(void *s, int c, unsigned int len)
{
    unsigned char *p = s;
    while (len--)
    {
        *p++ = (unsigned char)c;
    }
    return s;
}
*/

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
        dest32[i] = src32[i];
    }
    for (i = 0; i < num_bytes; i++)
    {
        dest8[i] = src8[i];
    }
    return dest;
}

void *memset(void *dest, int val, size_t n)
{
    uint32_t num_dwords = n / 4;
    uint32_t num_bytes = n % 4;
    uint32_t *dest32 = (uint32_t *)dest;
    uint8_t *dest8 = ((uint8_t *)dest) + num_dwords * 4;
    uint8_t val8 = (uint8_t)val;
    uint32_t val32 = val | (val << 8) | (val << 16) | (val << 24);
    uint32_t i;

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

void *malloc(size_t size)
{
    size = (size + sizeof(free_block) + (align_to - 1)) & ~(align_to - 1);
    free_block *block = free_block_list_head.next;
    free_block **head = &(free_block_list_head.next);
    while (block != 0)
    {
        if (block->size >= size)
        {
            *head = block->next;
            return ((char *)block) + sizeof(free_block);
        }
        head = &(block->next);
        block = block->next;
    }

    block = (free_block *)sbrk(size);
    block->size = size;

    return ((char *)block) + sizeof(free_block);
}

void free(void *ptr)
{
    free_block *block = (free_block *)(((char *)ptr) - sizeof(free_block));
    block->next = free_block_list_head.next;
    free_block_list_head.next = block;
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

void strcpy(char *buf_to, char *buf_from)
{

    char a = 0;
    while ((a = *buf_from++) != '\0')
    {
        *buf_to++ = a;
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