#include "pt.h"
#include <stdlib.h>
#include "tlb.h"
#include "types.h"
#include "ll.h"

page_t* free_page_list;
page_t* used_page_list;
pt_entry_t* page_table;

// Page table statistics counters.
uint32_t page_table_total_accesses;
uint32_t page_table_faults;
uint32_t page_table_faults_with_dirty_page;

int timestamp;
int page_counter;

page_t *remove_from_ll_with_ppn(page_t **list, uint32_t ppn){

    page_t *prev = NULL;
    page_t *current = *list;

    while(current != NULL){
        if(current->ppn == ppn){
            if(prev == NULL){ // if at the head of the list
                *list = current->next;
            } else {
                prev->next = current->next;
            }
            return current;
        }

        prev = current;
        current = current->next;
    }

    return NULL; //if not in ll
   
}

void free_page(page_t** list){

    page_t* head = *list;
    page_t* next_node;

    while (head != NULL){

        next_node = head->next;

        free(head);

        head = next_node;
    }
}

void free_page_table(){

    free(page_table);
    page_table = NULL;

    free_page(&used_page_list);
    free_page(&free_page_list);
}

void initialize_pt_system()
{   
    init_free_page_list(&free_page_list);
    init_pt();

    timestamp = 0;
    page_counter = 0;

    //clear the page table linked list here mabye(m)
    return;
}

/*
 * Initialize the "page_table"
 * The "page_table" is declared in as extern in include/pt.h file.
 */
void init_pt() {

    page_table = (pt_entry_t*)malloc(256*sizeof(pt_entry_t));

    return;
}

void init_free_page_list(page_t** free_page_list){
    for (int i = 255; i >=0; i--)
    {
        page_t *new_page = (page_t *)malloc(sizeof(page_t));
        new_page->ppn = i;
        new_page->next = NULL;
        new_page->time_stamp = timestamp;
        timestamp++;
        insert_in_ll(free_page_list, new_page);
    }
}

// Extract the VPN from the address and use it.
int check_page_table(uint32_t address){

    page_table_total_accesses++;

    int address_index_bits = 12;

    uint32_t vpn = address >> address_index_bits;

    pt_entry_t* page_table_to_check = &page_table[vpn];//need to look at the address of the page table(m)
   
    if(page_table_to_check->present == 1){
        //return PPN if the page is hit
        return page_table_to_check->PPN;

    } else if(page_table_to_check->dirty == 1) {

        page_table_faults_with_dirty_page++;

    } else {

        page_table_faults++;
    }

//return -1 if the entry is missing or present bit is 0 aka page fault
    return -1;
}

// Extract the VPN from the address and use it.
void update_page_table(uint32_t address, uint32_t PPN){

    //set PPN for VPN in page table entry
    //set present bit in page table entry
   

    int address_index_bits = 12;

    uint32_t vpn = address >> address_index_bits;

    pt_entry_t* page_to_check = &page_table[vpn];

    page_to_check->PPN = PPN;

    page_to_check->present = 1;

   

    page_t* head = used_page_list;

    while(head!= NULL){
        if(head->ppn == PPN){
            if(head->page_table_entry != 0){
                head->page_table_entry->present = 0;
            }
        }
        head = head->next;
    }

    insert_or_update_tlb_entry(address, PPN);
}

//set the dirty bit of the entry to 1
void set_dirty_bit_in_page_table(uint32_t address){

    int address_index_bits = 12;

    uint32_t vpn = address >> address_index_bits;

    pt_entry_t* page_to_check = &page_table[vpn];

    page_to_check->dirty = 1;

    return;
   
}

// LRU is to be use to find the victim page
page_t *get_victim_page(){
    // you may use the used_page_list to find the victim page
    // return the victim page

    if(used_page_list == NULL){
        return NULL;
    }

    //loop that checks for the lowest timestamp(m)

    page_t *victim_page = used_page_list;// remember this would be the head of the ll(m)
    page_t *current_page_to_check = victim_page->next;

    while(current_page_to_check != NULL){ // loops until the end of the ll(m)
        if(current_page_to_check->time_stamp < victim_page->time_stamp){// if the next page has a smaller timestamp(m)
            victim_page = current_page_to_check;
        }
        current_page_to_check = current_page_to_check->next;
    }

    remove_from_ll_with_ppn(&used_page_list, victim_page->ppn);//remove that page from page list

    page_table_total_accesses++;

    return victim_page;
}

// pops a page from the free page linked-list
page_t *get_free_page(){
    if (free_page_list == NULL){
        return NULL;
    } else{
        return delete_from_top_of_ll(&free_page_list);
    }
    return 0;
}

// print pt entries as per the spec
void print_pt_entries(){
    printf("\nPage Table Entries (Present-Bit Dirty-Bit VPN PPN)\n");
    for(int i = 0; i < 256; i++) {
        if(page_table[i].present == 1){
            printf("%d %d 0x%05x 0x%05x\n",page_table[i].present,page_table[i].dirty,i,page_table[i].PPN);
        }
        
    }

    
}

// print pt statistics as per the spec
void print_pt_statistics(){
    printf("\n* Page Table Statistics *\n");
    printf("total accesses: %d\n", page_table_total_accesses);
    printf("page faults: %d\n", page_table_faults);
    printf("page faults with dirty bit: %d\n", page_table_faults_with_dirty_page);
}
