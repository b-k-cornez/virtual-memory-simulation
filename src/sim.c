#include <stdio.h>  
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cache.h"
#include "tlb.h"
#include "pt.h"

// Initialize the system depending on the input parameters.
void initialize()
{
	initialize_cache();
	initialize_tlb();
	initialize_pt_system();
}

// Free the allocated memory for a graceful shutdown and to prevent memory leaks.
void free_memory()
{
	free_cache();

	free_tlb();

	// free the page table
	free_page_table();
}

// Print system-wide statistics.
void print_statistics()
{
	print_tlb_statistics();
	print_pt_statistics();
	print_cache_statistics();
}

// Print information when verbose is true.
void handle_verbose(memory_access_entry_t *entry, uint32_t pa)
{
	//print the virtual address and the physical address in hex to match the spec
	//print entry->verbose buffer
	if (entry->accesstype == READ){
		printf("R 0x%08x 0x%08x%s",entry->address, pa, entry->verbose);
	} else {
		printf("W 0x%08x 0x%08x%s",entry->address, pa, entry->verbose);
	}
	
}

// Check if all input parameters are provided and valid.
int check_parameters_valid()
{
	int cache_ret = check_cache_parameters_valid();
	int tlb_ret = check_tlb_parameters_valid();

	if (cache_ret == 0 && tlb_ret == 0) {
		return 0;
	} else {
		return -1;
	}
}

int main(int argc, char *argv[])
{
	int opt;
	trace_file = NULL;
	FILE *trace_fp;
	opterr = 0;
	uint32_t pa = 0;
	op_result_t ret;
	int r = 0;

	/* 
	 * This is just an example to show how to use getopt. You will need to do a
     * lot more to get your code correct. Try to think of different ways that
     * your inputs are not appropriate and accordingly add more code.
	 */
	while((opt = getopt(argc, argv, "S:A:B:t:vT:L:")) != -1) {
		switch(opt) {
			case 'S':
				r = process_arg_S(opt, optarg);
				if (r) {
					printf("Improper S parameter\n");
					return -1;
				}
				break;
			case 'A':
				r = process_arg_A(opt, optarg);
				if (r) {
					printf("Improper A parameter\n");
					return -1;
				}
				break;
			case 'B':
				r = process_arg_B(opt, optarg);
				if (r) {
					printf("Improper B parameter\n");
					return -1;
				}
				break;
			case 'T':
				r = process_arg_T(opt, optarg);
				if (r) {
					printf("Improper T parameter\n");
					return -1;
				}
				break;
			case 'L':
				r = process_arg_L(opt, optarg);
				if (r) {
					printf("Improper L parameter\n");
					return -1;
				}
				break;
			case 't':
				trace_file = optarg;
				break;
			case 'v':
				verbose = 1;
				break;
			case '?':
			default:
				printf("Invalid options provided. \n %s\n", usage_str);
				return -1;
		}
	}

	// Open the trace file.
	trace_fp = fopen(trace_file, "r");

	if (trace_file == NULL || strlen(trace_file) == 0) {
		printf("Trace file not provided.\n");
		return -1;
	}

	if (trace_fp == NULL) {
		printf("Trace file does not exists.\n");
		return -1;
	}

	// Check if all required parameters are provided
	ret = check_parameters_valid();
	if (ret) {
		printf("Invalid configuration\n");
       
		return -1;
	}

	// Initialize the system (including the cache).
	initialize();

	
	memory_access_entry_t *entry = (memory_access_entry_t*) malloc(sizeof(memory_access_entry_t));
	// Read the trace file, one line at a time.
	while (1) {
		ret = INVALID;
		process_trace_file_line(trace_fp, entry);

		// Break if end of file is reached.
		if (entry->address == -1) {
			break;
		}

		if(entry->address > 0x03FFFFFF){
			continue;
		}

		// only translate if valid
		pa = translate_address(entry);
		// acccess the cache
		// Based on the access type, either read from cache or write to cache.
		if (entry->accesstype == READ) {
			ret = read_from_cache(pa);
		} else if (entry->accesstype == WRITE) {
			ret = write_to_cache(pa);
		} else {
			printf("This message should not be printed. Fix your code.\n");
			return -1;
		}
		if (ret == MISS){
			strcat(entry->verbose, " CACHE-MISS\n");
		} else if (ret == HIT) {
			strcat(entry->verbose, " CACHE-HIT\n");
		} else {
			strcat(entry->verbose, " CACHE-INVALID\n");
		}
		// Handle verbose parameter.
		if (verbose) {
			handle_verbose(entry, pa);
		}
		entry->verbose[0] = '\0';
	}

	free(entry);
	fclose(trace_fp);



	// Print statistics at the end of the simulation.
	print_statistics();
	
	if (verbose) {
		print_tlb_entries();
		print_pt_entries();
	}

	// Free the allocated memory.
	free_memory();
	

    return 0;
} 
