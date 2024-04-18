//
// This file defines the function signatures necessary for creating the three
// cache systems and defines the prefetcher struct.
//

#include "prefetchers.h"

// Null Prefetcher
// ============================================================================
uint32_t null_handle_mem_access(struct prefetcher *prefetcher, struct cache_system *cache_system,
                                uint32_t address, bool is_miss)
{
    return 0; // No lines prefetched
}

void null_cleanup(struct prefetcher *prefetcher) {}

struct prefetcher *null_prefetcher_new()
{
    struct prefetcher *null_prefetcher = calloc(1, sizeof(struct prefetcher));
    null_prefetcher->handle_mem_access = &null_handle_mem_access;
    null_prefetcher->cleanup = &null_cleanup;
    return null_prefetcher;
}

// Sequential Prefetcher
// ============================================================================
// DONE feel free to create additional structs/enums as necessary

uint32_t sequential_handle_mem_access(struct prefetcher *prefetcher,
                                      struct cache_system *cache_system, uint32_t address,
                                      bool is_miss)
{
    // DONE: Return the number of lines that were prefetched.
    struct prefetcher_args *pfa = (struct prefetcher_args *)prefetcher->data;

    for(int i = 1; i <= pfa->prefetch_amount; i+=1) {
        //Need to advance address by one line size per prefetch
        cache_system_mem_access(cache_system, address+(cache_system->line_size*i),'R',true);
    }
    return pfa->prefetch_amount;
}

void sequential_cleanup(struct prefetcher *prefetcher)
{
    // DONE cleanup any additional memory that you allocated in the
    // sequential_prefetcher_new function.
    free(prefetcher->data);
}

struct prefetcher *sequential_prefetcher_new(uint32_t prefetch_amount)
{
    struct prefetcher *sequential_prefetcher = calloc(1, sizeof(struct prefetcher));
    sequential_prefetcher->handle_mem_access = &sequential_handle_mem_access;
    sequential_prefetcher->cleanup = &sequential_cleanup;
    struct prefetcher_args *pfa = malloc(sizeof(struct prefetcher_args));
    pfa->prefetch_amount = prefetch_amount;
    sequential_prefetcher->data = (void *)pfa;

    // DONE allocate any additional memory needed to store metadata here and
    // assign to sequential_prefetcher->data.

    return sequential_prefetcher;
}

// Adjacent Prefetcher
// ============================================================================
uint32_t adjacent_handle_mem_access(struct prefetcher *prefetcher,
                                    struct cache_system *cache_system, uint32_t address,
                                    bool is_miss)
{
    // DONE: perform the necessary prefetches for the adjacent strategy.
    //Advance address by one line size
    cache_system_mem_access(cache_system, address+cache_system->line_size,'R',true);
    // DONE: Return the number of lines that were prefetched.
    return 1;
}

void adjacent_cleanup(struct prefetcher *prefetcher)
{
    // DONE: cleanup any additional memory that you allocated in the
    // adjacent_prefetcher_new function.
}

struct prefetcher *adjacent_prefetcher_new()
{
    struct prefetcher *adjacent_prefetcher = calloc(1, sizeof(struct prefetcher));
    adjacent_prefetcher->handle_mem_access = &adjacent_handle_mem_access;
    adjacent_prefetcher->cleanup = &adjacent_cleanup;

    // DONE allocate any additional memory needed to store metadata here and
    // assign to adjacent_prefetcher->data.

    return adjacent_prefetcher;
}

// Custom Prefetcher - Strided Prefetcher
// ============================================================================
uint32_t custom_handle_mem_access(struct prefetcher *prefetcher, struct cache_system *cache_system,
                                  uint32_t address, bool is_miss)
{
    // TODO perform the necessary prefetches for your custom strategy.
    struct custom_prefetcher_data *cpd = prefetcher->data;
    int jump_size = 0;
    //prefetcher has not been used yet or less than 3 times
    if(cpd->last_address == UINT32_MAX || cpd->last_last_jump == INT32_MAX || cpd->last_jump == INT32_MIN) {
        jump_size = cache_system->line_size;
        cache_system_mem_access(cache_system, address+cache_system->line_size,'R',true);
    }
    else {
        //a pattern exists between the last two jumps
        if(cpd->last_jump == cpd->last_last_jump) {
            //prefetch mem+stride
            cache_system_mem_access(cache_system, address+cpd->last_jump,'R',true);
        }
        else { //no pattern exists, prefetch +1
            cache_system_mem_access(cache_system, address+cache_system->line_size,'R',true);
        }
        jump_size = address - cpd->last_address;
        
    }

    cpd->last_last_jump = cpd->last_jump;
    cpd->last_jump = jump_size;
    cpd->last_address = address;
    // DONE: Return the number of lines that were prefetched.

    return 1;
}

void custom_cleanup(struct prefetcher *prefetcher)
{
    // TODO cleanup any additional memory that you allocated in the
    // custom_prefetcher_new function.
    free(prefetcher->data);
}

struct prefetcher *custom_prefetcher_new()
{
    struct prefetcher *custom_prefetcher = calloc(1, sizeof(struct prefetcher));
    custom_prefetcher->handle_mem_access = &custom_handle_mem_access;
    custom_prefetcher->cleanup = &custom_cleanup;
    // Allocate any additional memory needed to store metadata here and
    // assign to custom_prefetcher->data.
    struct custom_prefetcher_data *cpd = malloc((sizeof(struct custom_prefetcher_data)));
    cpd->last_jump = INT32_MIN;
    cpd->last_last_jump = INT32_MAX;
    cpd->last_address = INT32_MAX;
    custom_prefetcher->data = (void *)cpd;

    

    return custom_prefetcher;
}
