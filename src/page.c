#include "page.h"
#include <stddef.h>
#include <stdint.h>

// Define constants
#define TOTAL_PAGES 128
#define PAGE_SIZE_2MB (2 * 1024 * 1024)

// Allocate pages
struct ppage physical_page_array[TOTAL_PAGES];

// Head of free pages list
static struct ppage *free_physical_pages = NULL;

// Initialize page frame allocator
void init_pfa_list(void) {
	free_physical_pages = NULL;

	for(int i = 0; i < TOTAL_PAGES; i++){
		physical_page_array[i].physical_addr = (void *)(i * PAGE_SIZE_2MB);
		physical_page_array[i].next = free_physical_pages;
		physical_page_array[i].prev = NULL;

		if (free_physical_pages) {
			free_physical_pages->prev = &physical_page_array[i];
		}
		free_physical_pages = &physical_page_array[i];
	}
}

// Allocate npages from free pages list 
struct ppage *allocate_physical_pages(unsigned int npages) {
	if (npages == 0 || free_physical_pages == NULL) return NULL;

	struct ppage *head = free_physical_pages;
	struct ppage *tail = head;
	unsigned int count = 1;

	while (count < npages && tail->next != NULL) {
		tail = tail->next;
		count++;
	}

	if (count < npages) {
		// Not enough pages
		return NULL;
	}

	// Remove allocated pages from free pages list
	free_physical_pages = tail->next;
	if (free_physical_pages) {
		free_physical_pages->prev = NULL;
	}
	head->prev = NULL;
	tail->next = NULL;
	
	return head;
}

//Return list of pages to the free pages list
void return_physical_pages(struct ppage *ppage_list) {
	if (!ppage_list) return;

	//Find tail
	struct ppage *tail = ppage_list;
	while (tail->next != NULL) {
		tail = tail->next;
	}

	//Put freed pages at front of free pages list
	tail->next = free_physical_pages;
	if (free_physical_pages) {
		free_physical_pages->prev = tail;
	}
	ppage_list->prev = NULL;
	free_physical_pages = ppage_list;
}

