#ifndef TLB_H_
#define TLB_H_

#include <stdio.h>  
#include <stdint.h>


typedef struct 
{
    uint8_t valid;
    uint8_t dirty;
    uint32_t VPN;
    uint32_t PPN;
    int lrucounter;
} tlb_entry_t;

int check_tlb_parameters_valid();
void initialize_tlb();
void free_tlb();
int process_arg_T(int opt, char *optarg);
int process_arg_L(int opt, char *optarg);
int check_tlb(uint32_t address);

extern uint32_t tlb_entries;
extern uint32_t tlb_associativity;

extern tlb_entry_t** tlb;

void insert_or_update_tlb_entry(uint32_t address, uint32_t PPN);
void set_dirty_bit_in_tlb(uint32_t address);
void print_tlb_statistics();
void print_tlb_entries();

#endif /* TLB_H_ */