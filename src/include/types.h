#ifndef TYPES_H_
#define TYPES_H_

// #include <stdio.h>  
#include <stdint.h>

typedef struct
{
    uint8_t present;
    uint8_t dirty;
    uint32_t PPN; // this will reflect the physical address of the page
    
} pt_entry_t;

typedef struct page_t
{   //this is a static address for this page, this will not change
    //this does not include the offset
    uint32_t ppn; 
    pt_entry_t *page_table_entry;
    char data[4096];
    struct page_t *next;
    //gonna add in another value here temporarily called time_stamp
    int time_stamp;
} page_t;

#endif /* TYPES_H_ */