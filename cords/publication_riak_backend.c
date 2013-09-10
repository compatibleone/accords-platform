
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <curl/curl.h>

#include "publication_occi_filter.h"
#include "publication.h"
#include "publication_backend_interface.h"
#include "publication_riak_backend.h"

#define MIN(a,b) ((a < b) ? (a) : (b))

static void init();
static void finalise();
static struct cords_publication * create  (struct cords_publication *initial_publication);
static struct cords_publication * retrieve_from_id(char *id);
static publication_list  retrieve_from_filter(struct cords_publication_occi_filter *filter);
static void  update (char *id, struct cords_publication *updated_publication);
static void  del    (char *id);
static void  delete_all_matching_filter (struct cords_publication_occi_filter *filter);
static cords_publication_id_list  list_ids (struct cords_publication_occi_filter *filter);

struct publication_backend_interface *  cords_publication_riak_backend_interface() {
    struct publication_backend_interface riak_interface =
       { init,
         finalise,
         create,
         retrieve_from_id,
         retrieve_from_filter,
         update,
         del,
         delete_all_matching_filter,
         list_ids,
         };
    struct publication_backend_interface *interface_ptr =
            (struct publication_backend_interface *) malloc (sizeof(struct publication_backend_interface));
    interface_ptr->init = riak_interface.init;
    interface_ptr->finalise = riak_interface.finalise;
    interface_ptr->create = riak_interface.create;
    interface_ptr->retrieve_from_id = riak_interface.retrieve_from_id;
    interface_ptr->retrieve_from_filter = riak_interface.retrieve_from_filter;
    interface_ptr->update = riak_interface.update;
    interface_ptr->del = riak_interface.del;
    interface_ptr->delete_all_matching_filter = riak_interface.delete_all_matching_filter;
    interface_ptr->list = riak_interface.list;
    return interface_ptr;
}

void init() {
    curl_global_init(CURL_GLOBAL_ALL);
}

void finalise() {
    curl_global_cleanup();
}

struct transfer_data {
    size_t transfered;
    struct cords_publication *data;
};

static size_t upload(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t limit = size * nmemb;
    struct transfer_data *data = userdata;
    
    size_t bytes_to_transfer = MIN(limit, 14 - data->transfered);
    data->transfered += bytes_to_transfer;    
    sprintf(ptr, "{\"bar\":\"baz\"}");
    return bytes_to_transfer;
}

static size_t download(void *content, size_t size, size_t nmemb, void *userdata) {
    size_t limit = size * nmemb;
    struct transfer_data *data = userdata;
    
    char buf[256];
    size_t bytes_read = MIN(limit, 14);
    memcpy(buf, content, bytes_read);
    buf[bytes_read] = '\0';
    char *colon = strrchr(buf, ':');
    *(colon + 4) = '\0';
    strcpy(data->data->id, colon);
    
    return limit;    
}

struct cords_publication * create  (struct cords_publication *initial_publication) {
    CURL *curl;
    CURLcode res;

    struct cords_publication *new_publication = NULL;
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        
        struct transfer_data data;
        data.transfered = 0;
        data.data = initial_publication;
        
        // Define the function that is used during upload, i.e. formats the category object
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, upload);
        curl_easy_setopt(curl, CURLOPT_READDATA, &data);
        
        char request_buffer[1024];
        sprintf(request_buffer, "http://devriak.market.onapp.com:10018/riak/%s/%s", "publication", initial_publication->id);        
        curl_easy_setopt(curl, CURLOPT_URL, request_buffer);
        
        // In case of redirection, follow
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
     
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        //curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)14); // TODO Use proper files size

        struct curl_slist *headers = NULL;
        // Disable Expect: Continue 100 header
        static const char buf[] = "Expect:"; 
        headers = curl_slist_append(headers, buf);
        
        headers = curl_slist_append(headers, "Content-Type: application/json"); 
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        res = curl_easy_perform(curl);
        
        if (CURLE_OK == res) {
            
            // TODO Extract the publication here
        }
        curl_easy_cleanup(curl);
    }
    return new_publication; 
}

struct cords_publication * retrieve_from_id(char *id) {
    CURL *curl;
    CURLcode res;

    struct cords_publication *retrieved_publication = NULL;
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        
        struct transfer_data data;
        data.transfered = 0;
        data.data = allocate_cords_publication();
        if (data.data) {            
                    
            // Define the function that is used during upload, i.e. formats the category object
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, download);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
            
            char request_buffer[1024];
            sprintf(request_buffer, "http://devriak.market.onapp.com:10018/riak/%s/%s", "publication", id);        
            curl_easy_setopt(curl, CURLOPT_URL, request_buffer);
            
            // In case of redirection, follow
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            
            res = curl_easy_perform(curl);
            
            if (CURLE_OK == res) {
                
                // TODO Extract the publication here
            }
        }
        curl_easy_cleanup(curl);
    }
    return retrieved_publication; 
}

publication_list retrieve_from_filter(struct cords_publication_occi_filter *filter) { 
    publication_list retVal;
    memset(&retVal, 0, sizeof(publication_list));
    return retVal; 
}
void  update (char *id, struct cords_publication *updated_publication) { }
void  del    (char *id) {}
void  delete_all_matching_filter (struct cords_publication_occi_filter *filter) {}
cords_publication_id_list list_ids (struct cords_publication_occi_filter *filter) { 
    cords_publication_id_list retVal;
    memset(&retVal, 0, sizeof(cords_publication_id_list));
    return retVal; 
}
