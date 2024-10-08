#ifndef CACHE_H_
#define CACHE_H_

#include "common.h"

extern uint32_t cache_size;
extern uint32_t cache_associativity;
extern uint32_t cache_block_size;

// Cache statistics
extern uint32_t cache_total_accesses;
extern uint32_t cache_hits;
extern uint32_t cache_misses;
extern uint32_t cache_read_accesses;
extern uint32_t cache_read_hits;
extern uint32_t cache_write_accesses;
extern uint32_t cache_write_hits;

void initialize_cache();
void free_cache();
void print_cache_statistics();
int check_cache_parameters_valid();

op_result_t read_from_cache(uint32_t pa);
op_result_t write_to_cache(uint32_t pa);

int process_arg_S(int opt, char *optarg);
int process_arg_A(int opt, char *optarg);
int process_arg_B(int opt, char *optarg);
void handle_cache_verbose(memory_access_entry_t entry, op_result_t ret);

typedef struct {
	uint8_t valid;
	uint8_t dirty;
	uint32_t tag;
	int lrucounter;
} block_t;

extern block_t **cache;

#endif /* CACHE_H_ */
