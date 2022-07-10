#pragma once
#include <kernel.h>

#define KHEAP_START (void *)0xC0400000
#define KHEAP_INITIAL_SIZE 48 * M
#define KHEAP_MAX_ADDRESS (void *)0xCFFFFFFF
#define HEAP_MIN_SIZE 4 * M

#define PAGE_SIZE 4096
#define OVERHEAD (sizeof(struct Block) + sizeof(unsigned int))

struct Block
{
    unsigned int size;
    struct Block *prev;
    struct Block *next;
};

unsigned int kmalloc_int(unsigned int sz, int align, unsigned int *phys);
void *kmalloc_cont(unsigned int sz, int align, unsigned int *phys);
void *kmalloc_a(unsigned int sz);
unsigned int kmalloc_p(unsigned int sz, unsigned int *phys);
unsigned int kmalloc_ap(unsigned int sz, unsigned int *phys);
void *kmalloc(unsigned int sz);
void kfree(void *p);
void *krealloc(void *ptr, unsigned int size);
void db_print();
int doesItFit(struct Block *n, unsigned int size);
void setFree(unsigned int *size, int x);
void removeNodeFromFreelist(struct Block *x);
void addNodeToFreelist(struct Block *x);
int isBetter(struct Block *node1, struct Block *node2);
struct Block *bestfit(unsigned int size);
struct Block *getPrevBlock(struct Block *n);
struct Block *getNextBlock(struct Block *n);
unsigned int getRealSize(unsigned int size);
unsigned int getSbrkSize(unsigned int size);
int isFree(struct Block *n);
int isEnd(struct Block *n);
void kheap_install(void *start, void *end, void *max);
void *malloc(unsigned int size);
void free(void *ptr);
void *kcalloc(unsigned int num, unsigned int size);
void *realloc(void *ptr, unsigned int size);