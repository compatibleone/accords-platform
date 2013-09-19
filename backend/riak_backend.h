#ifndef _riak_backend_h
#define _riak_backend_h

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

CURL *init_curl_common();
void set_curl_query_url(CURL *curl, const char *bucket, const char *key, riak_object_return return_object);
void set_search_url(CURL *curl, unsigned n_filters, const char *bucket, const char *query);
long perform_curl_and_get_code(CURL *curl, struct curl_slist *headers);
int perform_curl_and_check(CURL *curl, struct curl_slist *headers);
void enable_riak_search(const char *bucket);
void register_upload_data(CURL *curl, struct transfer_data *transfer, char *data);
struct curl_slist *set_http_headers(CURL *curl, struct curl_slist *headers);
char *vclock_from_headers(const char *headers);

#endif
