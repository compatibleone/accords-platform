#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "backend_profiling.h"

#define NANOS 1000000000LL
#define USED_CLOCK CLOCK_MONOTONIC_RAW 

static void print_filter_count_header(FILE *file, const char *name) {
    fprintf(file, ",%s:zeros,%s:ones,%s:twos,%s:mores", name, name, name, name);
}

static void print_filter_count_counts(FILE *file, const filter_count *counts) {
    fprintf(file, ",%u,%u,%u,%u", counts->zeros, counts->ones, counts->twos, counts->mores);    
}

void save_backend_profile(const char *xml_filename, const struct backend_profile *profile) {
    char filename_buffer[256];
    strcpy(filename_buffer, xml_filename);
    char *last_dot = strrchr(filename_buffer, '.');
    assert(last_dot);
    strcpy(last_dot, ".csv");
    
    FILE *f = fopen(filename_buffer, "w+");
    if (!f) {
        printf("\n\n******************** Unable to open file %s for writing ***************\n\n", filename_buffer);
        return;
    }
    
    fprintf(f, "creates,retrieve_from_ids,retrieve_from_filters,updates,deletes,delete_from_filters,lists,retrieve_from_names");
    print_filter_count_header(f, "retrieve_from_counts");
    print_filter_count_header(f, "delete_from_counts");
    print_filter_count_header(f, "list_counts");
    fprintf(f, "\n");
    
    fprintf(f, "%u,%u,%u,%u,%u,%u,%u,%u",
            profile->creates,
            profile->retrieve_from_ids,
            profile->retrieve_from_filters,
            profile->updates,
            profile->deletes,
            profile->delete_from_filters,
            profile->lists,
            profile->retrieve_from_names);
    print_filter_count_counts(f, &profile->retrieve_from_counts);
    print_filter_count_counts(f, &profile->delete_from_counts);
    print_filter_count_counts(f, &profile->list_counts);
    fprintf(f, "\n");
    
    fclose(f);
}

static long long usecs(long long elapsed) {
    return elapsed / 1000 + (elapsed % 1000 >= 500); // round up halves
}

void profile_print(struct backend_profile *profile, const char *bucket, long long elapsed, long long start) {
    if(profile->iteration_count++ >= 100) {
        profile->iteration_count = 0;
        
        long long now = profile_get_time();
        long long time_since_start = now - start;
        
        // This is magically available to us from the ether...it's the name of the process which we're running in
        extern char *program_invocation_name;
        printf("\n*** Profile: %s:%s: Total elapsed time = %llu ***\n\n", program_invocation_name, bucket, usecs(time_since_start));
        
        printf("*** Counts:\n\n");
        
        printf("Creates: %u\n", profile->creates);
        printf("Retrieves: %u\n", profile->retrieve_from_ids);
        printf("Retrieve from filters: %u\n", profile->retrieve_from_filters);
        printf("Updates: %u\n", profile->updates);
        printf("Deletes: %u\n", profile->deletes);
        printf("Delete from filters: %u\n", profile->delete_from_filters);
        printf("Lists: %u\n", profile->lists);
        printf("Retrieve by names: %u\n", profile->retrieve_from_names);
        
        printf("\n*** Times:\n\n");

        long long total = profile->times.creates + profile->times.retrieve_from_ids + profile->times.retrieve_from_filters
                + profile->times.updates + profile->times.deletes + profile->times.delete_from_filters + profile->times.lists
                + profile->times.retrieve_from_names;
        
        printf("Creates: %llu (%llu%% of category time)\n", usecs(profile->times.creates), profile->times.creates * 100LL / total);
        printf("Retrieves: %llu (%llu%% of category time)\n", usecs(profile->times.retrieve_from_ids), profile->times.retrieve_from_ids * 100LL / total);
        printf("Retrieve from filters: %llu (%llu%% of category time)\n", usecs(profile->times.retrieve_from_filters), profile->times.retrieve_from_filters * 100LL / total);
        printf("Updates: %llu (%llu%% of category time)\n", usecs(profile->times.updates), profile->times.updates * 100LL / total);
        printf("Deletes: %llu (%llu%% of category time)\n", usecs(profile->times.deletes), profile->times.deletes * 100LL / total);
        printf("Delete from filters: %llu (%llu%% of category time)\n", usecs(profile->times.delete_from_filters), profile->times.delete_from_filters * 100LL / total);
        printf("Lists: %llu (%llu%% of category time)\n", usecs(profile->times.lists), profile->times.lists * 100LL / total);
        printf("Retrieve by names: %llu (%llu%% of category time)\n", usecs(profile->times.retrieve_from_names), profile->times.retrieve_from_names * 100LL / total);
                
        printf("\nTotal time for this category = %llu (%llu%% of total time)\n", usecs(total), total * 100LL / time_since_start);
        
        printf("\n***************** End of profiling ****************\n");
        
    }
    
    long long usecs = elapsed / 1000 + (elapsed % 1000 >= 500); // round up halves
    
}

long long profile_get_time() {
    struct timespec now = {0};
    
    clock_gettime(USED_CLOCK, &now);
    return now.tv_sec*NANOS + now.tv_nsec;
}
