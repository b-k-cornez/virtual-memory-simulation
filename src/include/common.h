#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>  
#include <stdint.h>

extern char *usage_str;

extern uint32_t verbose;
extern char *trace_file;

typedef enum {READ, WRITE, INVALID} access_t;

typedef enum {MISS, HIT, ERROR, PG_FAULT} op_result_t;

typedef struct {
    uint32_t address;
    access_t accesstype;
    char verbose[4096];
} memory_access_entry_t;

memory_access_entry_t* process_trace_file_line(FILE *trace_fp, memory_access_entry_t *entry);

uint32_t translate_address(memory_access_entry_t *entry);

void set_dirty_bits_in_tlb_and_pt(uint32_t address);
void dummy_write_page_to_disk(char *page_data);
void dummy_read_page_from_disk(char *page_data, uint32_t disk_block);

#endif /* COMMON_H_ */
