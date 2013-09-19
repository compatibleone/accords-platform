#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <curl/curl.h>

#include "riak_backend.h"

#define MIN(a,b) ((a < b) ? (a) : (b))

static char ENABLE_SEARCH_CMD[] = "{\"props\":{\"precommit\":[{\"mod\":\"riak_search_kv_hook\",\"fun\":\"precommit\"}]}}";

static size_t upload(void *ptr, size_t size, size_t nmemb, void *userdata);

CURL *init_curl_common() {
    CURL *curl = curl_easy_init();
    if(curl) {
        // CURLOPT_VERBOSE is useful to enable during debug
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

        // In case of redirection, follow
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    }
    return curl;    
}

void set_curl_query_url(CURL *curl, const char *bucket, const char *key, riak_object_return return_object) {
    char request_buffer[1024];
    int written = sprintf(request_buffer, "http://devriak.market.onapp.com:10018/riak/%s/%s", bucket, key);
    if (RIAK_OPTION_RETURN_OBJECT == return_object) {
        sprintf(&request_buffer[written], "?returnbody=true");
    }
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


long perform_curl_and_get_code(CURL *curl, struct curl_slist *headers) {
    CURLcode res;
    res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    
    long http_code = 0;
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
            
    if (CURLE_OK == res) {
        return http_code;
    }
    else {
        return http_code ? http_code : -1; // Curl returns 0 if no response retrieved from server
    }
}

int perform_curl_and_check(CURL *curl, struct curl_slist *headers) {
    return (200 == perform_curl_and_get_code(curl, headers));
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
    CURL *curl = init_curl_common();
    if(curl) {
        long http_code;
        int success = 0;
        unsigned retries;
        for(retries = CURL_RETRIES; retries > 0 && !success; retries--) {   
            struct transfer_data transfer;
            register_upload_data(curl, &transfer, ENABLE_SEARCH_CMD);

            struct curl_slist *headers = set_http_headers(curl, NULL);
                            
            set_curl_query_url(curl, bucket, "", RIAK_OPTION_NO_OBJECT);
            
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

            http_code = perform_curl_and_get_code(curl, headers);
            success = (204 == http_code);
        }
        if(!success) {
            char *url;
            CURLcode url_retrieved = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
            if (CURLE_OK == url_retrieved) {
                printf("***** Failed to connect to riak server: url '%s': HTTP Code: %lu ************\n", url, http_code);
            }
            else {
                printf("***** Failed to connect to riak server at unknown url for bucket: '%s'. Is it running and accessible? ************\n", bucket);
            }
        }
        curl_easy_cleanup(curl);
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
