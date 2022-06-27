#pragma once
#include "../kernel.h"

struct time_t
{
    long number[3];
    char *string[3];
};

typedef struct clock
{
    struct time_t *time;
    long volatile timeout;
} clock_t;

extern clock_t *timer;

void clock_update();
void InitTimer();
