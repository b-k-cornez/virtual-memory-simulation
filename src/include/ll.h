#ifndef LL_H_
#define LL_H_

#include <stdio.h>
#include <stdint.h>
#include "types.h"

// typedef struct page_t
// {   //this is a static address for this page, this will not change
//     //this does not include the offset
//     uint32_t ppn; 
//     pt_entry_t *page_table_entry;
//     char data[4096];
//     struct page_t *next;
// } page_t;

void insert_in_ll(page_t **list, page_t *n);
// page_t *find_in_ll(page_t **list, uint32_t data);
page_t *delete_from_top_of_ll(page_t **list);

#endif /* LL_H_ */
