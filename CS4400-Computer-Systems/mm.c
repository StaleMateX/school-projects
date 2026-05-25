/*
 * mm-naive.c - The least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by allocating a
 * new page as needed.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused.
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

/* === Iteration Macros === */
/* Iterate over the pagelist from head to tail. */
#define FOR_EACH_PAGE(page_cursor) \
    for (page_cursor = first_page; \
         page_cursor != NULL; \
         page_cursor = get_next_page(page_cursor))

/* Iterate over the blocks in a page, from prologue to epilogue (excluding). */
#define FOR_EACH_BLOCK_IN_PAGE(page, block_cursor) \
    for (block_cursor = page + PAGE_LINKS_SIZE; \
         get_memory_block_size((header_t*)block_cursor) != HEADER_SIZE; \
         block_cursor = ((void*)block_cursor) + get_memory_block_size((header_t*)block_cursor))

/* Iterate over the free blocks in the freelist, starting at freelist_head. */
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

/* Helper functions for Page Management */
void* create_new_page(size_t min_block_size);
void* allocate_block_in_pages(void* first_page, size_t block_size);
void* allocate_from_free_block(header_t* free_block, size_t block_size);
void* create_allocated_block(header_t* header, size_t block_size);
void* create_free_block(header_t* header, size_t block_size);
void add_block_to_freelist(header_t* header);
void remove_block_from_freelist(header_t* header);
void unlink_page_from_list(void* page);
void link_page_to_list(void* page);
void unmap_page_if_empty(header_t* block);
void write_block_metadata(header_t* header_ptr, size_t block_size, int alloc_flag);

/* Functions for Managing Pages */
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
    if (sizeof(header_t) > HEADER_SIZE) {
        fprintf(stderr, "ERROR: header_t size exceeds allowed header size.\n");
        return -1;
    }

    first_page = NULL;
    last_page = NULL;
    freelist_head = NULL;
    minimum_page_size = INIT_PAGE_SIZE;

    void* page = create_new_page(minimum_page_size - PAGE_OVERHEAD);
    if (!page) return -1;
    
    return 0;
}

/* 
 * mm_malloc - Allocate a block by using bytes from current_page,
 *     grabbing a new page if necessary.
 */
void* mm_malloc(size_t size)
{
    size_t block_size = MAX(MIN_BLOCK_SIZE, ALIGN(size) + BLOCK_OVERHEAD);
    void* block_ptr = allocate_block_in_pages(first_page, block_size);

    if (!block_ptr) {
        void* new_page = create_new_page(block_size);
        if (!new_page) {
            fprintf(stderr, "ERROR: Page allocation failed.\n");
            return NULL;
        }

        block_ptr = allocate_block_in_pages(new_page, block_size);
        if (!block_ptr) {
            fprintf(stderr, "ERROR: Block allocation failed after new page.\n");
            return NULL;
        }
    }
    return block_ptr;  
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void* payload_ptr)
{
    header_t* header = get_block_header_from_payload(payload_ptr);
    size_t block_size = get_memory_block_size(header);

    // Coalesce with adjacent free blocks
    header_t* next_block = get_next_memory_block(header);
    header_t* previous_block = get_previous_memory_block(header);

    if (!is_memory_block_allocated(next_block)) {
        remove_block_from_freelist(next_block);
        block_size += get_memory_block_size(next_block);
    }

    if (!is_memory_block_allocated(previous_block)) {
        remove_block_from_freelist(previous_block);
        header = previous_block;
        block_size += get_memory_block_size(previous_block);
    }

    // Update header/footer for coalesced block
    write_block_metadata(header, block_size, 0);
    write_block_metadata(get_block_footer_from_header(header), block_size, 0);

    // Add coalesced block to the free list
    add_block_to_freelist(header);

    // Unmap the page if it no longer contains allocated blocks
    unmap_page_if_empty(header);  
}

/*
 * =============================================================================
 * === Page Management ===
 * =============================================================================
 */

/**
 * Create and initialize a new page.
 * 
 * @param min_block_size Minimum size of the free block in the new page.
 * @return Pointer to the new page, or NULL if allocation fails.
 */
void* create_new_page(size_t min_block_size)
{
    size_t total_page_size = MAX(ALIGN(min_block_size) + PAGE_OVERHEAD, minimum_page_size);
    total_page_size = PAGE_ALIGN(total_page_size); // Ensure page size is aligned

    // Update minimum page size for future allocations
    minimum_page_size = MIN(2 * minimum_page_size, MAX_PAGE_SIZE);

    // Request memory for the new page
    void* new_page = mem_map(total_page_size);
    if (!new_page) return NULL;  // Allocation failed

    // Calculate free block size
    size_t free_block_size = total_page_size - PAGE_OVERHEAD;
    if (free_block_size < MIN_BLOCK_SIZE) {
        mem_unmap(new_page, total_page_size); // Clean up if initialization fails
        fprintf(stderr, "ERROR: Free block size too small.\n");
        return NULL;
    }

    // Link the page to the pagelist
    link_page_to_list(new_page);

    // Initialize page metadata
    void* placeholder = new_page + PAGE_LINKS_SIZE;

    // Prologue block
    write_block_metadata((header_t*)placeholder, 2 * HEADER_SIZE, 1); // Allocated prologue header
    placeholder += HEADER_SIZE;
    write_block_metadata((header_t*)placeholder, 2 * HEADER_SIZE, 1); // Allocated prologue footer
    placeholder += HEADER_SIZE;

    // Free block
    placeholder = create_free_block((header_t*)placeholder, free_block_size);

    // Epilogue block
    write_block_metadata((header_t*)placeholder, HEADER_SIZE, 1); // Allocated epilogue

    return new_page;  
}

/*
 * Attempt to allocate a block within the free blocks of the heap.
 * 
 * @param first_page Pointer to the first page in the pagelist.
 * @param block_size Size of the block to allocate (including header and footer).
 * @return Pointer to the allocated block's payload, or NULL if allocation fails.
 */
void* allocate_block_in_pages(void* first_page, size_t block_size) {
    void* block_cursor = NULL;
    FOR_EACH_BLOCK_IN_FREELIST(block_cursor) {
        if (block_cursor && get_memory_block_size(block_cursor) >= block_size) {
            return allocate_from_free_block(block_cursor, block_size);
        }
    }
    return NULL;
}

/*
 * Allocate a block from an existing free block.
 * 
 * @param free_block Pointer to the header of the free block.
 * @param block_size Size of the block to allocate (including header and footer).
 * @return Pointer to the allocated block's payload.
 */
void* allocate_from_free_block(header_t* free_block, size_t block_size)
{
    size_t free_block_size = get_memory_block_size(free_block);
    size_t remaining_size = free_block_size - block_size;

    // If remaining size is too small to form a valid block, use the entire block
    if (remaining_size < MIN_BLOCK_SIZE) {
        block_size = free_block_size;
        remaining_size = 0;
    }

    // Remove the free block from the free list
    remove_block_from_freelist(free_block);

    // Allocate the block
    void* payload_ptr = create_allocated_block(free_block, block_size);

    // Create a new free block from the remaining space, if applicable
    if (remaining_size > 0) {
        header_t* remaining_block = (void*)free_block + block_size;
        create_free_block(remaining_block, remaining_size);
    }

    return payload_ptr;
}

/*
 * Create an allocated block at a specified location.
 * 
 * @param header Pointer to the block's header.
 * @param block_size Size of the block (including header and footer).
 * @return Pointer to the block's payload.
 */
void* create_allocated_block(header_t* header, size_t block_size)
{
    // Write metadata for the header and footer of the block
    write_block_metadata(header, block_size, 1);
    write_block_metadata(((void*)header) + block_size - HEADER_SIZE, block_size, 1);
    // Return a pointer to the payload (after the header)
    return (((void*)header) + HEADER_SIZE);
}

/*
 * Create a free block at a specified location.
 * 
 * @param header Pointer to the block's header.
 * @param block_size Size of the block (including header and footer).
 * @return Pointer to the next location after the free block.
 */
void* create_free_block(header_t* header, size_t block_size)
{
    // Write metadata for the header and footer of the free block
    write_block_metadata(header, block_size, 0);
    write_block_metadata(get_block_footer_from_header(header), block_size, 0);

    // Add the block to the freelist
    add_block_to_freelist(header);

    // Return the next location after the block
    return (void*)header + block_size;
}

/*
 * Add a block to the free list.
 * 
 * @param header Pointer to the block's header.
 */
void add_block_to_freelist(header_t* header)
{
    if (!freelist_head) {
         // Freelist is empty; set the block as the head
        set_next_free_block(header, NULL);
        set_previous_free_block(header, NULL);
    } else {
        // Link the new block at the beginning of the freelist
        set_previous_free_block(freelist_head, header);
        set_next_free_block(header, freelist_head);
        set_previous_free_block(header, NULL);
    }
    freelist_head = header;
}

/*
 * Remove a block from the free list.
 * 
 * @param header Pointer to the block's header.
 */
void remove_block_from_freelist(header_t* header)
{
    header_t* prev = get_previous_free_block(header);
    header_t* next = get_next_free_block(header);

    // Update the links of neighboring blocks
    if (prev) {
        set_next_free_block(prev, next);
    }
    if (next) {
        set_previous_free_block(next, prev);
    }


    // Update the freelist head if necessary
    if (header == freelist_head) {
        freelist_head = next;
    }
}

/*
 * Add a page to the pagelist.
 * 
 * @param page Pointer to the page being added.
 */
void link_page_to_list(void* page)
{
    /* append the page the pagelist */
    if (last_page == NULL)
    {
        // Initialize the pagelist with the first page
        set_previous_page(page, NULL);
        set_next_page(page, NULL);
        first_page = page;
        last_page = page;
    }
    else
    {
        // Append the new page to the end of the pagelist
        set_next_page(last_page, page);
        set_previous_page(page, last_page);
        set_next_page(page, NULL);
        last_page = page;
    }
}

/*
 * Remove a page from the pagelist.
 * 
 * @param page Pointer to the page being removed.
 */
void unlink_page_from_list(void* page)
{
    void* prev = get_previous_page(page);
    void* next = get_next_page(page);

    // Update the links of neighboring pages
    if (prev) {
        set_next_page(prev, next);
    }
    if (next) {
        set_previous_page(next, prev);
    }

    // Update the pagelist head and tail if necessary
    if (page == first_page) {
        first_page = next;
    }
    if (page == last_page) {
        last_page = prev;
    }
}

/*
 * Unmap a page if all its blocks are free.
 * 
 * @param block Pointer to the header of a block within the page.
 */
void unmap_page_if_empty(header_t* block) {
  void* page_start = (void*)block;
    size_t page_size = PAGE_LINKS_SIZE; // Start with page links size
    header_t* block_cursor;

    // Iterate backward to locate the prologue
    while (get_memory_block_size(page_start) != (2 * HEADER_SIZE)) {
        if (is_memory_block_allocated(page_start)) return; // Early exit if allocated block found
        page_start = get_previous_memory_block(page_start);
    }

    // Move to the actual start of the page (before the prologue)
    page_start = (void*)page_start - PAGE_LINKS_SIZE;

    // Iterate over all blocks in the page and calculate the page size
    FOR_EACH_BLOCK_IN_PAGE(page_start, block_cursor) {
        size_t block_size = get_memory_block_size(block_cursor);
        page_size += block_size;

        // Skip prologue and epilogue; check if any allocated block exists
        if (block_size != HEADER_SIZE && block_size != (2 * HEADER_SIZE) && is_memory_block_allocated(block_cursor)) {
            return; // Early exit if any allocated block found
        }
    }

    page_size += HEADER_SIZE; // Add epilogue size

    // By this point, the page is fully free
    remove_block_from_freelist(block);
    unlink_page_from_list(page_start);
    mem_unmap(page_start, page_size);
}

/*
 * Write metadata (size and allocation status) into a block header.
 * 
 * @param block_header Pointer to the block's header.
 * @param block_size Size of the block in bytes.
 * @param allocation_flag 0 if the block is free, 1 if it is allocated.
 */
void write_block_metadata(header_t* block_header, size_t block_size, int allocation_flag)
{
    *block_header = (block_size) | (allocation_flag);
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