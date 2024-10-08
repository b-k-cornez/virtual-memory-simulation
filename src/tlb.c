#include "tlb.h"
#include <stdlib.h>

// Input parameters to control the tlb.
uint32_t tlb_entries;
uint32_t tlb_associativity;

// TLB statistics counters.
uint32_t tlb_total_accesses;
uint32_t tlb_hits;
uint32_t tlb_misses;

tlb_entry_t** tlb;

// Check if all the necessary paramaters for the tlb are provided and valid.
// Return 0 when everything is good. Otherwise return -1.


typedef struct ll {

    uint32_t vpn;
    int index_num;
    struct ll *next;
    int marker;

} ll;


ll *lru_ll;
int counter;
int num;


ll *lru_ll_2;
int boris;


ll *make_node(uint32_t vpn, int index_number){
    ll *new_node = (ll*)malloc(sizeof(ll));
    if(new_node != NULL){
        new_node->vpn = vpn;
        new_node->index_num = index_number;
        new_node->next = NULL; 
    }

    return new_node;
}

int pop_from_tail(ll **list_head){

    if(list_head == NULL){
        return -1;
    }

    ll *current = *list_head;
    ll *prev = NULL;


    while(current->next != NULL){
        prev = current;
        current = current->next;
    }

    if(prev == NULL){
        *list_head = NULL;
    }else {
        prev->next = NULL;
    }

    int in = current->index_num;
    free(current);
    return in;
}

int pop_from_ll(ll **list, uint32_t vpn){

    ll *current = *list;
    ll *prev = NULL;

    while(current != NULL && current->vpn != vpn){
        prev = current;
        current = current->next;
    }

    if(current != NULL){
        if(prev == NULL){
            *list = current->next;
        } else {
            prev->next = current->next;
        }
    }

    int in = current->index_num;
    free(current);

    return in;
}

void add_to_head(ll **list, ll *node){
    ll *new_node = node;
    if(new_node != NULL){
        new_node->next = *list;
        *list = new_node;
    }
}

int search_ll(ll *list, uint32_t vpn){

    ll *current_node = list;

    while(current_node != NULL){
        if(current_node->vpn == vpn){
            return 1;
        }
        current_node = current_node->next;
    }
    
    return 0;
}

int search_by_index(ll *list, int index){
    ll *current_node = list;

    while(current_node != NULL){
        if(current_node->index_num == index){
            return current_node->marker;
        }
        current_node = current_node->next;
    }
    return 0;
}


int check_tlb_parameters_valid()
{
    if (tlb_associativity > 4 || tlb_associativity < 1) {
        return -1;
    }
    else if (tlb_entries < 2) {
        return -1;
    }
    return 0;
}

/*
 * Initialize the "tlb" depending on the input parameters T and L.
 * The "tlb" is declared in as extern in include/tlb.h file.
 */
void initialize_tlb()
{
    tlb_hits = 0;
    tlb_misses = 0;
    tlb_total_accesses = 0;

    counter = 0;
    num = 0;

    lru_ll = NULL;
    lru_ll_2 = NULL;

    if (tlb_associativity == 1) {
        tlb = (tlb_entry_t **)calloc(tlb_entries,sizeof(tlb_entry_t *));
        for(int i =0; i < tlb_entries; i++) {
            tlb[i] = (tlb_entry_t *)calloc(1,sizeof(tlb_entry_t));
            if(tlb[i] == NULL) {
                printf("Unable to allocate memory");
            }
        }  
    } 
    else if (tlb_associativity == 2) {
        tlb = (tlb_entry_t **)calloc(tlb_entries,sizeof(tlb_entry_t *));
        for(int i = 0; i<tlb_entries; i++){
            tlb[i] = (tlb_entry_t *)calloc(tlb_entries,sizeof(tlb_entry_t));
        }
    }

//     else if (tlb_associativity == 3) {
//     int num_sets = tlb_entries / 2;
//     tlb = (tlb_entry_t **)calloc(num_sets, sizeof(tlb_entry_t *));
//     for (int i = 0; i < num_sets; i++) {
//         tlb[i] = (tlb_entry_t *)calloc(2, sizeof(tlb_entry_t));
//         if(tlb[i] == NULL) {
//             printf("unable to allocate memory");
//         }
//     }
// }
//     else if (tlb_associativity == 4) {
//         for(int i = 0; i < tlb_entries;i++) {
//             tlb[i] = (tlb_entry_t *)calloc(4*tlb_entries,sizeof(tlb_entry_t));
//         }
//     }
//  return;
}

void free_tlb() {
    if (tlb_associativity == 1) {
        for(int i = 0; i < tlb_entries; i++) {
            free(tlb[i]);
        }
        free(tlb);
    }
    if (tlb_associativity == 2) {
        for(int i = 0; i < tlb_entries; i++) {
            free(tlb[i]);
        }
        free(tlb);

        ll *node = lru_ll;
        ll *next;

        while(node != NULL){
            next = node->next;
            free(node);
            node = next;
        }

    }
    if (tlb_associativity == 3) {
        int num_sets = tlb_entries/2;
        for(int i = 0; i < num_sets; i++) {
            free(tlb[i]);
        }
        free(tlb);

        
        ll *node = lru_ll;
        ll *node2 = lru_ll_2;
        ll *next;

        while(node != NULL){
            next = node->next;
            free(node);
            node = next;
        }

        while(node2 != NULL){
            next = node2->next;
            free(node2);
            node2 = next;
        }

    }
    if (tlb_associativity == 4) {
        for(int i = 0; i < tlb_entries; i++) {
            free(tlb[i]);
        }
        free(tlb);
    }
}

// Process the T parameter properly and initialize `tlb_entries`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_T(int opt, char *optarg)
{
    int num = atoi(optarg);

    if(num < 2) {
        return -1;
    } else if (num % 2 != 0){
        return -1;    
    } else {
        tlb_entries = num;
    }
    return 0;
 }

// Process the A parameter properly and initialize `tlb_associativity`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_L(int opt, char *optarg)
{
    int tlbassociativity = atoi(optarg);

    if(tlbassociativity > 4){
        return -1;
    } else {
        tlb_associativity = tlbassociativity;
    }
    return 0;
}

// Check if the tlb hit or miss.
// Extract the VPN from the address and use it.
// Keep associativity in mind while searching.
int check_tlb(uint32_t address){
    //return -1 if the entry is missing or valid bit is 0 aka tlb miss
    //return PPN if the entry is valid and TAG matches aka tlb hit
    tlb_total_accesses++;
    if (tlb_associativity == 1) {
        uint32_t vpn = address >> 12;
        int tlb_index = vpn % tlb_entries;

        if(tlb[tlb_index]->valid == 1 && tlb[tlb_index]->VPN == vpn) {
            tlb_hits++;
            return tlb[tlb_index]->PPN;
        }
        tlb_misses++;
        return -1;  
    }
    if (tlb_associativity == 2) {
        uint32_t vpn = address >> 12;
        for(int i = 0; i < tlb_entries; i++) {
            if(tlb[i]->valid == 1 && tlb[i]->VPN == vpn) {
                tlb_hits++;

                if(search_ll(lru_ll, vpn)){

                    int i = pop_from_ll(&lru_ll, vpn);
                    ll *new_node = make_node(vpn, i);
                    add_to_head(&lru_ll, new_node);

                }
                return tlb[i]->PPN;
            }
        }
        tlb_misses++;
        return -1;
    }
    // if (tlb_associativity == 3) {

    //     uint32_t vpn = address >> 12;
    //     int tlb_index = (vpn % tlb_entries*2)*2;

    //     if(tlb[tlb_index]->lrucounter == 1){
    //         if(tlb[tlb_index][0].valid == 1 && tlb[tlb_index][0].VPN == vpn) {
    //         tlb_hits++;
    //         return tlb[tlb_index][0].PPN;

    //     } else if(tlb[tlb_index][1].valid == 1 && tlb[tlb_index][1].VPN == vpn) {
    //         tlb_hits++;
    //         return tlb[tlb_index][1].PPN;
    //     }
        
    //     tlb_misses++;
    //     return -1;
    //     }
    // }
    return 0;
    
}

void set_dirty_bit_in_tlb(uint32_t address){
    //set the dirty bit of the entry to 1
    uint32_t vpn = address >> 12;

    if(tlb_associativity == 1) {
        int tlb_index = vpn % tlb_entries;
        if(tlb[tlb_index]->valid && tlb[tlb_index]->VPN == vpn) {
            tlb[tlb_index]->dirty = 1;
        }
    }
    if(tlb_associativity == 2) {
        for(int i = 0; i < tlb_entries; i++) {
            if(tlb[i]->valid && tlb[i]->VPN == vpn) {
                tlb[i]->dirty = 1;
            }
        }
    }
    // if (tlb_associativity == 3) {

    //     uint32_t vpn = address >> 12;
    //     int tlb_index = (vpn % tlb_entries*2)*2;

        
    //     if(tlb[tlb_index][0].valid && tlb[tlb_index][0].VPN == vpn && tlb[tlb_index]->lrucounter == 1) {
    //         tlb[tlb_index][0].dirty = 1;
    //     } else if(tlb[tlb_index][1].valid && tlb[tlb_index][1].VPN == vpn && tlb[tlb_index]->lrucounter == 0) {
    //         tlb[tlb_index][1].dirty = 1;
    //     }
        
    // }
}

// LRU replacement policy has to be implemented.
void insert_or_update_tlb_entry(uint32_t address, uint32_t PPN){
    // if the entry is free, insert the entry
    // if the entry is not free, identify the victim entry and replace it
    //set PPN for VPN in tlb
    //set valid bit in tlb
    uint32_t vpn = address >> 12;

    if (tlb_associativity == 1) {
        int tlb_index = vpn % tlb_entries;
        if(!tlb[tlb_index]->valid) {
            tlb[tlb_index]->VPN = vpn;
            tlb[tlb_index]->PPN = PPN;
            tlb[tlb_index]->valid = 1;
        } else {
            tlb[tlb_index]->VPN = vpn;
            tlb[tlb_index]->PPN = PPN;
        }
        
    } else if(tlb_associativity == 2){
        //int j = 1;

        if(counter < tlb_entries){
            for(int i = 0; i < tlb_entries; i++) {
                if(tlb[i]->valid == 0) {
                    tlb[i]->VPN = vpn;
                    tlb[i]->PPN = PPN;
                    tlb[i]->valid = 1;
                    
                    ll *node = make_node(vpn, num);
                    add_to_head(&lru_ll, node);
                    counter++;
                    num++;//index
                    return;
                }
            }

        } else {

            int i = pop_from_tail(&lru_ll);
            tlb[i]->PPN = PPN;
            tlb[i]->VPN = vpn;
            ll *new_node = make_node(vpn, i);
            add_to_head(&lru_ll, new_node);
            counter++;
            return;
            }
        }
   // }

    // if (tlb_associativity == 3){

    //     uint32_t vpn = address >> 12;
    //     int tlb_index = vpn % (tlb_entries)/2;

    
    //     for(int n = 0; n<2; n++){
    //         if(tlb[tlb_index*2 + n]->valid == 0){
    //             tlb[tlb_index*2 + n]->PPN = PPN;
    //             tlb[tlb_index*2 + n]->VPN = vpn;
    //             tlb[tlb_index*2 + n]->valid = 1;

    //             if(boris == 0){
    //                 ll *node = make_node(vpn, num);
    //                 add_to_head(&lru_ll, node);  

    //             } else {
    //                 ll *node2 = make_node(vpn, num);
    //                 add_to_head(&lru_ll_2, node2); 
    //             }

    //             counter++;
    //             num++;
    //             return;

                
    //         }       
    //     }

    


           
        
    // }
}

// print pt entries as per the spec
void print_tlb_entries(){
    //print the tlb entries
    printf("\nTLB Entries (Valid-Bit Dirty-Bit VPN PPN)\n");
    int counter = 0;
    while (counter  < tlb_entries) {
        for(int i = 0; i < tlb_entries; i++) {
            if(tlb[i]->valid) {
                printf("%d %d 0x%05x 0x%05x\n",tlb[i]->valid,tlb[i]->dirty,tlb[i]->VPN,tlb[i]->PPN);
                counter++;
            } else if (tlb[i]->valid == 0) {
                printf("0 0 - -\n");
                counter++;
            }
        }
    }
}

// print tlb statistics as per the spec
void print_tlb_statistics(){
    //print the tlb statistics
    printf("\n* TLB Statistics *\n");
    printf("total accesses: %d\n", tlb_total_accesses);
    printf("hits: %d\n", tlb_hits);
    printf("misses: %d\n", tlb_misses);
}

