#pragma once
#include <kernel.h>

int strlen(char *str);
uint8_t *strdup(uint8_t *str);
void reverse(char *s);
void *memcpy(void *dest, void *src, size_t n);
void *memset(void *dest, int val, size_t n);
int strcmp(char *str1, char *str2);
int strncmp(const char *s1, const char *s2, unsigned int n);
void strcpy(char *dest, char *src);
char *strncat(char *destination, const char *source, size_t num);
int strstr(char *str, char *strSub);