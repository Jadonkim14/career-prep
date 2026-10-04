#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "cachelab.h"

// 1. Cache line structure
typedef struct {
    int valid;
    unsigned long long tag;
    unsigned long long last_used;
} CacheLine;

// 2. Cache configuration
int s, E, b;

// 3. Cache storage
CacheLine **cache;

// 4. Cache statistics
int hits = 0;
int misses = 0;
int evictions = 0;

// 5. LRU timestamp
unsigned long long clock_count = 0;

// 6. Cache access function
void accessCache(unsigned long long address)
{
    unsigned long long set_index =
        (address >> b) & ((1ULL << s) - 1);

    unsigned long long tag =
        address >> (s + b);

    CacheLine *set = cache[set_index];

    // Search for a cache hit
    for (int i = 0; i < E; i++) {
        if (set[i].valid == 1 && set[i].tag == tag) {
            hits++;
            set[i].last_used = ++clock_count;
            return;
        }
    }

    // Handle a cache miss
    misses++;

    for (int i = 0; i < E; i++) {
        if (set[i].valid == 0) {
            set[i].valid = 1;
            set[i].tag = tag;
            set[i].last_used = ++clock_count;
            return;
        }
    }

    // Handle LRU eviction
    evictions++;

    unsigned long long oldest = set[0].last_used;
    int oldest_idx = 0;

    for (int i = 1; i < E; i++) {
        if (set[i].last_used < oldest) {
            oldest = set[i].last_used;
            oldest_idx = i;
        }
    }

    set[oldest_idx].tag = tag;
    set[oldest_idx].last_used = ++clock_count;
}

void initCache(void)
{
    int S = 1 << s;

    // Allocate an array of set pointers
    cache = calloc(S, sizeof(CacheLine *));

    // Allocate cache lines for each set
    for (int i = 0; i < S; i++) {
        cache[i] = calloc(E, sizeof(CacheLine));
    }
}

void freeCache(void)
{
    int S = 1 << s;

    // TODO: Free the cache lines in each set
    for (int i = 0; i < S; i++) {
        free(cache[i]);
    }

    // TODO: Free the array of set pointers
    free(cache);
}

int main(int argc, char *argv[])
{
    int opt;
    char *trace_file = NULL;
    int verbose = 0;

    while ((opt = getopt(argc, argv, "s:E:b:t:v")) != -1) {
        switch (opt) {
            case 's':
                s = atoi(optarg);
                break;

            // TODO: Handle -E
            case 'E':
                E = atoi(optarg);
                break;

            // TODO: Handle -b
            case 'b':
                b = atoi(optarg);
                break;

            // TODO: Handle -t
            case 't':
                trace_file = optarg;
                break;

            // TODO: Handle -v
            case 'v':
                verbose = 1;
                break;
            
            default:
                fprintf(stderr, "Invalid option: -%c\n", optopt);
                return 1;
        }
    }

    (void)verbose;

    if (trace_file == NULL || s <= 0 || E <= 0 || b <= 0) {
        fprintf(stderr, "Invalid cache configuration\n");
        return 1;
    }

    initCache();

    FILE *fp = fopen(trace_file, "r");
    if (fp == NULL) {
        perror("Failed to open trace file");
        freeCache();
        return 1;
    }

    char line[256];
    char operation;
    unsigned long long address;
    int size;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, " %c %llx,%d",
                &operation, &address, &size) != 3) {
            continue;
        }

        // TODO: Process each memory operation
        switch (operation)
        {
            case 'I':
                break;
            
            case 'L':
            case 'S':
                accessCache(address);
                break;

            case 'M':
                accessCache(address);
                accessCache(address);
                break;
            
            default:
                fprintf(stderr, "Unknown operation: %c\n", operation);
                fclose(fp);
                freeCache();
                return 1;
        }
    }

    fclose(fp);

    printSummary(hits, misses, evictions);

    freeCache();

    return 0;
}