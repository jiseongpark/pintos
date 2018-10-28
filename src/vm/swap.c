#include "vm/swap.h"

unsigned swap_hash_hash_helper(const struct hash_elem * element, void * aux);
bool swap_hash_less_helper(const struct hash_elem *a, const struct hash_elem *b, void *aux);

void swap_init(void)
{
	sema_init(&swap_sema, 1);

	sema_down(&swap_sema);
	hash_init(&swap_table, swap_hash_hash_helper, swap_hash_less_helper, NULL);
	sema_up(&swap_sema);
}

void swap_set_ste(uint32_t *upage)
{
	sema_down(&swap_sema);
	STE* new_ste = (STE*)malloc(sizeof(STE));
	new_ste->uaddr = upage;

	hash_insert(&swap_table, &new_ste->helem);
	sema_up(&swap_sema);
}

void swap_remove_ste(uint32_t* upage)
{
	if(upage==NULL) return;
	sema_down(&swap_sema);
	STE* ste = swap_ste_lookup(upage);
	hash_delete(&swap_table, &ste->helem);
	free(ste);

	sema_up(&swap_sema);
}

STE* swap_ste_lookup(uint32_t *addr)
{
	STE ste;
	struct hash_elem *helem;

	ste.uaddr = addr;
	helem = hash_find(&swap_table, &ste.helem);

	return helem!=NULL ? hash_entry(helem, STE, helem) : NULL;
}

bool swap_in(uint32_t *uaddr)
{
	PTE *pte = page_pte_lookup(uaddr);
	uint8_t *paddr = NULL;

	ASSERT(pte->uaddr == uaddr);

	/* add to frame table */
	paddr = frame_get_fte(uaddr, PAL_USER | PAL_ZERO);
	if(paddr == NULL) return false;

	/* put data to physical memory */
	//use disk_read in devices/disk.c

	/* install page */
	if(pagedir_get_page(thread_current()->pagedir, uaddr) != NULL) return false;
	pagedir_set_page(thread_current()->pagedir, pte->uaddr, paddr, pte->writable);

	/* update mapping info in page table*/
	pte->paddr = paddr;

	/* erase info from swap table */
	swap_remove_ste(uaddr);
}

bool swap_out(uint32_t *uaddr)
{
	/* disk get */
	/* pte->is_swapped_out = true */
	/* store file info */
	/* put to swap table */
	/* remvoe from frame table */
}

unsigned swap_hash_hash_helper(const struct hash_elem * element, void * aux)
{
	STE *ste = hash_entry(element, STE, helem);
	return hash_bytes(&ste->uaddr, sizeof(uint32_t));
}

bool swap_hash_less_helper(const struct hash_elem *a, const struct hash_elem *b, void *aux)
{
	uint32_t* a_key = hash_entry(a, STE, helem) -> uaddr;
	uint32_t* b_key = hash_entry(b, STE, helem) -> uaddr;
	if(a_key < b_key) return true;
	else return false;
}
