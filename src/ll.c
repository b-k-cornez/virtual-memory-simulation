#include "ll.h"
#include "types.h"
#include <stdlib.h>

void insert_in_ll(page_t **head, page_t *n)
{
    n->next = *head;
    *head = n;
}

// page_t *find_in_ll(page_t **list, uint32_t index)
// {
//     page_t *n = *list;
//     while (n)
//     {
//         if (n->ppn == index)
//         {
//             return n;
//         }
//         n = n->next;
//     }
//     return -1; // -1 if not present
// }

page_t *delete_from_top_of_ll(page_t **head) {
    page_t *n = *head;
    *head = n->next;
    return n;
}


