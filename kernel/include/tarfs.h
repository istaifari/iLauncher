#pragma once
#include "../kernel.h"

struct tar_header
{
    int8_t name[128];
    int8_t mode[8];
    int8_t uid[8];
    int8_t gid[8];
    int8_t size[12];
    int8_t mtime[12];
    int8_t chksum[8];
    int8_t typeflag;
    int8_t linkname[128];
    int8_t magic[6];
    int8_t version[2];
    int8_t uname[32];
    int8_t gname[32];
    int8_t devmajor[8];
    int8_t devminor[8];
    int8_t prefix[131];
    int8_t atime[12];
    int8_t ctime[12];
};
typedef struct tar_header tar_header_t;

extern struct fs_node *tarfs_root_node;

enum tar_typeflag
{
    REGTYPE = '0',
    AREGTYPE = '\0',
    LNKTYPE = '1',
    CHRTYPE = '3',
    BLKTYPE = '4',
    DIRTYPE = '5',
    FIFOTYPE = '6',
};

void *tarfs_install();
uint64_t tarfs_convert_number(char *num);
void *tarfs_get_file(struct tar_header *node, const char *filename);
size_t tarfs_get_len(struct tar_header *node, const char *filename);