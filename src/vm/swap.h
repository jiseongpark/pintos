#ifndef VM_FRAME_H
#define VM_FRAME_H

#include <stdbool.h>
#include <hash.h>
#include "threads/init.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "threads/palloc.h"
#include "userprog/pagedir.h"
#include "threads/vaddr.h"
#include "vm/frame.h"
#include "vm/page.h"

typedef struct swap_table_entry
{
	uint32_t *uaddr;              /* User virtual address of the swapped frame */

	struct hash_elem helem;       /* Hash element. */
} STE;

struct hash swap_table;           /* Swap table. */

struct semaphore swap_sema;       /* Swap table semaphore */

void swap_init(void);
void swap_set_ste(uint32_t *upage);
void swap_remove_ste(uint32_t* upage);
STE* swap_ste_lookup(uint32_t *addr);
bool swap_in(uint32_t *uaddr);
bool swap_out(uint32_t *uaddr);

#endif /* vm/swap.h */