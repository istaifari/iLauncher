#include <page.h>

extern void *heap_start, *heap_end, *heap_max, *heap_curr;
extern bool kheap_enabled;

uint8_t *temp_mem;
bool paging_enabled = 0;

page_directory_t *kpage_dir;

void PageFault_Handler(int_frame_t *reg)
{
    _reboot();
}

void *virtual2phys(page_directory_t *dir, void *virtual_addr)
{
    if (!paging_enabled)
    {
        return (void *)(virtual_addr - LOAD_MEMORY_ADDRESS);
    }
    uint32_t page_dir_idx = PAGEDIR_INDEX(virtual_addr), page_tbl_idx = PAGETBL_INDEX(virtual_addr), page_frame_offset = PAGEFRAME_INDEX(virtual_addr);
    if (!dir->ref_tables[page_dir_idx])
    {
        return NULL;
    }
    page_table_t *table = dir->ref_tables[page_dir_idx];
    if (!table->pages[page_tbl_idx].present)
    {
        return NULL;
    }
    uint32_t t = table->pages[page_tbl_idx].frame;
    t = (t << 12) + page_frame_offset;
    return (void *)t;
}

void *dumb_kmalloc(uint32_t size, int align)
{
    void *ret = temp_mem;
    if (align && !IS_ALIGN(ret))
        ret = (void *)PAGE_ALIGN(ret);
    temp_mem = temp_mem + size;
    return ret;
}

void allocate_region(page_directory_t *dir, uint32_t start_va, uint32_t end_va, int iden_map, int is_kernel, int is_writable)
{
    uint32_t start = start_va & 0xfffff000;
    uint32_t end = end_va & 0xfffff000;
    while (start <= end)
    {
        if (iden_map)
            allocate_page(dir, start, start / PAGE_SIZE, is_kernel, is_writable);
        else
            allocate_page(dir, start, 0, is_kernel, is_writable);
        start = start + PAGE_SIZE;
    }
}

void allocate_page(page_directory_t *dir, uint32_t virtual_addr, uint32_t frame, int is_kernel, int is_writable)
{
    page_table_t *table = NULL;
    if (!dir)
    {
        return;
    }
    uint32_t page_dir_idx = PAGEDIR_INDEX(virtual_addr), page_tbl_idx = PAGETBL_INDEX(virtual_addr);
    table = dir->ref_tables[page_dir_idx];
    if (!table)
    {
        if (!kheap_enabled)
            table = dumb_kmalloc(sizeof(page_table_t), 1);
        else
            table = kmalloc_a(sizeof(page_table_t));

        memset(table, 0, sizeof(page_table_t));
        uint32_t t = (uint32_t)virtual2phys(kpage_dir, table);
        dir->tables[page_dir_idx].frame = t >> 12;
        dir->tables[page_dir_idx].present = 1;
        dir->tables[page_dir_idx].rw = 1;
        dir->tables[page_dir_idx].user = 1;
        dir->tables[page_dir_idx].page_size = 0;
        dir->ref_tables[page_dir_idx] = table;
    }
    if (!table->pages[page_tbl_idx].present)
    {
        uint32_t t;
        if (frame)
            t = frame;
        else
            t = allocate_block();
        table->pages[page_tbl_idx].frame = t;
        table->pages[page_tbl_idx].present = 1;
        table->pages[page_tbl_idx].rw = 1;
        table->pages[page_tbl_idx].user = 1;
    }
}

void free_region(page_directory_t *dir, uint32_t start_va, uint32_t end_va, int free)
{
    uint32_t start = start_va & 0xfffff000;
    uint32_t end = end_va & 0xfffff000;
    while (start <= end)
    {
        free_page(dir, start, 1);
        start = start + PAGE_SIZE;
    }
}


void free_page(page_directory_t *dir, uint32_t virtual_addr, int free)
{
    if (dir == TEMP_PAGE_DIRECTORY)
        return;
    uint32_t page_dir_idx = PAGEDIR_INDEX(virtual_addr), page_tbl_idx = PAGETBL_INDEX(virtual_addr);
    if (!dir->ref_tables[page_dir_idx])
    {
        return;
    }
    page_table_t *table = dir->ref_tables[page_dir_idx];
    if (!table->pages[page_tbl_idx].present)
    {
        return;
    }
    if (free)
        free_block(table->pages[page_tbl_idx].frame);
    table->pages[page_tbl_idx].present = 0;
    table->pages[page_tbl_idx].frame = 0;
}

void paging_install()
{
    temp_mem = bitmap + bitmap_size;
    kpage_dir = dumb_kmalloc(sizeof(page_directory_t), 1);
    memset(kpage_dir, 0, sizeof(page_directory_t));
    uint32_t i = LOAD_MEMORY_ADDRESS;
    while (i < LOAD_MEMORY_ADDRESS + 4 * M)
    {
        allocate_page(kpage_dir, i, 0, 1, 1);
        i = i + PAGE_SIZE;
    }
    i = LOAD_MEMORY_ADDRESS + 4 * M;
    while (i < LOAD_MEMORY_ADDRESS + 4 * M + KHEAP_INITIAL_SIZE)
    {
        allocate_page(kpage_dir, i, 0, 1, 1);
        i = i + PAGE_SIZE;
    }
    idt_set_gate(14, PageFault_Handler, 0x08, 0x8E);
    switch_page_directory(kpage_dir, 0);
    enable_paging();
    allocate_region(kpage_dir, 0x0, 0x10000, 1, 1, 1);
}

void switch_page_directory(page_directory_t *page_dir, uint32_t phys)
{
    uint32_t t;
    if (!phys)
        t = (uint32_t)virtual2phys(TEMP_PAGE_DIRECTORY, page_dir);
    else
        t = (uint32_t)page_dir;
    asm volatile("mov %0, %%cr3" ::"r"(t));
}

void enable_paging()
{
    uint32_t cr0, cr4;
    asm volatile("mov %%cr4, %0"
                 : "=r"(cr4));
    CLEAR_PSEBIT(cr4);
    asm volatile("mov %0, %%cr4" ::"r"(cr4));
    asm volatile("mov %%cr0, %0"
                 : "=r"(cr0));
    SET_PGBIT(cr0);
    asm volatile("mov %0, %%cr0" ::"r"(cr0));
    paging_enabled = 1;
}

void *ksbrk(int size)
{
    void *runner = NULL;
    void *new_boundary;
    void *old_heap_curr;
restart_sbrk:
    old_heap_curr = heap_curr;
    if (size == 0)
    {
        goto ret;
    }
    else if (size > 0)
    {
        new_boundary = heap_curr + (uint32_t)size;
        if (new_boundary <= heap_end)
            goto update_boundary;
        else if (new_boundary > heap_max)
            return NULL;
        else if (new_boundary > heap_end)
        {
            runner = heap_end;
            while (runner < new_boundary)
            {
                allocate_page(kpage_dir, (uint32_t)runner, 0, 1, 1);
                runner = runner + PAGE_SIZE;
            }
            if (old_heap_curr != heap_curr)
            {
                goto restart_sbrk;
            }
            heap_end = runner;
            goto update_boundary;
        }
    }
    else if (size < 0)
    {
        new_boundary = (void *)((uint32_t)heap_curr - (uint32_t)abs(size));
        if (new_boundary < heap_start + HEAP_MIN_SIZE)
        {
            new_boundary = heap_start + HEAP_MIN_SIZE;
        }
        runner = heap_end - PAGE_SIZE;
        while (runner > new_boundary)
        {
            free_page(kpage_dir, (uint32_t)runner, 1);
            runner = runner - PAGE_SIZE;
        }
        heap_end = runner + PAGE_SIZE;
        goto update_boundary;
    }
update_boundary:
    heap_curr = new_boundary;
ret:
    return old_heap_curr;
}

void copy_page_directory(page_directory_t *dst, page_directory_t *src)
{
    for (uint32_t i = 0; i < 1024; i++)
    {
        if (kpage_dir->ref_tables[i] == src->ref_tables[i])
        {
            dst->tables[i] = src->tables[i];
            dst->ref_tables[i] = src->ref_tables[i];
        }
        else
        {
            dst->ref_tables[i] = copy_page_table(src, dst, i, src->ref_tables[i]);
            uint32_t phys = (uint32_t)virtual2phys(src, dst->ref_tables[i]);
            dst->tables[i].frame = phys >> 12;
            dst->tables[i].user = 1;
            dst->tables[i].rw = 1;
            dst->tables[i].present = 1;
        }
    }
}

page_table_t *copy_page_table(page_directory_t *src_page_dir, page_directory_t *dst_page_dir, uint32_t page_dir_idx, page_table_t *src)
{
    page_table_t *table = (page_table_t *)kmalloc_a(sizeof(page_table_t));
    for (int i = 0; i < 1024; i++)
    {
        if (!table->pages[i].frame)
            continue;
        uint32_t src_virtual_address = (page_dir_idx << 22) | (i << 12) | (0);
        uint32_t dst_virtual_address = src_virtual_address;
        uint32_t tmp_virtual_address = 0;
        allocate_page(dst_page_dir, dst_virtual_address, 0, 0, 1);
        allocate_page(src_page_dir, tmp_virtual_address, (uint32_t)virtual2phys(dst_page_dir, (void *)dst_virtual_address), 0, 1);
        if (src->pages[i].present)
            table->pages[i].present = 1;
        if (src->pages[i].rw)
            table->pages[i].rw = 1;
        if (src->pages[i].user)
            table->pages[i].user = 1;
        if (src->pages[i].accessed)
            table->pages[i].accessed = 1;
        if (src->pages[i].dirty)
            table->pages[i].dirty = 1;
        memcpy((void *)tmp_virtual_address, (void *)src_virtual_address, PAGE_SIZE);
        free_page(src_page_dir, tmp_virtual_address, 0);
    }
    return table;
}
