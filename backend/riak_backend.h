#ifndef _riak_backend_h
#define _riak_backend_h

#define RIAK_DEBUG_TRACE (0)
#define RIAK_DEBUG_TRACE_VERBOSE (RIAK_DEBUG_TRACE && 1)

#define CURL_RETRIES (6)

typedef enum {
    RIAK_OPTION_RETURN_OBJECT,
    RIAK_OPTION_NO_OBJECT
} riak_object_return;

struct transfer_data {
    size_t total;
    size_t transfered;
    char *data;
};

struct node_list {
    unsigned n_nodes;
    char **addresses;
};

extern struct node_list global_riak_nodes;

CURL *init_curl_common();
void set_curl_query_url(CURL *curl, const char *bucket, const char *key, riak_object_return return_object);
void set_search_url(CURL *curl, unsigned n_filters, const char *bucket, const char *query);
long perform_curl_and_get_code(CURL *curl, struct curl_slist *headers, unsigned retries);
int perform_curl_and_check(CURL *curl, struct curl_slist *headers, unsigned retries);
void exponential_backoff(unsigned retries);
void debug_curl_failure(CURL* curl, CURLcode res, long http_code);
void global_riak_init();
void enable_riak_search(const char *bucket);
void register_upload_data(CURL *curl, struct transfer_data *transfer, char *data);
struct curl_slist *set_http_headers(CURL *curl, struct curl_slist *headers);
char *vclock_from_headers(const char *headers);

void setup_download(CURL *curl, struct transfer_data *download_data);
void setup_read_vclock(CURL *curl, struct transfer_data *header_data);

#endif
