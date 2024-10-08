#include "cache.h"
#include <stdlib.h>
#include <string.h> // added
#include <stdio.h> // added
#include <math.h> // added

// Cache statistics counters.
uint32_t cache_total_accesses;
uint32_t cache_hits;
uint32_t cache_misses;
uint32_t cache_read_accesses;
uint32_t cache_read_hits;
uint32_t cache_write_accesses;
uint32_t cache_write_hits;

// Input parameters to control the cache.
uint32_t cache_size;
uint32_t cache_associativity;
uint32_t cache_block_size;

block_t **cache;


/*
 * Perform a read from the memory for a particular address.
 * Since this is a cache-simulation, memory is not involved.
 * No need to implement the logic for this function.
 * Call this function when cache needs to read from memory.
 */
int read_from_memory(uint32_t pa)
{
    return 0;
}

/*
 * Perform a write from the cache to memory for a particular address.
 * Since this is a cache-simulation, memory is not involved.
 * No need to implement the logic for this function.
 * Call this function when cache needs to write to memory.
 */
int write_to_memory(uint32_t pa)
{
    return 0;
}

/*
 *********************************************************
  Please edit the below functions for Part 1.
  You are allowed to add more functions that may help you
  with your implementation. However, please do not add
  them in any file. All your new code should be in cache.c
  file below this line.
 *********************************************************
*/

/*
 * Initialize the cache depending on the input parameters S, A, and B
 * and the statistics counter. The cache is declared in as extern in
 * include/cache.h file.
 */
void initialize_cache() //2
{
    //set variables to 0
    cache_total_accesses = 0;
    cache_hits = 0;
    cache_misses = 0;
    cache_read_accesses = 0;
    cache_read_hits = 0;
    cache_write_accesses = 0;
    cache_write_hits = 0;
       
    if (cache_associativity == 1) {
        int num_sets = cache_size/cache_block_size;
        cache = (block_t **)calloc(num_sets,sizeof(block_t *));
        if (cache == NULL) {
            printf("Unable to allocate memory");
        }
        //Initialize Cache Lines
        for(int i = 0; i < num_sets; i++) {
            cache[i] = (block_t *)calloc(cache_block_size, sizeof(block_t));
        }
    }

    if (cache_associativity == 2) {
        int num_sets = cache_size/cache_block_size;
        cache = (block_t **)calloc(num_sets,sizeof(block_t *));
        for(int i =0; i<num_sets; i++) {
        }
        if (cache == NULL) {
            printf("Unable to allocate memory");
        }

        for(int i = 0; i < num_sets; i++) {
            cache[i] = (block_t *)calloc(cache_block_size, sizeof(block_t));
        }
    }
    if (cache_associativity == 3) {
        int num_sets = cache_size / (cache_block_size);
        cache = (block_t **)calloc(num_sets,sizeof(block_t *));
        if (cache == NULL) {
            printf("Unable to allocate memory");
        }
        for(int i = 0; i < num_sets; i++){
            cache[i] = (block_t *)calloc(2, sizeof(cache_block_size));
        }
    }
    if (cache_associativity == 4) {
        int num_sets = cache_size / (cache_block_size);
        cache = (block_t **)calloc(num_sets,sizeof(block_t *));
        if (cache == NULL) {
            printf("Unable to allocate memory");
        }
        for(int i = 0; i < num_sets; i++){
            cache[i] = (block_t *)calloc(4, sizeof(cache_block_size));
        }
    }
}

/*
 * Free the allocated memory for the cache to avoid memory leaks.
 */
void free_cache()
{
    if (cache_associativity == 1) {
        int num_sets = cache_size/cache_block_size;
        for(int i = 0; i < num_sets; i++) {
            free(cache[i]);
        }
        free(cache);
    }
    if (cache_associativity == 2) {
        int num_sets = cache_size/cache_block_size;
        for(int i = 0; i < num_sets; i++) {
            free(cache[i]);
        }
        free(cache);
    }
    if (cache_associativity == 3) {
        int num_sets = cache_size/(cache_block_size * 2);
        for(int i = 0; i < num_sets; i++) {
            free(cache[i]);
        }
        free(cache);
    }
    if (cache_associativity == 4) {
        int num_sets = cache_size/(cache_block_size * 4);
        for(int i = 0; i < num_sets; i++) {
            free(cache[i]);
        }
        free(cache);
    }
}

// Print cache statistics.
void print_cache_statistics()
{
	printf("\n* Cache Statistics *\n");
    printf("total accesses: %d\n", cache_total_accesses);
    printf("hits: %d\n", cache_hits);
    printf("misses: %d\n", cache_misses);
    printf("total reads: %d\n", cache_read_accesses);
    printf("read hits: %d\n", cache_read_hits);
    printf("total writes: %d\n", cache_write_accesses);
    printf("write hits: %d\n", cache_write_hits);
}

/*
 * Perform a read from the cache for a particular address.
 * Since this is a simulation, there is no data. Ignore the data part.
 * The return value is always a HIT or a MISS and never an ERROR.
 */

op_result_t read_from_cache(uint32_t pa) // 3
{
    cache_total_accesses++;
    cache_read_accesses++;

    if(cache_associativity == 1) {

        int num_sets = cache_size/cache_block_size;

        int offset_bits = log2(cache_block_size);
        int index_bits = round(log2(num_sets));
        int tag_bits = 32 - offset_bits - index_bits;

        //Use Bit-Masking to extract bits from Hex pa
        //uint32_t offset_mask = (1 << offset_bits) - 1;
        uint32_t index_mask = ((1 << index_bits) - 1) << offset_bits;
        uint32_t tag_mask = ((1 << tag_bits) - 1) << (index_bits + offset_bits);

        //uint32_t offset = pa & offset_mask;
        uint32_t index = (pa & index_mask) >> offset_bits;
        uint32_t tag = (pa & tag_mask) >> (index_bits + offset_bits);
       
        if(cache[index]->valid && cache[index]->tag == tag){
            cache_hits++;
            cache_read_hits++;
            return HIT;
        }
        else {
            cache_misses++;
            read_from_memory(pa);
            cache[index]->valid = 1;
            cache[index]->tag = tag;
            return MISS;
        }
   
    } else if (cache_associativity == 2) {
        int num_sets = cache_size/cache_block_size;

        int offset_bits = log2(cache_block_size);
        int tag_bits = 32 - offset_bits;

        uint32_t tag_mask = ((1 << tag_bits)-1) << offset_bits;

        uint32_t tag = (pa & tag_mask) >> offset_bits;

        for(int i = 0; i < num_sets; i++) {
            if(cache[i]->valid && cache[i]->tag == tag) {
                cache_hits++;
                cache_read_hits++;
                cache[i]->lrucounter = 0;
                return HIT;
            }
        }
        cache_misses++;

        int lruindex = 0;
        for(int i = 1; i < num_sets; i++) {
            if(cache[i]->lrucounter > cache[lruindex]->lrucounter) {
                lruindex = i;
            }
        }
        cache[lruindex]->valid = 1;
        cache[lruindex]->tag = tag;
        cache[lruindex]->lrucounter = 0;

        for(int i = 0; i < num_sets; i++) {
            if(i != lruindex) {
                cache[i]->lrucounter++;
            }
        }

        return MISS;

    } else if (cache_associativity == 3) {
        int num_sets = cache_size/(cache_block_size*2);

        int offset_bits = log2(cache_block_size);
        int index_bits = round(log2(num_sets));
        int tag_bits = 32 - offset_bits - index_bits;

        uint32_t index_mask = ((1 << index_bits) - 1) << offset_bits;
        uint32_t tag_mask = ((1 << tag_bits) - 1) << (index_bits + offset_bits);

        uint32_t index = (pa & index_mask) >> offset_bits;
        uint32_t tag = (pa & tag_mask) >> (index_bits + offset_bits);

        for(int i = 0; i < 2; i++) {
            int j = index*2 + i;
            if(cache[j]->valid && cache[j]->tag == tag) {
                cache_hits++;
                cache_read_hits++;
                return HIT;
            }
        }
        cache_misses++;

        int lruindex = index*2;
        for(int i = index*2+1; i < index*2 + 2; i++) {
            if(cache[i]->lrucounter > cache[lruindex]->lrucounter) {
                lruindex = i;
            }
        }
        cache[lruindex]->valid = 1;
        cache[lruindex]->tag = tag;
        cache[lruindex]->lrucounter = 0;

        for(int i = index*2; i < index*2+2; i++) {
            if (i != lruindex) {
                cache[i]->lrucounter++;
            }
        }
        return MISS;

    } else if (cache_associativity == 4) {
        int num_sets = cache_size/(cache_block_size*4);

        int offset_bits = log2(cache_block_size);
        int index_bits = round(log2(num_sets));
        int tag_bits = 32 - offset_bits - index_bits;

        uint32_t index_mask = ((1 << index_bits) - 1) << offset_bits;
        uint32_t tag_mask = ((1 << tag_bits) - 1) << (index_bits + offset_bits);

        uint32_t index = (pa & index_mask) >> offset_bits;
        uint32_t tag = (pa & tag_mask) >> (index_bits + offset_bits);

        for(int i = 0; i < 4; i++) {
            int j = index*4 + i;
            if(cache[j]->valid && cache[j]->tag == tag) {
                cache_hits++;
                cache_read_hits++;
                return HIT;
            }
        }
        cache_misses++;

        int lruindex = index*4;
        for(int i = index*4 + 1; i < index*4 + 4; i++) {
            if(cache[i]->lrucounter > cache[lruindex]->lrucounter) {
                lruindex = i;
            }
        }
        cache[lruindex]->valid = 1;
        cache[lruindex]->tag = tag;
        cache[lruindex]->lrucounter = 0;

        for(int i = index*4; i < index*4+4; i++) {
            if (i != lruindex) {
                cache[i]->lrucounter++;
            }
        }
        return MISS;
    }

    return ERROR;  
}


/*
 * Perform a write from the cache for a particular address.
 * Since this is a simulation, there is no data. Ignore the data part.
 * The return value is always a HIT or a MISS and never an ERROR.
 */
op_result_t write_to_cache(uint32_t pa) // 4
{
    cache_total_accesses++;
    cache_write_accesses++;
   
    if(cache_associativity==1){

        //basically the same as before

        int num_sets = cache_size/cache_block_size;

        int offset_bits = log2(cache_block_size);
        int index_bits = round(log2(num_sets));
        int tag_bits = 32 - offset_bits - index_bits;

        //Use Bit-Masking to extract bits from Hex pa
        //uint32_t offset_mask = (1 << offset_bits) - 1;
        uint32_t index_mask = ((1 << index_bits) - 1) << offset_bits;
        uint32_t tag_mask = ((1 << tag_bits) - 1) << (index_bits + offset_bits);

        //uint32_t offset = pa & offset_mask;
        uint32_t index = (pa & index_mask) >> offset_bits;
        uint32_t tag = (pa & tag_mask) >> (index_bits + offset_bits);

        if(cache[index]->valid && cache[index]->tag == tag){
            cache_hits++;
            cache_write_hits++;
            return HIT;

        } else {    
            //increase the appropriate stats
            cache_misses++;
            //cache_write_accesses++;
            //simulate a read from memory
            write_to_memory(pa);
            //mark as valid and dirty then write the tag to this
            cache[index]->valid = 1;
            cache[index]->dirty = 1;
            cache[index]->tag = tag;

            return MISS;
        }


    } else if (cache_associativity == 2) {
        int num_sets = cache_size/cache_block_size;

        int offset_bits = log2(cache_block_size);
        int tag_bits = 32 - offset_bits;

        uint32_t tag_mask = ((1 << tag_bits) - 1) << offset_bits;
        uint32_t tag = (pa & tag_mask) >> offset_bits;

        for (int i = 0; i < num_sets; i++) {
            if(cache[i]->valid && cache[i]->tag == tag) {
                cache_hits++;
                cache_write_hits++;
                return HIT;
            }
        }
        cache_misses++;
        int lruindex = 0;
        for(int i = 1; i < num_sets; i++){
            if(cache[i]->lrucounter > cache[lruindex]->lrucounter) {
                lruindex = i;
            }
        }
        cache[lruindex]->valid = 1;
        cache[lruindex]->tag = tag;
        cache[lruindex]->dirty = 1;
        cache[lruindex]->lrucounter = 0;

        for(int i = 0; i < num_sets; i++) {
            if (i != lruindex) {
                cache[i]->lrucounter++;
            }
        }
        return MISS;

    } else if (cache_associativity == 3) {
        int num_sets = cache_size/(cache_block_size*2);

        int offset_bits = log2(cache_block_size);
        int index_bits = round(log2(num_sets));
        int tag_bits = 32 - offset_bits - index_bits;

        uint32_t index_mask = ((1 << index_bits) - 1) << offset_bits;
        uint32_t tag_mask = ((1 << tag_bits) - 1) << (index_bits + offset_bits);

        uint32_t index = (pa & index_mask) >> offset_bits;
        uint32_t tag = (pa & tag_mask) >> (index_bits + offset_bits);

        for(int i = 0; i < 2; i++) {
            int j = index*2 + i;
            if(cache[j]->valid && cache[j]->tag == tag) {
                cache_hits++;
                cache_write_hits++;
                return HIT;
            }
        }
        cache_misses++;

        int lruindex = index*2;
        for(int i = index*2 + 1; i < index*2 + 2; i++) {
            if(cache[i]->lrucounter > cache[lruindex]->lrucounter) {
                lruindex = i;
            }
        }
        cache[lruindex]->valid = 1;
        cache[lruindex]->tag = tag;
        cache[lruindex]->dirty = 1;
        cache[lruindex]->lrucounter = 0;

        for(int i = index*2; i < index*2+2; i++) {
            if (i != lruindex) {
                cache[i]->lrucounter++;
            }
        }
        return MISS;

    } else if (cache_associativity == 4) {
        int num_sets = cache_size/(cache_block_size*4);

        int offset_bits = log2(cache_block_size);
        int index_bits = round(log2(num_sets));
        int tag_bits = 32 - offset_bits - index_bits;

        uint32_t index_mask = ((1 << index_bits) - 1) << offset_bits;
        uint32_t tag_mask = ((1 << tag_bits) - 1) << (index_bits + offset_bits);

        uint32_t index = (pa & index_mask) >> offset_bits;
        uint32_t tag = (pa & tag_mask) >> (index_bits + offset_bits);

        for(int i = 0; i < 4; i++) {
            int j = index*4 + i;
            if(cache[j]->valid && cache[j]->tag == tag) {
                cache_hits++;
                cache_write_hits++;
                return HIT;
            }
        }
        cache_misses++;

        int lruindex = index*4;
        for(int i = index*4 + 1; i < index*4 + 4; i++) {
            if(cache[i]->lrucounter > cache[lruindex]->lrucounter) {
                lruindex = i;
            }
        }
        cache[lruindex]->valid = 1;
        cache[lruindex]->tag = tag;
        cache[lruindex]->dirty = 1;
        cache[lruindex]->lrucounter = 0;

        for(int i = index*4; i < index*4+4; i++) {
            if (i != lruindex) {
                cache[i]->lrucounter++;
            }
        }
        return MISS;
    }

    return ERROR;
}


// Process the S parameter properly and initialize `cache_size`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_S(int opt, char *optarg) // 1
{
    int size = atoi(optarg);

    if(size <= 0){
        return -1;
    } else if (size % 2 != 0){
        return -1;
    } else {
        cache_size = size;
    }

    return 0;
}

// Process the A parameter properly and initialize `cache_associativity`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_A(int opt, char *optarg) // 1
{
    int associativity = atoi(optarg);

    if(associativity > 4){
        return -1;
    } else {
        cache_associativity = associativity;
    }
    return 0;
}

// Process the B parameter properly and initialize `cache_block_size`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_B(int opt, char *optarg) // 1
{
    int block_size = atoi(optarg);

    if(block_size % 4 == 0){
        cache_block_size = block_size;
    } else {
        return -1;
    }
    return 0;
}

// When verbose is true, print the details of each operation -- MISS or HIT.
void handle_cache_verbose(memory_access_entry_t entry, op_result_t ret)
{
    if (ret == HIT) {
        printf("0x%08X CACHE-HIT\n", entry.address); // print the code plus HIT or MISS (0x0000008 - HIT)
    }
    if (ret == MISS) {
        printf("0x%08X CACHE-MISS\n", entry.address);
    }
}

// Check if all the necessary paramaters for the cache are provided and valid.
// Return 0 when everything is good. Otherwise return -1.
int check_cache_parameters_valid() // 1
{
	int four_w_a_sets = cache_size/(cache_block_size);
	int two_w__a_lines = cache_size/(cache_block_size);

    if(cache_size <= 0 || cache_associativity <= 0 || cache_block_size <= 0){
        return -1;
    } else if(cache_size % cache_block_size != 0){
        return -1;
    } else if(cache_associativity == 4 && four_w_a_sets % 1 != 0){ // checks if line number is a fraction for 4
		return -1;
	} else if(cache_associativity == 3 && two_w__a_lines % 1 != 0){ // same as above but for 3
		return -1;
	}
	 else if(cache_associativity == 4 && four_w_a_sets < 4){ // must be at least 4 sets for 4 way associativity
		return -1;
	} else if(cache_associativity == 3 && two_w__a_lines < 2){ // must be at least 2 sets for 2 way associativity
		return -1;
	}


    return 0;
}

