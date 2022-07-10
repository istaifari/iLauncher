#include <stdlib.h>

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

long atoi(const char *str)
{
    long res = 0;
    long sign = 1;
    long i = 0;
    if (str[0] == '-') {
        sign = -1;
        i++;
    }
    for (; str[i] != '\0'; i++)
        res = res * 10 + str[i] - '0';
    return sign * res;
}

void *itoa(long res, char *a)
{
    long size = 1;
    long sign = 1;
    if (res < 0)
        sign = -1;
    if (sign == -1)
    {
        res = -res;
        a[size] = '-';
    }
    long t = res;
    while (t / 10 != 0)
    {
        t = t / 10;
        size++;
    }
    a[size] = '\0';
    t = res;
    long i = size - 1;
    while (i >= 0)
    {
        if (sign == -1)
            a[i+1] = (t % 10) + '0';
        else
            a[i] = (t % 10) + '0';
        t = t / 10;
        i--;
    }
    return a;
}

void sleep(long milisecond)
{
    /*for (long volatile i = 0; i < (long volatile)milisecond; i++)
        for (long volatile j = 0; j < (long volatile)64000000; j++)
            ;*/
    long tmp = global_timer->timefull + milisecond;
    while (global_timer->timefull >= tmp)
        ;
}