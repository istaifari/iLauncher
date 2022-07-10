#pragma once
#include <kernel.h>

#define LIST_MAGIC 0xC0FFBABE

typedef struct list
{

    uint32_t magic;
    void *data;

    struct list *next;
} list_t;

typedef uint8_t (*list_iterator_t)(list_t *l, void *udata);

list_t *
list_add(list_t *l, void *data);
list_t *
list_get(list_t *l, uint32_t idx);
list_t *
list_foreach(list_t *l, list_iterator_t it, void *udata);