/*
 * mm-naive.c - The least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by allocating a
 * new page as needed.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 *
 * Jacob Xu, u1448572
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/* === Constants === */
#define PTR_SIZE (sizeof(void*))            // Size of a pointer
#define ALIGNMENT 16                        // All payloads are aligned to this
#define HEADER_SIZE ALIGNMENT               // Block header size (aligned to 16 bytes)
#define BLOCK_OVERHEAD (2 * HEADER_SIZE)    // Header + Footer size
#define PAGE_LINKS_SIZE ALIGN(2 * PTR_SIZE) // Space for prev/next page links
#define INIT_PAGE_SIZE (4 * mem_pagesize()) // Initial page size
#define MAX_PAGE_SIZE (100 * INIT_PAGE_SIZE) // Maximum page size
#define PAGE_OVERHEAD (PAGE_LINKS_SIZE + (3 * HEADER_SIZE)) // Overhead for metadata
#define MIN_BLOCK_SIZE (BLOCK_OVERHEAD + (2 * PTR_SIZE))    // Minimum allocatable block


/* === Utility Macros === */
#define MAX(a, b) ((a) > (b) ? (a) : (b))   // Return maximum of two values
#define MIN(a, b) ((a) < (b) ? (a) : (b))   // Return minimum of two values
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1)) // Align to multiple of 16
#define PAGE_ALIGN(size) (((size) + (mem_pagesize() - 1)) & ~(mem_pagesize() - 1)) // Align to page size

/* iterate the pagelist, head to tail */
#define FOR_EACH_PAGE(page_cursor) \
    for (page_cursor = first_page; \
         page_cursor != NULL; \
         page_cursor = get_next_page(page_cursor))
/* iterate on the blocklist of a page, prologue (including) to epilogue (excluding) */
#define FOR_EACH_BLOCK_IN_PAGE(page, block_cursor) \
    for (block_cursor = page + PAGE_LINKS_SIZE; \
         get_memory_block_size((header_t*)block_cursor) != HEADER_SIZE; \
         block_cursor = ((void*)block_cursor) + get_memory_block_size((header_t*)block_cursor))
/* iterate on freelist, starting at freelist_head */
#define FOR_EACH_BLOCK_IN_FREELIST(block_cursor) \
    for (block_cursor = freelist_head; \
         block_cursor != NULL && get_next_free_block(block_cursor) != NULL; \
         block_cursor = (void*)get_next_free_block(block_cursor))

/* === Type Definitions === */
typedef unsigned int header_t; // Header/footer size type

/* === Global Variables === */
void* first_page;         // Pointer to the first page in the list
void* last_page;          // Pointer to the last page (unused but reserved for expansion)
header_t* freelist_head;  // Head of the free list
size_t minimum_page_size; // Current minimum page size

/* Helper functions */
void* create_new_page(size_t min_freeblock_size);
void* attempt_reallocation(void* first_page, size_t new_block_size);
void* reallocate_block(header_t* free_block_pointer, size_t new_block_size);
void* put_free_block(header_t* start, size_t total_block_size);
void* put_block(header_t* header, size_t blocksize);
void link_freeblock(header_t* freeblock);
void unlink_freeblock(header_t* freeblock);
void unlink_freepage(void* page);
void link_page(void* page);
void unmap_containing_page_if_free(header_t* block);
void pack_header(header_t* header_ptr, size_t block_size, int alloc_flag);

/* Pointer arithmetic and bitwise operations functions */
void* get_previous_page(void* current_page);
void* get_next_page(void* current_page);
void set_previous_page(void* current_page, void* previous_page);
void set_next_page(void* current_page, void* next_page);
int is_memory_block_allocated(header_t* block_header);
size_t get_memory_block_size(header_t* block_header);
header_t* get_block_header_from_payload(void* payload_ptr);
header_t* get_block_footer_from_header(header_t* block_header);
header_t* get_next_free_block(header_t* current_free_block);
header_t* get_previous_free_block(header_t* current_free_block);
void set_next_free_block(header_t* current_free_block, header_t* next_free_block);
void set_previous_free_block(header_t* current_free_block, header_t* previous_free_block);
header_t* get_next_memory_block(header_t* current_block);
header_t* get_previous_memory_block(header_t* current_block);


/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    /* do some basic checks on the system */
    if (sizeof(header_t ) > HEADER_SIZE)
    {
        printf("SYSTEM ERROR! sizeof(header_t) > HEADER_SIZE. \n");
        return 1;
    }

    first_page = NULL;
    last_page = NULL;
    freelist_head = NULL;
    minimum_page_size = INIT_PAGE_SIZE;

    create_new_page(minimum_page_size - PAGE_OVERHEAD); /* auto-inits head/tail page */
    if (first_page == NULL)
    {
        return 1;
    }

    //check_heap();
    return 0;
}

/* 
 * mm_malloc - Allocate a block by using bytes from current_page,
 *     grabbing a new page if necessary.
 */
void* mm_malloc(size_t size)
{
    size_t new_block_size;
    void* block_pointer;
    void* new_page;

    new_block_size = MAX(MIN_BLOCK_SIZE, ALIGN(size) + BLOCK_OVERHEAD);
    
    /* attempt to put block in the free spots of the heap */
    block_pointer = attempt_reallocation(first_page, new_block_size);
    if (block_pointer == NULL)
    {
        /* no free spot found, make new page (link at the end of pagelist) */
        new_page = create_new_page(new_block_size);
        if (new_page == NULL)
        {
            return NULL; /* page could not be made */
        }

        /* put the block in the new page */
        block_pointer = attempt_reallocation(new_page, new_block_size);
        if (block_pointer == NULL)
        {
            printf("MALLOC ERROR! New page not big enough for its block. \n");
            return NULL;
        }
    }
    
    //check_heap();
    return block_pointer;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void* payload_ptr)
{
    header_t* header_ptr;
    header_t* footer_ptr;
    header_t* next_block;
    header_t* prev_block;
    size_t block_size;
    //void* containing_page;

    header_ptr = get_block_header_from_payload(payload_ptr);
    block_size = get_memory_block_size(header_ptr);

    /* coalesce adjacent free blocks */
    next_block = get_next_memory_block(header_ptr);
    prev_block = get_previous_memory_block(header_ptr);
    if (!is_memory_block_allocated(next_block))
    {
        unlink_freeblock(next_block);
        block_size += get_memory_block_size(next_block);
    }
    if (!is_memory_block_allocated(prev_block))
    {
        unlink_freeblock(prev_block);
        header_ptr = prev_block;
        block_size += get_memory_block_size(prev_block);
    }

    /* add header/footer for new, coalesced block */
    pack_header(header_ptr, block_size, 0);
    footer_ptr = get_block_footer_from_header(header_ptr);
    pack_header(footer_ptr, block_size, 0);

    /* add to freelist */
    link_freeblock(header_ptr);

    /* unmap the containing page if it no longer has any allocated blocks */
    unmap_containing_page_if_free(header_ptr);

    //check_heap();
    return;
}

/* === Page Management === */

/**
 * Create and initialize a new page.
 * 
 * @param min_block_size Minimum size of the free block in the new page.
 * @return Pointer to the new page, or NULL if allocation fails.
 */
void* create_new_page(size_t min_freeblock_size)
{
    void* new_page;
    void* placeholder;
    size_t page_total_size;
    size_t free_block_size;

    page_total_size = MAX(ALIGN(min_freeblock_size) + PAGE_OVERHEAD, minimum_page_size);
    free_block_size = page_total_size - PAGE_OVERHEAD;
    minimum_page_size = MIN(2 * minimum_page_size, MAX_PAGE_SIZE);

    /* create a blank page */
    new_page = mem_map(page_total_size);
    if (new_page == NULL)
    {
        return NULL;
    }

    link_page(new_page);

    /* add a prologue */
    placeholder = new_page + PAGE_LINKS_SIZE;
    pack_header((header_t*)placeholder, 2 * HEADER_SIZE, 1);
    placeholder += HEADER_SIZE;
    pack_header((header_t*)placeholder, 2 * HEADER_SIZE, 1);
    placeholder += HEADER_SIZE;

    /* add a giant free block for all the allocatable space */
    placeholder = put_free_block((header_t*)placeholder, free_block_size);

    /* add an epilogue */
    pack_header((header_t*)placeholder, HEADER_SIZE, 1);

    return new_page;
}

/*
 * Attempt to allocate a block within the free blocks of the heap.
 *
 * @new_block_size: size in bytes of new block (including header/footer)
 * @return: pointer to the allocated block, or null if there was no space.
 */
void* attempt_reallocation(void* first_page, size_t new_block_size)
{
    void* block_cursor;
    //void* smallest_block;
    //size_t blocksize;

    block_cursor = NULL;
    FOR_EACH_BLOCK_IN_FREELIST(block_cursor)
    {
        if (get_memory_block_size(block_cursor) >= new_block_size)
        {
            return reallocate_block(block_cursor, new_block_size);
        }
    }
    return NULL;
}

/*
 * Allocate a block using a free block.
 * CAUTION! Assumes that the size of the free block pointed to by
 * @free_block_pointer is >= to @new_block_size
 *
 * @free_block_pointer: pointer to start (header) of the free block.
 * @new_block_size: size in bytes of entire new block (including header/footer)
 * @return: pointer to new block payload
 */
void* reallocate_block(header_t* freeblock, size_t new_block_size)
{
    size_t freeblock_size;
    size_t remaining_blocksize;
    void* new_payload_ptr;
    header_t* remaining_freeblock;

    freeblock_size = get_memory_block_size(freeblock);
    remaining_blocksize = freeblock_size - new_block_size;
    if (remaining_blocksize < MIN_BLOCK_SIZE)
    {
        /* only make a new block at the end if theres enough space */
        new_block_size = freeblock_size;
        remaining_blocksize = 0;
    }


    unlink_freeblock(freeblock);

    /* put the new block at the start of the free block */
    new_payload_ptr = put_block(freeblock, new_block_size);

    /* re-make the remainder of the free block if needed */
    if (remaining_blocksize > 0)
    {
        remaining_freeblock = ((void*)freeblock) + new_block_size;
        put_free_block(remaining_freeblock, remaining_blocksize);
    }


    return new_payload_ptr;
}

/*
 * Place an allocated block.
 *
 * @return: a pointer to the new payload.
 */
void* put_block(header_t* header, size_t blocksize)
{
    pack_header(header, blocksize, 1);
    pack_header(((void*)header) + blocksize - HEADER_SIZE, blocksize, 1);

    return (((void*)header) + HEADER_SIZE);
}

/*
 * Place a free block in a page.
 *
 * @header_ptr: the first byte of the free block.
 * @total_block_size: size in bytes of block including header/footer.
 * @return: byte just after the free block.
 */
void* put_free_block(header_t* header_ptr, size_t total_block_size)
{
    header_t* footer_ptr;

    pack_header(header_ptr, total_block_size, 0);
    /* must get footer_ptr after packing header */
    footer_ptr = get_block_footer_from_header(header_ptr);
    pack_header(footer_ptr, total_block_size, 0);

    link_freeblock(header_ptr);

    return ((void*)header_ptr + total_block_size);
}

/*
 * Prepend a freeblock to the freelist.
 */
void link_freeblock(header_t* header_ptr)
{
    /* prepend to freelist */
    if (freelist_head == NULL)
    {
        set_next_free_block(header_ptr, NULL);
        set_previous_free_block(header_ptr, NULL);

        freelist_head = header_ptr;
    }
    else
    {
        set_previous_free_block(freelist_head, header_ptr);
        set_previous_free_block(header_ptr, NULL);
        set_next_free_block(header_ptr, freelist_head);

        freelist_head = header_ptr;
    }
}

/*
 * Unlink a freeblock from the freelist.
 */
void unlink_freeblock(header_t* freeblock)
{
    if (get_previous_free_block(freeblock) != NULL)
    {
        set_next_free_block(get_previous_free_block(freeblock),
                           get_next_free_block(freeblock));
    }
    if (get_next_free_block(freeblock) != NULL)
    {
        set_previous_free_block(get_next_free_block(freeblock),
                           get_previous_free_block(freeblock));
    }
    if (freeblock == freelist_head)
    {
        freelist_head = get_next_free_block(freeblock);
    }
}

/*
 * Unlink a (presumably free) page from the pagelist.
 */
void unlink_freepage(void* page)
{
    if (get_previous_page(page) != NULL)
    {
        set_next_page(get_previous_page(page),
                      get_next_page(page));
    }
    if (get_next_page(page) != NULL)
    {
        set_previous_page(get_next_page(page),
                      get_previous_page(page));
    }
    if (page == first_page)
    {
        first_page = get_next_page(page);
    }
    if (page == last_page)
    {
        last_page = get_previous_page(page);
    }
}

/*
 * Append a page to the pagelist.
 */
void link_page(void* page)
{
    /* append the page the pagelist */
    if (last_page == NULL)
    {
        /* this is the first page, init the pagelist */
        set_previous_page(page, NULL);
        set_next_page(page, NULL);
        first_page = page;
        last_page = page;
    }
    else
    {
        set_next_page(last_page, page);
        set_previous_page(page, last_page);
        set_next_page(page, NULL);
        last_page = page;
    }
}

/*
 * Determine if there are any allocated blocks in a page.
 *
 * @return: 0 if there are allocated blocks, 1 if there are not.
 */
int is_page_free(void* page)
{
    header_t* block_cursor;
    size_t blocksize;

    FOR_EACH_BLOCK_IN_PAGE(page, block_cursor)
    {
        blocksize = get_memory_block_size(block_cursor);
        /* if block is allocd, and it is not the prologue or epilogue */
        if (blocksize != HEADER_SIZE &&
            blocksize != (2 * HEADER_SIZE) &&
            is_memory_block_allocated(block_cursor))
        {
            return 0;
        }
    }
    return 1;
}

/*
 * If the page has no allocated blocks in it, then unlink the freeblock(s) in a
 * page, unlink the page, and unmap the page.
 *
 * This is in its own helper so it can shortcircuit the process and just return
 * if it finds an allocated block in the process of iterating backwards to the
 * start of the page from the given block. It'd be more readable to split the
 * logic into their own helpers, but this would not allow the shortcircuits.
 */
void unmap_containing_page_if_free(header_t* block)
{
    void* placeholder;
    header_t* block_cursor;
    size_t blocksize;
    size_t size_so_far;

    /* put placeholder at the page prologue by iterating backwards */
    placeholder = (void*)block;
    while (get_memory_block_size(placeholder) != (2 * HEADER_SIZE))
    {
        if (is_memory_block_allocated(placeholder))
        {
            /* shortcicruit the whole thing if an alloc'd block is seen */
            return;
        }

        placeholder = get_previous_memory_block(placeholder);
    }

    /* move from prologue to start of page */
    placeholder = placeholder - PAGE_LINKS_SIZE;

    /* look through whole page for free ptrs and calculate page size */
    size_so_far = PAGE_LINKS_SIZE;
    FOR_EACH_BLOCK_IN_PAGE(placeholder, block_cursor)
    {
        blocksize = get_memory_block_size(block_cursor);
        size_so_far += blocksize;
        /* if block is allocd, and it is not the prologue or epilogue */
        if (blocksize != HEADER_SIZE &&
            blocksize != (2 * HEADER_SIZE) &&
            is_memory_block_allocated(block_cursor))
        {
            /* shortcicruit, found allocd block */
            return;
        }
    }
    /* macro above doesnt include epilogue */
    size_so_far += HEADER_SIZE;

    /* unlink and unmap the page, by this point the page is entirely free */
    unlink_freeblock(block);
    unlink_freepage(placeholder);
    mem_unmap(placeholder, size_so_far);
}

/*
 * Pack metadata into a header.
 *
 * @block_size: the size of the entire block in bytes.
 * @alloc_flag: 0 if this block is free, 1 if it is allocated.
 */
void pack_header(header_t* header, size_t block_size, int alloc_flag)
{
    *header = (block_size) | (alloc_flag);
}

/*
 * =============================================================================
 * === Functions for Managing Pages ===
 * =============================================================================
 */

/*
 * Retrieve a pointer to the previous page in the pagelist.
 * 
 * @param current_page Pointer to the current page.
 * @return Pointer to the previous page, or NULL if none exists.
 */
void* get_previous_page(void* current_page) {
    return (*((void**)current_page));
}

/*
 * Retrieve a pointer to the next page in the pagelist.
 * 
 * @param current_page Pointer to the current page.
 * @return Pointer to the next page, or NULL if none exists.
 */
void* get_next_page(void* current_page) {
    return (*((void**)(current_page + PTR_SIZE)));
}

/*
 * Set the previous page pointer in the pagelist.
 * 
 * @param current_page Pointer to the current page.
 * @param previous_page Pointer to the previous page.
 */
void set_previous_page(void* current_page, void* previous_page) {
    *((void**)current_page) = previous_page;
}

/*
 * Set the next page pointer in the pagelist.
 * 
 * @param current_page Pointer to the current page.
 * @param next_page Pointer to the next page.
 */
void set_next_page(void* current_page, void* next_page) {
    *((void**)(current_page + PTR_SIZE)) = next_page;
}

/*
 * =============================================================================
 * === Functions for Block Size and Allocation Status ===
 * =============================================================================
 */

/*
 * Retrieve the size of a memory block, as stored in its header.
 * 
 * @param block_header Pointer to the block's header.
 * @return Size of the block, including header and footer, in bytes.
 */
size_t get_memory_block_size(header_t* block_header) {
    return ((*block_header) & ~0x7);
}

/*
 * Check if a memory block is currently allocated.
 * 
 * @param block_header Pointer to the block's header.
 * @return 1 if the block is allocated, 0 otherwise.
 */
int is_memory_block_allocated(header_t* block_header) {
    return ((*block_header) & 0x1);
}

/*
 * =============================================================================
 * === Functions for Payload, Headers, and Footers ===
 * =============================================================================
 */

/*
 * Convert a payload pointer to the corresponding header pointer.
 * 
 * @param payload Pointer to the payload of the block.
 * @return Pointer to the block's header.
 */
header_t* get_block_header_from_payload(void* payload) {
    return (header_t*)(payload - HEADER_SIZE);
}

/*
 * Convert a header pointer to the corresponding footer pointer.
 * 
 * @param block_header Pointer to the block's header.
 * @return Pointer to the block's footer.
 */
header_t* get_block_footer_from_header(header_t* block_header) {
    size_t block_size = get_memory_block_size(block_header);
    return (header_t*)((void*)block_header + block_size - HEADER_SIZE);
}

/*
 * =============================================================================
 * === Functions for Managing Free Lists ===
 * =============================================================================
 */

/*
 * Retrieve the pointer to the next free block in the freelist.
 * 
 * @param current_free_block Pointer to the header of the current free block.
 * @return Pointer to the next free block in the freelist, or NULL if none exists.
 */
header_t* get_next_free_block(header_t* current_free_block) {
    return *((void**)((void*)current_free_block + PTR_SIZE));
}

/*
 * Retrieve the pointer to the previous free block in the freelist.
 * 
 * @param current_free_block Pointer to the header of the current free block.
 * @return Pointer to the previous free block in the freelist, or NULL if none exists.
 */
header_t* get_previous_free_block(header_t* current_free_block) {
    return *((void**)((void*)current_free_block + (2 * PTR_SIZE)));
}

/*
 * Set the pointer to the next free block in the freelist.
 * 
 * @param current_free_block Pointer to the current free block's header.
 * @param next_free_block Pointer to the next free block's header.
 */
void set_next_free_block(header_t* current_free_block, header_t* next_free_block) {
    *((header_t**)((void*)current_free_block + PTR_SIZE)) = next_free_block;
}

/*
 * Set the pointer to the previous free block in the freelist.
 * 
 * @param current_free_block Pointer to the current free block's header.
 * @param previous_free_block Pointer to the previous free block's header.
 */
void set_previous_free_block(header_t* current_free_block, header_t* previous_free_block) {
    *((header_t**)((void*)current_free_block + (2 * PTR_SIZE))) = previous_free_block;
}

/*
 * =============================================================================
 * === Functions for Navigating Blocks ===
 * =============================================================================
 */

/*
 * Retrieve the pointer to the header of the next memory block.
 * 
 * @param current_block Pointer to the current block's header.
 * @return Pointer to the header of the next block.
 */
header_t* get_next_memory_block(header_t* current_block) {
    return (header_t*)((void*)current_block + get_memory_block_size(current_block));
}

/*
 * Retrieve the pointer to the header of the previous memory block.
 * 
 * @param current_block Pointer to the current block's header.
 * @return Pointer to the header of the previous block.
 */
header_t* get_previous_memory_block(header_t* current_block) {
    size_t prev_blocksize = get_memory_block_size((header_t*)((void*)current_block - HEADER_SIZE));
    return (header_t*)((void*)current_block - prev_blocksize);
}

