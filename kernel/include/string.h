#pragma once
#ifndef _STRING_H
#define _STRING_H
#include <stddef.h>

int strlen(char *str);
void reverse(char *s);
int strcmp(char *str1,char *str2);
void strcpy(char* buf_to,char* buf_from);
char* strncat(char* destination, const char* source, size_t num);
int strstr(char* str, char* strSub);
#endif  