#ifndef VM_FRAME_H
#define VM_FRAME_H

#include <stdbool.h>
#include <list.h>
#include <hash.h>
#include "threads/init.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "threads/palloc.h"
#include "userprog/pagedir.h"
#include "threads/vaddr.h"

typedef struct frame_table_entry
{
	uint32_t *uaddr;          /* virtual address of the page */
	uint32_t *paddr;          /* physical address of the page */

	struct thread *ut;        /* process that use this frame */
	bool is_swapped_out;      /* flag for swapped out */

	struct list_elem elem;    /* list element */
	struct hash_elem helem;   /* hash element */
} FTE;

struct hash frame_table;      /* frame table */

struct list fte_list;         /* list of all FTE */

struct semaphore frame_sema;  /* frame table semaphore */

void frame_init(void);
uint8_t* frame_get_fte(uint32_t *upage, enum palloc_flags flag);
bool frame_set_fte(uint32_t *upage, uint32_t *kpage);
FTE* frame_lru_fte(void);
void frame_remove_fte(uint32_t* upage);
FTE* frame_fte_lookup(uint32_t *addr);



#endif /* vm/frame.h */