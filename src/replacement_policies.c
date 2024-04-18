//
// This file contains all of the implementations of the replacement_policy
// constructors from the replacement_policies.h file.
//
// It also contains stubs of all of the functions that are added to each
// replacement_policy struct at construction time.
//
// ============================================================================
// NOTE: It is recommended that you read the comments in the
// replacement_policies.h file for further context on what each function is
// for.
// ============================================================================
//

#include "replacement_policies.h"

// LRU Replacement Policy
// ============================================================================
// Feel free to create additional structs/enums as necessary

void lru_cache_access(struct replacement_policy *replacement_policy,
                      struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    // Update the LRU replacement policy state given a new memory access
    // Find cache line and update time of last access
    struct cache_line *cl = cache_system_find_cache_line(cache_system,set_idx,tag);
    cl->last_used = cache_system->time;
    cache_system->time += 1;
}

uint32_t lru_eviction_index(struct replacement_policy *replacement_policy,
                            struct cache_system *cache_system, uint32_t set_idx)
{
    // return the index within the set that should be evicted.
    // Find line within set with smallest time of last access.
    uint32_t oldest = UINT32_MAX;
    uint32_t oldestidx = 0;
    for(uint32_t i = 0; i < cache_system->associativity; i++) {
        struct cache_line *cl = &(cache_system->cache_lines[(set_idx * cache_system->associativity) + i]);
        if(oldest > cl->last_used) {
            oldest = cl->last_used;
            oldestidx  = i;
        }
    }
    return oldestidx;
}

void lru_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    //No additional cleanup required, no dynamically allocated variables
}

struct replacement_policy *lru_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    struct replacement_policy *lru_rp = calloc(1, sizeof(struct replacement_policy));
    lru_rp->cache_access = &lru_cache_access;
    lru_rp->eviction_index = &lru_eviction_index;
    lru_rp->cleanup = &lru_replacement_policy_cleanup;
    // allocate any additional memory to store metadata here and assign to
    // lru_rp->data.

    return lru_rp;
}

// RAND Replacement Policy
// ============================================================================
void rand_cache_access(struct replacement_policy *replacement_policy,
                       struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    // update the RAND replacement policy state given a new memory access
    //random -> do nothing
    cache_system->time += 1;
}

uint32_t rand_eviction_index(struct replacement_policy *replacement_policy,
                             struct cache_system *cache_system, uint32_t set_idx)
{
    // return random index within the set that should be evicted.
    // Need to be careful about generating random numbers, see here 
    // https://stackoverflow.com/questions/1202687/how-do-i-get-a-specific-range-of-numbers-from-rand
    uint32_t line = (uint32_t)((double)rand() / ((double)RAND_MAX + 1) * cache_system->associativity);
    return line;
}

void rand_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    //No additional cleanup required, no extra dynamically allocated variables
}

struct replacement_policy *rand_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    // Seed randomness
    srand(time(NULL));

    struct replacement_policy *rand_rp = malloc(sizeof(struct replacement_policy));
    rand_rp->cache_access = &rand_cache_access;
    rand_rp->eviction_index = &rand_eviction_index;
    rand_rp->cleanup = &rand_replacement_policy_cleanup;

    // Allocate any additional memory to store metadata here and assign to
    // rand_rp->data.

    return rand_rp;
}

// LRU_PREFER_CLEAN Replacement Policy
// ============================================================================
void lru_prefer_clean_cache_access(struct replacement_policy *replacement_policy,
                                   struct cache_system *cache_system, uint32_t set_idx,
                                   uint32_t tag)
{
    // Update the LRU replacement policy state given a new memory access
    // Find cache line and update time of last access
    struct cache_line *cl = cache_system_find_cache_line(cache_system,set_idx,tag);
    cl->last_used = cache_system->time;
    cache_system->time += 1;
}

uint32_t lru_prefer_clean_eviction_index(struct replacement_policy *replacement_policy,
                                         struct cache_system *cache_system, uint32_t set_idx)
{
    // return the index within the set that should be evicted.
    // Find line within set with smallest time of last access.
    uint32_t oldest = UINT32_MAX;
    uint32_t oldestidx = 0;
    bool foundclean = false;
    int start = set_idx * cache_system->associativity;
    for(uint32_t i = 0; i < cache_system->associativity; i++) {
        struct cache_line *cl = &(cache_system->cache_lines[start + i]);
        if(cl->status == EXCLUSIVE && oldest > cl->last_used) {
            oldest = cl->last_used;
            oldestidx  = i;
            foundclean = true;
        }
    }
    //Just find last used block, guaranteed to be dirty since no clean was found
    if(foundclean == false) {
        oldest = UINT32_MAX;
        int start = set_idx * cache_system->associativity;
        for(uint32_t i = 0; i < cache_system->associativity; i++) {
            struct cache_line *cl = &(cache_system->cache_lines[start + i]);
            if( oldest > cl->last_used) {
                oldest = cl->last_used;
                oldestidx  = i;
            }
        }
    }
    return oldestidx;
}

void lru_prefer_clean_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    //No additional cleanup required, no dynamically allocated variables
}

struct replacement_policy *lru_prefer_clean_replacement_policy_new(uint32_t sets,
                                                                   uint32_t associativity)
{
    struct replacement_policy *lru_prefer_clean_rp = malloc(sizeof(struct replacement_policy));
    lru_prefer_clean_rp->cache_access = &lru_prefer_clean_cache_access;
    lru_prefer_clean_rp->eviction_index = &lru_prefer_clean_eviction_index;
    lru_prefer_clean_rp->cleanup = &lru_prefer_clean_replacement_policy_cleanup;

    // Allocate any additional memory to store metadata here and assign to
    // lru_prefer_clean_rp->data.

    return lru_prefer_clean_rp;
}
