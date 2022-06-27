#include "../include/clock.h"

clock_t *timer;

__attribute__((interrupt)) void HandleTimer(int_frame_t *r)
{
    clock_update(timer);
    PIC_End();
}

/* Coming Soon */

void timer_phase(long hz)
{
    long divisor = 1193180 / hz;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

void InitTimer()
{
    timer->time->number[2] = 0;
    timer->time->number[1] = 0;
    timer->time->number[0] = 0;
    timer->time->string[2] = "00";
    timer->time->string[1] = "00";
    timer->time->string[0] = "00";
    timer->timeout = 0;
    timer_phase(100);
}

void *clock_tostring(clock_t *object, long i)
{
    char *tmp_str = "00";
    long tmp_int = object->time->number[i];
    itoa(tmp_int, tmp_str);
    return tmp_str;
}

void clock_update(clock_t *object)
{
    audio_stop_beep();
    if (++object->timeout >= 100)
    {
        object->timeout = 0;
        if (object->time->number[2] >= 23)
        {
            object->time->number[2] = 0;
            object->time->number[1] = 0;
            object->time->number[0] = 0;
        }
        if (object->time->number[1] >= 59)
        {
            object->time->number[1] = 0;
            object->time->number[2]++;
        }
        if (object->time->number[0] >= 59)
        {
            object->time->number[0] = 0;
            object->time->number[1]++;
        }
        else
        {
            object->time->number[0]++;
        }
        for (long i = 0; i < 2; i++)
            object->time->string[i] = clock_tostring(object, i);
        audio_start_beep(1000);
    }
}