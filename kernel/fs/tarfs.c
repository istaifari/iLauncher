#include <tarfs.h>

fs_node_t *tarfs_root_node = 0;
tar_header_t *headers[64];
static uint32_t files = 0;

fs_node_t *tarfs_finddir(fs_node_t *node, char *name);
struct dirent *tarfs_readdir(fs_node_t *node, uint32_t index);
uint32_t tarfs_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t tarfs_parse(unsigned int address, uint32_t end);
uint32_t tarfs_getsize(const char *in);

static uint8_t _find_name(list_t *entry, void *udata)
{
    if (!strcmp(((fs_node_t *)(entry->data))->name, (uint8_t *)udata))
        return false;
    else
        return true;
}

fs_node_t *tarfs_root()
{
    return tarfs_root_node;
}

static inline uintptr_t round_down(uintptr_t val, uintptr_t place)
{
    return val & ~(place - 1);
}

static inline uintptr_t round_up(uintptr_t val, uintptr_t place)
{
    return round_down(val + place - 1, place);
}

fs_node_t *tarfs_finddir(fs_node_t *node, char *name)
{
    uint32_t i;
    list_t *nodes_list, *entry;

    if (name == NULL)
        return NULL;

    if (node->flags & FS_DIRECTORY)
    {
        nodes_list = (list_t *)node->inode;
        entry = list_foreach(nodes_list, _find_name, name);
        if (entry)
            return (fs_node_t *)entry->data;
    }

    return NULL;
}

struct dirent *tarfs_readdir(fs_node_t *node, uint32_t index)
{
    struct dirent *d;
    fs_node_t *i_node;
    list_t *nodes_list, *entry;

    if (node == NULL)
        return NULL;

    if (node->flags & FS_DIRECTORY)
    {

        nodes_list = (list_t *)node->inode;

        entry = list_get(nodes_list, index);

        if (entry == NULL)
            return NULL;

        i_node = (fs_node_t *)entry->data;

        d = (struct dirent *)kmalloc(sizeof(struct dirent));

        if (d == NULL)
            return NULL;

        strcpy(d->name, i_node->name);
        d->inode = i_node->inode;
        d->offset = index;
        d->flags = i_node->flags;

        return d;
    }

    return NULL;
}

fs_node_t *tarfs_create_path(uint8_t **p)
{
    fs_node_t *parent, *node;
    uint32_t s = 0;
    list_t *entry;
    if (p == NULL)
        return tarfs_root_node;

    parent = tarfs_root_node;
    while (p[s + 1] != NULL)
    {
        entry = list_foreach((list_t *)parent->inode, _find_name, p[s]);
        if (entry)
        {
            parent = (fs_node_t *)entry->data;
        }
        else
        {
            node = (fs_node_t *)kmalloc(sizeof(fs_node_t));
            strcpy(node->name, p[s]);
            // printf("create %s/%s \n", parent->name, p[s]);
            node->inode = 0; // if has child, when child added will be the ptr for the list_t
            node->flags = FS_DIRECTORY;
            node->read = 0;
            node->write = 0;
            node->open = 0;
            node->close = 0;
            node->readdir = &tarfs_readdir;
            node->finddir = &tarfs_finddir;
            parent->inode = (uint32_t)list_add((list_t *)parent->inode, node);
            parent = node;
        }

        s++;
    }

    return parent;
}

uint32_t tarfs_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    uint32_t file_sz;
    file_sz = tarfs_getsize(headers[node->inode]->size);
    if (offset > file_sz)
        return 0;
    if ((offset + size) > file_sz)
        size = file_sz - offset;
    memcpy((uint8_t *)((uint32_t)headers[node->inode] + 512), buffer, size);
    return size;
}

uint64_t tarfs_convert_number(char *num)
{
    size_t len = strlen(num);
    uint64_t value = 0;

    for (size_t place = 0; place < len; place += 1)
    {
        uint64_t part = num[place] - '0';
        uint64_t shift = (len - 1 - place) * 3;
        value += part << shift;
    }

    return value;
}

void *tarfs_install()
{
    placement_address = *(uint32_t *)(mboot_ptr->mods_addr + LOAD_MEMORY_ADDRESS);
    uint32_t i, f = 0;

    tarfs_parse(*(uint32_t *)(mboot_ptr->mods_addr), placement_address);

    tarfs_root_node = (fs_node_t *)kmalloc(sizeof(fs_node_t));
    strcpy(tarfs_root_node->name, "ram");
    tarfs_root_node->flags = FS_DIRECTORY;
    tarfs_root_node->read = 0;
    tarfs_root_node->write = 0;
    tarfs_root_node->open = 0;
    tarfs_root_node->close = 0;
    tarfs_root_node->readdir = &tarfs_readdir;
    tarfs_root_node->finddir = &tarfs_finddir;
    tarfs_root_node->inode = 0;

    uint8_t s = 0;
    fs_node_t *p_node, *node;
    uint8_t **p;

    for (i = 0; i < files; i++)
    {
        p = path(headers[i]->name);

        p_node = tarfs_create_path(p);

        s = 0;
        while (p[s++])
            ;
        s--;

        switch (headers[i]->typeflag)
        {
        case REGTYPE:
        case AREGTYPE:

            node = (fs_node_t *)kmalloc(sizeof(fs_node_t));

            strcpy(node->name, p[s - 1]);
            node->inode = i;
            node->flags = FS_FILE;
            node->read = &tarfs_read;
            node->write = 0;
            node->open = 0;
            node->close = 0;
            node->readdir = 0;
            node->finddir = 0;

            f++;

            break;
        case DIRTYPE:

            p_node = tarfs_create_path(p);

            node = (fs_node_t *)kmalloc(sizeof(fs_node_t));

            strcpy(node->name, p[s - 1]);
            node->inode = 0;
            node->flags = FS_DIRECTORY;
            node->read = 0;
            node->write = 0;
            node->open = 0;
            node->close = 0;
            node->readdir = &tarfs_readdir;
            node->finddir = &tarfs_finddir;
            break;
        default:
            break;
        }

        // printf("add %s to %s parent\n", node->name, p_node->name);
        p_node->inode = (uint32_t)list_add((list_t *)p_node->inode, node);
        path_free(p);
    }
    return (struct tar_header *)(*(uint32_t *)(mboot_ptr->mods_addr));
    return tarfs_root_node;
}

void *tarfs_get_file(struct tar_header *node, const char *filename)
{
    if (!node)
        return NULL;
    while (node->name[0])
    {
        if (strcmp(node->name, filename) == 0)
            return (char *)node + 512;
        if (node->name[0] == '\0')
            break;

        size_t len = tarfs_convert_number(node->size);

        uintptr_t next_tar = (uintptr_t)node;
        next_tar += len + 0x200;
        next_tar = round_up(next_tar, 512);

        node = (struct tar_header *)next_tar;
    }

    return NULL;
}

size_t tarfs_get_len(struct tar_header *node, const char *filename)
{
    if (!node)
        return 0;
    while (node->name[0])
    {
        size_t len = tarfs_convert_number(node->size);

        if (strcmp(node->name, filename) == 0)
            return len;
        if (node->name[0] == '\0')
            break;

        uintptr_t next_tar = (uintptr_t)node;
        next_tar += len + 0x200;
        next_tar = round_up(next_tar, 512);

        node = (struct tar_header *)next_tar;
    }

    return 0;
}

uint32_t tarfs_getsize(const char *in)
{

    uint32_t size = 0;
    uint32_t j;
    uint32_t count = 1;

    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);

    return size;
}

uint32_t tarfs_parse(unsigned int address, uint32_t end)
{
    for (files = 0;; files++)
    {
        struct tar_header *header = (struct tar_header *)address;

        if (header->name[0] == '\0')
            break;

        uint32_t size = tarfs_getsize(header->size);

        headers[files] = header;

        address += ((size / 512) + 1) * 512;

        if (size % 512)
            address += 512;

        if (address > end)
            break;
    }
    return files;
}