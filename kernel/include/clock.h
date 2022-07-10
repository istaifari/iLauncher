#pragma once
#include <kernel.h>

struct time_t
{
    long *number;
    char *ss;
    char *mm;
    char *hh;
};

typedef struct clock
{
    struct time_t *time;
    long timefull;
    long volatile timeout;
} clock_t;

extern clock_t *global_timer;

void clock_update();
void *clock_read(clock_t *object, long index);
void *clock_fullread(clock_t *object);
void *clock_settime(clock_t *object, const char *time);
void InitTimer();
