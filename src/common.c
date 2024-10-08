#include "common.h"
#include "ll.h"
#include "pt.h"
#include "tlb.h"
#include <string.h>

char *usage_str = "Usage: ./sim -S <S> -A <A> -B <B> -t <trace_file> [-v]";

// Input parameters.
uint32_t verbose = 0;
char *trace_file = NULL;

//dummy parameters
uint32_t dummy_disk_block = 0;

// Read one entry at a time from the trace file.
memory_access_entry_t* process_trace_file_line(FILE *trace_fp, memory_access_entry_t *entry)
{
	char operation;
	uint32_t address = 0;
	int ret = 0;

	ret = fscanf(trace_fp, " %c %x\n", &operation, &address);
	if (ret != EOF) {	
        if (operation == 'R') {
			entry->address = address;
			entry->accesstype = READ;
        } else if (operation == 'W') {
			entry->address = address;
			entry->accesstype = WRITE;
        }
    } else {
		entry->address = -1;
		entry->accesstype = INVALID;
	}

	return entry;
}

uint32_t translate_address(memory_access_entry_t *entry){
	//implimate the spec and translate the address

	//check tlb first
	int tlb_ret = INVALID;
	int page_table_ret = INVALID;
	int PPN = check_tlb(entry->address);
	if(PPN != -1){
		//TLB HIT
		//Update LRU status for the TLB entry.
		tlb_ret = HIT;
	} else {
		//TLB MISS
		tlb_ret = MISS;
		//check page table entry
		PPN = check_page_table(entry->address);
		if (PPN != -1){
			page_table_ret = HIT;
			//page table hit
			//update tlb
			insert_or_update_tlb_entry(entry->address, PPN);
		} else {
			page_table_ret = MISS;
			// page fault
			// check from free page list
			page_t *free_page = get_free_page();
			if(free_page != NULL){
				//free page list is non-null
				dummy_read_page_from_disk(free_page->data, dummy_disk_block);
				update_page_table(entry->address, free_page->ppn);
				PPN = free_page->ppn;
			} else{
				// if free page list is null
				// evict 1 page from free page list
				page_t *victim_page = get_victim_page();
				if (victim_page->page_table_entry->dirty == 1){
					dummy_write_page_to_disk(victim_page->data);
					victim_page->page_table_entry->dirty = 0;
				}
				dummy_read_page_from_disk(victim_page->data, dummy_disk_block);
				update_page_table(entry->address, victim_page->ppn);
				PPN = victim_page->ppn;
			}
		}
	}

	
	if (tlb_ret == HIT){
		strcat(entry->verbose, " TLB-HIT");
		strcat(entry->verbose, " -");
	}
	else if (tlb_ret == INVALID){
		strcat(entry->verbose, " TLB-INVALID");
	}
	else if (tlb_ret == MISS){
		strcat(entry->verbose, " TLB-MISS");
		if (page_table_ret == HIT)
			strcat(entry->verbose, " PAGE-HIT");
		else if (page_table_ret == MISS)
			strcat(entry->verbose, " PAGE-FAULT");
		else
			strcat(entry->verbose, " PAGE-INVALID");
	}

	if (entry->accesstype == WRITE) {
		set_dirty_bits_in_tlb_and_pt(entry->address);
	}

	//return physical address by adding offset
    return (PPN << 12) | (entry->address & 0x00000FFF);
}

//dummy function to read page from disk
// No need to modify/implement this function
void dummy_read_page_from_disk(char *page_data, uint32_t disk_block)
{
	return;
}

// dummy function to write page to disk
// No need to modify/implement this function
void dummy_write_page_to_disk(char *page_data)
{
	return;
}

// Extract the VPN from the address and use it.
void set_dirty_bits_in_tlb_and_pt(uint32_t address){
	//set dirty bit in page table
	set_dirty_bit_in_page_table(address);
	//set dirty bit in tlb
	set_dirty_bit_in_tlb(address);
}
