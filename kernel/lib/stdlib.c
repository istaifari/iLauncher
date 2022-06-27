#include "../include/stdlib.h"

static long next = 1;

long rand(void)
{
    next = next * 1103515245 + 12345;
    return (long)(next / 65536) % 32768;
}

long srand(long seed)
{
    next = seed;
}

long atoi(char *string)
{
    char a = 0;
    long res = 0;
    while ((a = *string++) != '\0')
    {
        res = res * 10 + a - '0';
    }
    return res;
}

void itoa(long n, char *s)
{
    long i, sign;
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do
    {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    char c;
    for (long i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void sleep(long milisecond)
{
    for (long volatile i = 0; i < (long volatile)milisecond; i++);
        for (long volatile j = 0; j < (long volatile)64000000; j++);
}