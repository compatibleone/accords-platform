#include <curl/curl.h>

#include "riak_backend.h"

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

long perform_curl_and_get_code(CURL *curl, struct curl_slist *headers) {
    CURLcode res;
    res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    
    if (CURLE_OK == res) {
        long http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        return http_code;
    }
    return -1;
}

int perform_curl_and_check(CURL *curl, struct curl_slist *headers) {
    return (200 == perform_curl_and_get_code(curl, headers));
}
