#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include <curl/curl.h>

#include "riak_backend.h"

#define DEBUG_TRACE (0)
#define DEBUG_TRACE_VERBOSE (DEBUG_TRACE && 0)

#define MIN(a,b) ((a < b) ? (a) : (b))

static char ENABLE_SEARCH_CMD[] = "{\"props\":{\"precommit\":[{\"mod\":\"riak_search_kv_hook\",\"fun\":\"precommit\"}]}}";

static size_t upload(void *ptr, size_t size, size_t nmemb, void *userdata);

CURL *init_curl_common() {
    CURL *curl = curl_easy_init();
    if(curl) {
        // CURLOPT_VERBOSE is useful to enable during debug
        if (DEBUG_TRACE_VERBOSE) {
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        }

        // In case of redirection, follow
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    }
    return curl;    
}

void set_curl_query_url(CURL *curl, const char *bucket, const char *key, riak_object_return return_object) {
    char request_buffer[1024];
    int written = snprintf(request_buffer, sizeof(request_buffer), "http://devriak.market.onapp.com:10018/riak/%s/%s", bucket, key);
    if (RIAK_OPTION_RETURN_OBJECT == return_object) {
        snprintf(&request_buffer[written], sizeof(request_buffer) - written, "?returnbody=true");
    }
    assert(strnlen(request_buffer, sizeof(request_buffer)) > 0);
    curl_easy_setopt(curl, CURLOPT_URL, request_buffer);
}

void set_search_url(CURL *curl, unsigned n_filters, const char *bucket, const char *query) {
    assert(bucket);
    assert(curl);
    
    char request_buffer[1024];
    if(0 == n_filters) {
        // List - warning, shouldn't be used in production for performance reasons
        snprintf(request_buffer, sizeof(request_buffer), "http://devriak.market.onapp.com:10018/riak/%s?keys=true&props=false", bucket);
    }
    else {
        assert(query);
        if(query) {
            // Here we request up to 100,000 results.  What happens if there are more than 100,000 matches?
            // Don't find out!  
            snprintf(request_buffer, sizeof(request_buffer), "http://devriak.market.onapp.com:10018/solr/%s/select?wt=json&rows=100000&q=%s", bucket, query);            
        }
    }
    curl_easy_setopt(curl, CURLOPT_URL, request_buffer);
}

long perform_curl_and_get_code(CURL *curl, struct curl_slist *headers, unsigned retries) {    
    CURLcode res;
    res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    
    long http_code = 0;
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
    
    if (CURLE_OK == res) {
        return http_code;
    }
    else {
        if (DEBUG_TRACE) {
            char *url;
            CURLcode url_retrieved;
            url_retrieved = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
            if (CURLE_OK == url_retrieved) {
                printf("Request to '%s' failed with http error code '%ld'\n", url, http_code);
            }
            else {
                printf("Failed to connect to riak server at unknown url\n");
            }
            printf("Curl reported error %d, see curl.h for details\n", res);
        }
        return http_code ? http_code : -1; // Curl returns 0 if no response retrieved from server
    }
}

int perform_curl_and_check(CURL *curl, struct curl_slist *headers, unsigned retries) {
    int success = (200 == perform_curl_and_get_code(curl, headers, retries));
    if (!success) {
        exponential_backoff(retries);
    }
    return success;
}

void exponential_backoff(unsigned retries) {
    unsigned backoff = CURL_RETRIES - retries;
    if (retries > 1) { // No point in backing off if we're not going to retry again
        struct timespec requested, elapsed;
        long long sleep_period = backoff ? 1000000ll << (3 * backoff) : 0;
        requested.tv_sec = sleep_period / 1000000000ll;
        requested.tv_nsec = sleep_period % 1000000000ll;
        printf("*** Server failure when attempting to contact riak, backing off for %lums ***\n", requested.tv_sec * 1000 + requested.tv_nsec / 1000000l);
        nanosleep(&requested, &elapsed);
        printf("Backoff complete\n");
    }
}

struct curl_slist *set_http_headers(CURL *curl, struct curl_slist *headers) {
    // Disable Expect: Continue 100 header
    static const char buf[] = "Expect:"; 
    headers = curl_slist_append(headers, buf);            
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    return headers;
}

static size_t upload(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t limit = size * nmemb;
    struct transfer_data *data = userdata;
    
    size_t bytes_to_transfer = MIN(limit, data->total - data->transfered);    
    memcpy(ptr, data->data + data->transfered, bytes_to_transfer);
    data->transfered += bytes_to_transfer;
    return bytes_to_transfer;
}

static size_t download(void *content, size_t size, size_t nmemb, void *userdata) {
    size_t limit = size * nmemb;
    struct transfer_data *data = userdata;
    
    data->data = realloc(data->data, data->transfered + limit + 1);
    if (!data->data) {        
        return 0;
    }
    
    memcpy(data->data + data->transfered, content, limit);
    data->transfered += limit;
    data->data[data->transfered] = '\0';
    
    return limit;    
}

static void initialise_download_operation(CURL *curl, struct transfer_data *data, CURLoption function_option, CURLoption data_option) {
    data->transfered = 0;
    data->data = NULL;          
                
    // Define the function that is used during upload, i.e. formats the category object
    curl_easy_setopt(curl, function_option, download);
    curl_easy_setopt(curl, data_option, data);
}

void setup_download(CURL *curl, struct transfer_data *download_data) {
    initialise_download_operation(curl, download_data, CURLOPT_WRITEFUNCTION, CURLOPT_WRITEDATA);
}

void setup_read_vclock(CURL *curl, struct transfer_data *header_data) {
    initialise_download_operation(curl, header_data, CURLOPT_HEADERFUNCTION, CURLOPT_HEADERDATA);
}

void register_upload_data(CURL *curl, struct transfer_data *transfer, char *data) {
    transfer->transfered = 0;
    transfer->data = data;
    transfer->total = strlen(transfer->data);          
    // Define the function that is used during upload, i.e. formats the category object
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, upload);
    curl_easy_setopt(curl, CURLOPT_READDATA, transfer);
}

void enable_riak_search(const char *bucket) {
    int success = 0;
    unsigned retries;
    long http_code;
    CURL *curl = init_curl_common();
    if(curl) {
        for(retries = CURL_RETRIES; retries > 0 && !success; retries--) {
            struct transfer_data transfer;
            register_upload_data(curl, &transfer, ENABLE_SEARCH_CMD);

            struct curl_slist *headers = set_http_headers(curl, NULL);
                            
            set_curl_query_url(curl, bucket, "", RIAK_OPTION_NO_OBJECT);
            
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

            http_code = perform_curl_and_get_code(curl, headers, retries);
            success = (204 == http_code);
            if (!success) {
                exponential_backoff(retries);
            }
        }
        curl_easy_cleanup(curl);
    }     
    if (!success && DEBUG_TRACE) {
        printf("****** Failed to initialise riak search *******\n");
    }
}

static const char VCLOCK_HEADER[] = "X-Riak-Vclock: ";
char *vclock_from_headers(const char *headers) {
    const char *start = strstr(headers, VCLOCK_HEADER);
    if (start) {
        const char *end = strchr(start, '\r');
        if (end) {
            size_t length = end - start;
            char *retval = malloc(length + 1);
            if (retval) {
                strncpy(retval, start, length);
                retval[length] = '\0';
                return retval;
            }
        }
    }
    assert(0); // Invalid Vclock string
    return NULL;
}
