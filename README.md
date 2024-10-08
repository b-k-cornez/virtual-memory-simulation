[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/FC8jwUdb)
![BuildAndTest](../../workflows/BuildAndTest/badge.svg?branch=main) ![Points badge](../../blob/badges/.github/badges/points.svg)

# Inf2C-CS 2023/24 | Coursework 2 | VM & Cache Simulator

**Deadline:** Friday 01st December 2023 at 16:00 BST

**Instructors:** Boris Grot, Yuvraj Patel

**TA:** Dilina Dehigama, Shyam Jesalpura

We are releasing coursework2 in two parts to help spread out the work and reduce the stress. This document is for Part 2 (released on 17/11/2024). Part 1 was released on 10/11/2024. In Part 2, your overall goal is to understand the interplay between the processor cache and virtual memory subsystems. In particular, we will comprehend how virtual to physical address translations happen and how the data is accessed in the hardware cache. In Part 1, your task was to simulate the cache. In Part 2, you will work on building a TLB and Page table simulator, and to integrate it with your cache simulator from Part 1. 

You will continue working with your project partner on Part 2. You are not allowed to change partners.  

You are required to submit the code for Part 2, which will include the code for simulating the processor cache (from Part 1), the TLB, and the Page Tables. 


## 1. Downloading the coursework

To get started, you will need to **clone** this repository. If you have usSTUDENT-ID STUDENT-FIRST-NAME-LAST-NAMEed GitHub before, you will already have a HTTPS access token or an SSH key. If you do not have either, you will need to create one of them following the guides below:
  - To create an SSH key, use [this guide](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent).
  - To create a HTTPS token, use [this guide](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token). Feel free to tick all the boxes when asked what permissions you would like to grant this token.

At this point, you can clone the repository:
  - If you used HTTPS above, use the command:
    ```
    git clone https://github.com/inf2c-cs-2022/REPOSITORY_NAME.git
    ```
  - If you used SSH, use the command:
    ```
    git clone git@github.com:inf2c-cs-2022/REPOSITORY_NAME.git
    ```
Once the clone has finished, you can enter the repository directory:
```
cd REPOSITORY_NAME
```
If this is your first time using Git, you will need to do a bit of configuration:
  ```
  git config --global user.name "your_github_username"
  git config --global user.email "your_github_email"
  ```
This will set your username and email globally, so all future GitHub repositories will inherit this configuration (unless they overwrite it). If you do not wish to do that, just omit the `--global`. You can verify the setup using `git config -l`, which will tell you what you just set.

## 2. Writing your solutions

In the `src` folder, you will find the template files where you will write your code. Do not change the files containing the "Do not change" header. You will copy the cache.c from the Part 1 repository into the Part 2 repository. For Part 2, you will be adding functionality to the tlb.c and pt.c files. You are allowed to add more functions in the tlb.c and pt.c. You are free to modify the cache.c file if you want to. As with Part 1, you will find a trace.txt file that you will use for simulating the cache and testing your code. You are free to modify the trace file or create other trace files and accordingly pass the filename as a parameter to the program. 

### How to compile

``` 
cd src/
make
```

To avoid cross-system compatibility issues, you must make sure that your simulator compiles (without warnings) and runs on DICE. We strongly suggest you develop it on DICE as well to avoid last-minute surprises. You can also use Virtual DICE, a version of DICE Linux that runs in VirtualBox, for your work if you want to. You can find more information on Virtual DICE here – https://blog.inf.ed.ac.uk/systems/2023/10/31/virtual-dice-is-now-available/ and https://computing.help.inf.ed.ac.uk/vdice. Please note that we will be using DICE machines for grading purposes. CODE THAT DOES NOT RUN ON DICE WILL RECEIVE A MARK OF 0 REGARDLESS OF HOW WELL IT RUNS ON YOUR SYSTEM.


## 3. Pushing your changes to GitHub

You are encouraged to commit your work regularly. This allows you to track the history of your changes and revert to an earlier version of your code if you need to. It also protects you from losing any of your work in the case of a computer failure. Once you push your code to GitHub, the auto-grader will check your code by running the public test cases and provide you with a score. 

## 4. Testing your solutions locally

We will provide a few test cases for you to verify that your code is correct. Remember that the shared test cases are not exhaustive in nature. They are only there as a hint to help you with testing. We encourage you to create your own trace files to test different scenarios. The autograder will also run the shared test cases when you commit the code. Do not try to work on a solution that passes the provided test. Instead, try to implement a correct solution that would pass any test case. 
Initially, we will release a few simple test cases that you can use to verify your code. Later, we will release a few more test cases that are a bit complex and tries to check different aspects of your implementation. We will inform you when we release more test cases. 

## 5. Input and output

### Input:

The trace file will contain the virtual address references & access mode (R/W) you will simulate. The format is simple:

AccessMode(R/W) Virtual-Address

The mode can either be "R" or "W"; "R" indicates a memory read. "W" indicates a memory write. The virtual address is a 32-bit address specified in hex format. 

As an example, the trace file might contain:

    R 0x00001000
    R 0x00002000
    W 0x00001004

This trace represents three reads from different memory locations followed by a write. Please note that the 2nd and 3rd addresses are nearby.

Please note that all the traces would contain 4-byte aligned addresses.


### Output:

In normal mode (i.e., without the -v option), the program should print out statistics as below at the end. The output should follow the printing format strictly.

    * TLB Statistics *
    total accesses: 3
    hits: 1
    misses: 2
    
    * Page Table Statistics *
    total accesses: 1
    page faults: 2
    page faults with a dirty bit: 0

    * Cache Statistics *
    total accesses: 3
    hits: 1
    misses: 2
    total reads: 2
    read hits: 0
    total writes: 1
    write hits: 1

If verbose is on (-v argument provided), you need to print additional information apart from the above-mentioned statistical information. After every read or write, you will print the virtual address read from the trace file, the physical address, followed by a HIT or MISS for the TLB, Page Table, and the Cache. There is a page fault when the entry is not found in the page table. So, instead of a PT-MISS, you should print PAGE-FAULT. In case of a TLB Hit, as the Page Table is not consulted, use "-" in the output. In addition, you will also print the contents of the TLB and the Page Table. For the Page Table, only print the entries when the present bit is set. The format for the TLB entries is Valid-Bit Dirty-Bit VPN PPN. For the Page Table entries, the format is Present-Bit Dirty-Bit VPN PPN. In case of associativity more than 1, you should print the entries in the index order.

For example, the above trace might lead to the following output:

    R 0x00001000 0x00000000 TLB-MISS PAGE-FAULT CACHE-MISS
    R 0x00002000 0x00001000 TLB-MISS PAGE-FAULT CACHE-MISS
    W 0x00001004 0x00000004 TLB-HIT PAGE-HIT CACHE-HIT

TLB Entries (Valid-Bit Dirty-Bit VPN PPN)

    1 1 0x00001 0x00000
    1 0 0x00002 0x00001   

Page Table Entries (Present-Bit Dirty-Bit VPN PPN)

    1 1 0x00001 0x00000
    1 0 0x00002 0x00001 

While accessing the first two read access, the corresponding TLB and Page Table entries are not present. Hence, there will be a TLB-MISS and a PAGE-FAULT. For the first write access, as the TLB is present in the TLB, there will be a TLB-HIT. 

The first write access(0x1004) might be a HIT if the block size is 16 bytes (or more), and the first read access (0x1000) ends up as a MISS, fetching the entire block and leading to cache hits for the subsequent spatially nearby memory accesses. 


## 6. Task Overview & Specifics

In Part 2 of this coursework, you will build a TLB and Page Table simulator to understand how virtual memory works. Please refer to the lecture slides and the course textbook for cache, TLB, and Page Table fundamentals.

Before we discuss the specifics, let us first understand the flow of the data accessed by the CPU for a particular virtual address. 

Whenever a CPU tries to access a virtual address, the following happens:

    1. The hardware checks TLB for virtual page number (VPN). 
      a. If the VPN is found in the TLB (i.e., the valid bit is set), there is a TLB hit; the physical page number (PPN) is accessed for address translation. The physical address is constructed by merging the PPN and the offset. If the operation is a write operation, set the dirty bit in the TLB entry. Also set the corresponding PTE dirty bit. 
      b. If the VPN is not found in the TLB (i.e., the valid bit is clear or no entry is found), there is a TLB Miss.
        A. Walk the page table to find the VPN -> PPN mapping.
          I. On finding the mapping as a page table entry (PTE) in the page table, the TLB is loaded with the VPN-> PPN mapping. If needed, the entry may replace some other mapping in the TLB using a TLB replacement policy. As the page is present in the memory, the present bit is set in the PTE. The physical address is constructed by merging the PPN and the offset. If the operation is a write operation, set the dirty bit in the TLB entry. Also set the corresponding PTE dirty bit.
          II. If the VPN -> PPN mapping is not found in the page table, a page fault occurs as the page is not present in the memory (i.e., the present bit is clear) and needs to be loaded from the disk. On a page fault, 
            i.	If the memory is not full (i.e., there are free pages available), pick one free page from the free list, read the referenced page from the disk and put into the free page, update the page table, set the present bit in the PTE, and update the TLB with VPN -> PPN mapping by replacing an entry if necessary. If the operation is a write operation, set the dirty bit in the TLB entry. Also set the corresponding PTE dirty bit. 
            ii.	If the memory is full, make space to accommodate the newly read page from the disk.
              1. Select a victim page amongst all the pages existing in the memory using a page replacement policy. 
              2. If the victim page is not dirty (i.e., dirty bit is clear)
                A. Read the referenced page from the disk into the memory, updates the page table, sets the present bit in the PTE, and updates the TLB with VPN-> PPN mapping
                    by replacing the victim page's entry. The physical address is constructed by merging the PPN and the offset. If the operation is a write operation, set the dirty bit in the TLB entry. Also set the corresponding PTE dirty bit. 
              3. If the victim page is dirty (i.e., dirty bit is set), 
                A. Write the victim page out to the disk and clear the dirty bit. Then, read the referenced page from the disk into the memory, update the page table, set the present bit in the PTE, and update the TLB with VPN-> PPN mapping by replacing the victim page's entry. The physical address is constructed by merging the PPN and the offset. If the operation is a write operation, set the dirty bit in the TLB entry. Also set the corresponding PTE dirty bit. 

    2. After translating the virtual address to a physical address, the hardware checks if the block (cache line) is in the cache. Please refer to the specification of Part 1 for these steps. Their implementation does not need to change for Part 2.

### Specifics

You should write a program called a simulator, which accepts the following command-line arguments:

|Argument |Description |
|--|--|
|-S size| The -S argument controls the size of the cache size in bytes. For example, -S 32 will set the cache size to 32 bytes.|
|-T entries| The -T argument controls the number of TLB entries. For example, -T 16 will set the TLB having 16 entries. The minimum number of entries should be 2.|
|-A associativity | The -A argument sets the associativity of the cache. For example, 1 is direct mapped, 2 is fully associative, 3 is 2-way associative, and 4 is 4-way associative. |
|-L associativity| The -L argument sets the associativity of the TLB. For example, 1 is direct mapped, 2 is fully associative, 3 is 2-way associative, and 4 is 4-way associative. |
|-B blocksize | The -B argument controls the cache line (or block) size of the cache in bytes. The minimum should be 4 bytes, and all the cache line size should be a multiple of 4 bytes. |
|-t trace file | The -t argument enables passing the file name of the trace file that contains the virtual address that needs to be traced by the simulator. |
|-v verbose | The -v argument will enable verbose and help with debugging. |

Please note that you have already implemented the handling of the S, A, B, and t parameters as part of Part 1 implementation. You will need to handle the T and L parameters. Additionally, you will modify the -v handling to accommodate the changes needed in the output.

There will be some options when specified might be invalid. For example, block sizes that are not multiple of 4, trace files not existing, associativity arguments being invalid, etc. It is your responsibility to figure out all the invalid cases. On encountering invalid cases, the program should print the error message `Invalid configuration` and exit the program with exit code 1. 

Hint: To effectively handle the options, you should likely use the getopt package. Start with a simple example and work your way through to support multiple arguments. 

You are expected to implement the steps discussed in the overview section. The program will open the trace file (filename passed as a parameter), read one line at a time, and simulate the TLB, Page table, and cache access. First, the virtual address read from the trace file will be translated into a physical address by consulting the TLB and Page tables. The physical address will then be passed to the Cache for access. The format of the trace file is discussed above. For the sake of simplicity, you can assume that the virtual and physical addresses are 32-bit. You will start with an empty TLB, Page Table, and Cache on start-up. You will reuse the code you implemented for cache simulation in Part 1 for Part 2, too. You are expected to implement the TLB and Page Table simulation.

### Simulating the TLB

You will assume that there is only one TLB for instructions and data. The page size for the simulated machine is 4KB. The number of TLB entries and the associativity is specified by the -T and -L options, respectively. You will start with an empty TLB on start-up.

You can assume that each TLB entry will have the following fields:

1. Valid bit - indicates whether the entry is valid.
2. Dirty bit - indicates whether the page has been modified.
3. Virtual Page Number (Tag) (31-12 bits)
4. Physical Page Number (31-12 bits)

You can break the Tag into Tag and Index/Set when the associativity is set associative. You can add more entries if needed to implement the replacement policy. 

During the simulation, you should record the # of accesses to the TLB, the # of hits, and # of misses.

### Simulating the Page Table

The page table structure that you will simulate is a linear page table. To reduce the storage requirements for your page table, you should assume that bits 26 through 31 of the virtual address are always 0 (in other words, the maximum address value in the trace file is 2^26-1). The size of the main memory on the machine you are simulating is 256 pages (total size = 256 * 4K = 1MB). Initially, the main memory should be empty. You will have a page-free list (256 pages initially) you will use whenever a page is allocated. Your page allocation policy should simply hand out the physical pages in order of increasing page number (starting at 0) until the free list is empty. Once all the 256 pages are used (i.e., the page-free list is empty), you should use an LRU replacement policy to simulate how the main memory is managed. Since we are simulating a single process and it never dies, you can assume that the pages will never be added back to the free list. At start-up, we will set up the free page list using the linked list code provided for you. You will use the provided dummy functions to simulate the pages being read from the disk (dummy_read_page_from_disk()) and written to the disk (dummy_write_page_to_disk ()). You do not need to modify this linked list code or the dummy functions.

Remember that on a TLB Hit or when there is a hit while walking the page table for a particular VPN, your LRU implementation will have to consider it as an access to the page. If you do not track such events properly, your LRU implementation may provide you with the wrong victim page leading to your output being wrong. You may want to use the given linked-list APIs to manage the used/allocated pages. 

 while implementing LRU, you will have to 

You can assume that each page table entry will have the following fields:

1. Present bit - indicates whether the page is present in the memory.
2. Dirty bit - indicates whether the page is dirty.
3. Physical Page Number (31-12 bits) (Remember that the total memory size is 1 MB. So calculate how many MSBs will have 0's).

Remember that for linear page tables, you do not need to store the VPNs because the index within the table represents the VPN. 

You may add more entries for your implementation, such as a replacement policy. 

During the simulation, you should record the # of accesses to the page table, # of page faults, and # of page faults where the pages are dirty. 
 
### Simulating the Hardware Cache (Part 1 implementation)

Please refer to Part 1 specification to look at the steps. For the sake of simplicity, we have not included those steps here.

## 8. Submitting your code

You are finished once you have pushed all your changes to GitHub and are happy with your code and points. Once the deadline has passed, we will revoke your write access to this repository and grade your work. The last version committed before the deadline is the one that will be marked. 

You also need to create a text file and specify the name and the student id of your partner. The filename should be partner.txt and use the following format to specify the information

STUDENT-ID[space]STUDENT-FIRST-NAME-LAST-NAME

## 9. Good scholarly practice

Please remember the good scholarly practice requirements of the University regarding work for credit. You can find guidance on the School page:

https://web.inf.ed.ac.uk/infweb/admin/policies/academic-misconduct

This also has links to the relevant University pages.

As mentioned, your code will be subject to further automatic and manual review after the deadline. Submitted code will be checked for similarity with other submissions using the [MOSS system](http://theory.stanford.edu/~aiken/moss/). MOSS has been effective at finding similarities in the past and it is not fooled by name changes or reordering of code blocks. Courseworks are **individual**, and we expect everyone to submit their sole, independent work.

Extensions or Extra Time Adjustments (ETA) are permitted up to a maximum of six days, but cannot be combined. If assessed coursework is submitted late without an approved extension or ETA, a penalty of 5% per calendar day will be applied, up to a maximum of six days after the original deadline, after which a mark of zero will be given.

## 10. Asking questions

If you have questions about the coursework, you should consult the lecture slides, lab materials and the appendix of the Patterson and Hennessy course textbook. If you have questions about the coursework, please start by **checking existing discussions** on Piazza. You should only start a new discussion if you cannot find the answer in the existing posts. It is quite possible that other students will already have encountered and solved the same problem and will be able to help you. The TA will also monitor Piazza and clarify things as necessary, after allowing time for student discussion to take place. Please be reminded that academic misconduct regulations also apply to Piazza, so you should **not** post coursework solutions or code snippets publicly. If in doubt, make your question private, and an instructor will check if the post is acceptable.
