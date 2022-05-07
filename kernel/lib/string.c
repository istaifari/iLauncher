#include "../include/string.h"

int strlen(char *str)
{
    int len = 0;
    while ((*str++) != '\0')
        len++;
    return len;
}

void memcpy(void *dest, void *src, size_t n)
{
    char *csrc = (char *)src;
    char *cdest = (char *)dest;
    for (int i = 0; i < n; i++)
    {
        cdest[i] = csrc[i];
    }
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