#ifndef _backend_profiling_h
#define _backend_profiling_h

typedef struct filter_count_t {
    int zeros;
    int ones;
    int twos;
    int mores;
} filter_count;

struct profile_times {
    long long creates;
    long long retrieve_from_ids;
    long long retrieve_from_filters;
    long long updates;
    long long deletes;
    long long delete_from_filters;
    long long lists;
    long long retrieve_from_names;
};

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
    struct profile_times times;
    unsigned iteration_count;
};

void save_backend_profile(const char *filename, const struct backend_profile *profile);

long long profile_get_time();
void profile_print(struct backend_profile *profile, const char *bucket, long long elapsed, long long start);

#define BACKEND_PROFILING

#endif
