#ifndef _backend_profiling_h
#define _backend_profiling_h

typedef struct filter_count_t {
    int zeros;
    int ones;
    int twos;
    int mores;
} filter_count;

struct backend_profile {
    int creates;
    int retrieve_from_ids;
    int retrieve_from_filters;
    int updates;
    int deletes;
    int delete_from_filters;
    int lists;
    int retrieve_from_names;
    filter_count retrieve_from_counts;
    filter_count delete_from_counts;
    filter_count list_counts;
};

void save_backend_profile(const char *filename, const struct backend_profile *profile);

//#define BACKEND_PROFILING

#endif
