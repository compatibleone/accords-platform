#ifndef _riak_backend_h
#define _riak_backend_h

#define CURL_RETRIES (3)

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
long perform_curl_and_get_code(CURL *curl, struct curl_slist *headers);
int perform_curl_and_check(CURL *curl, struct curl_slist *headers);

#endif
