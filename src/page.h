#ifndef PAGE_H
#define PAGE_H

#include <stdint.h>

struct ppage {
	struct ppage *next;
	struct ppage *prev;
	void *physical_addr;

};

//initialize page frame allocator
void init_pfa_list(void);

//allocate pages from free pages list
struct ppage *allocate_physical_pages(unsigned int npages);

//return list of pages to free pages list
void return_physical_pages(struct ppage *ppage_list);

#endif
