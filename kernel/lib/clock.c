#include <clock.h>

clock_t *timer;

__attribute__((interrupt)) void TimerInt_Handler(int_frame_t *r)
{
    asm("cli");
    clock_update(timer);
    PIC_End();
    asm("sti");
}

void clock_timer_phase(long hz)
{
    long divisor = 1193180 / hz;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

void InitTimer()
{
    if (!timer)
    {
        asm("cli");
        timer = kmalloc(sizeof(clock_t) * ((sizeof(struct time_t) * ((sizeof(long) * 4))) * (sizeof(char) * (2 * 3))));
        timer->time = kmalloc((sizeof(struct time_t) * ((sizeof(long) * 4))) * (sizeof(char) * (2 * 3)));
        timer->time->number = kmalloc(sizeof(long) * 4);
        timer->time->number[3] = 0;
        timer->time->number[2] = 0;
        timer->time->number[1] = 0;
        timer->time->number[0] = 0;
        timer->time->hh = kmalloc(sizeof(char) * (2));
        memset(timer->time->hh, '0', 1);
        timer->time->mm = kmalloc(sizeof(char) * (2));
        memset(timer->time->mm, '0', 1);
        timer->time->ss = kmalloc(sizeof(char) * (2));
        memset(timer->time->ss, '0', 1);
        timer->timeout = 0;
        clock_timer_phase(100);
        asm("sti");
    }
    idt_set_gate(32 + 0, TimerInt_Handler, 0x08, 0x8E);
}

void *clock_convert1to01(char *str)
{
    long number = atoi(str);
    if (number < 10)
    {
        str[1] = str[0];
        str[2] = str[0];
        str[0] = '0';
        str[2] = '\0';
    }
}

void *clock_read(clock_t *object, long index)
{
    char *format01 = "00";
    char *format1 = "0";
    long number = object->time->number[index];
    if (number < 10)
    {
        memset(format01, '0', 1);
        itoa(number, format1);
        format01[1] = format1[0];
    }
    else
    {
        memset(format01, '0', 1);
        itoa(number, format01);
    }
    return format01;
}

void *clock_fullread(clock_t *object)
{
    char *format = "--:--:--";
    memcpy(format, "--:--:--", 8);
    if (atoi(object->time->hh) < 10)
    {
        format[2 - 2] = '0';
        format[2 - 1] = object->time->hh[0];
    }
    else
    {
        format[2 - 2] = object->time->hh[0];
        format[2 - 1] = object->time->hh[1];
    }
    if (atoi(object->time->mm) < 10)
    {
        format[5 - 2] = '0';
        format[5 - 1] = object->time->mm[0];
    }
    else
    {
        format[5 - 2] = object->time->mm[0];
        format[5 - 1] = object->time->mm[1];
    }
    if (atoi(object->time->ss) < 10)
    {
        format[8 - 2] = '0';
        format[8 - 1] = object->time->ss[0];
    }
    else
    {
        format[8 - 2] = object->time->ss[0];
        format[8 - 1] = object->time->ss[1];
    }
    format[8] = '\0';
    return format;
}

void *clock_settime(clock_t *object, const char *time)
{
    char *hh = time + (2 - 2);
    char *mm = time + (5 - 2);
    char *ss = time + (8 - 2);
    for (size_t i = 0; i < 6; i++)
    {
        if (i < 3)
            mm[2 + i] = '\0';
        hh[2 + i] = '\0';
    }
    object->time->number[3] = atoi(hh);
    object->time->number[2] = atoi(mm);
    object->time->number[1] = atoi(ss);
}

void clock_update(clock_t *object)
{
    audio_stop_beep();
    object->timeout = 0;
    if (object->time->number[3] >= 24)
    {
        object->time->number[3] = 0;
        object->time->number[2] = 0;
        object->time->number[1] = 0;
        object->time->number[0] = 0;
    }
    if (object->time->number[2] >= 60)
    {
        object->time->number[2] = 0;
        object->time->number[3]++;
    }
    if (object->time->number[1] >= 60)
    {
        object->time->number[1] = 0;
        object->time->number[2]++;
    }
    if (object->time->number[0] >= 100)
    {
        object->time->number[0] = 0;
        object->time->number[1]++;
        audio_start_beep(1000);
    }
    else
    {
        object->time->number[0]++;
    }
    itoa(object->time->number[1], object->time->ss);
    itoa(object->time->number[2], object->time->mm);
    itoa(object->time->number[3], object->time->hh);
    object->timefull = object->time->number[0] + object->time->number[1] + object->time->number[2];
}