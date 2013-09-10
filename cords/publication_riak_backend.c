
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <curl/curl.h>
#include <json.h>

#include "allocate.h"

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
    size_t total;
    size_t transfered;
    char *data;
};

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

static char *json_string(const struct cords_publication *publication) {
    json_object *jo = json_object_new_object();
    
    if (publication->id) {
        json_object_object_add(jo, "id", json_object_new_string(publication->id));
    }
    if (publication->operator) {
        json_object_object_add(jo, "operator", json_object_new_string(publication->operator));
    }
    
    char *retVal = allocate_string(json_object_to_json_string(jo));
    
    // Free the object
    json_object_put(jo);
    
    return retVal;
}

static struct cords_publication *cords_publication_from_json(const char *input) {
    struct cords_publication *new_publication = allocate_cords_publication();
    if (new_publication) {
        json_bool success;
        json_object *jo = json_tokener_parse(input);
        
        json_object *id;
        success = json_object_object_get_ex(jo, "id", &id);
        if (success) {
            new_publication->id = allocate_string(json_object_get_string(id));
        }

        json_object *operator;
        success = json_object_object_get_ex(jo, "operator", &operator);
        if (success) {
            new_publication->operator = allocate_string(json_object_get_string(operator));
        }

        // Free the object
        json_object_put(jo);
    }
    
    return new_publication;
}

static const char VCLOCK_HEADER[] = "X-Riak-Vclock: ";
static char *vclock_from_headers(const char *headers) {
    const char *start = strstr(headers, VCLOCK_HEADER);
    if (start) {
        const char *end = strchr(start, '\r');
        if (end) {
            size_t length = end - start + 1;
            char *retval = malloc(length);
            if (retval) {
                strncpy(retval, start, length - 1);
                retval[length] = '\0';
                return retval;
            }
        }
    }
    return NULL;
}

static void initialise_download_operation(CURL *curl, struct transfer_data *data, CURLoption function_option, CURLoption data_option) {
    data->transfered = 0;
    data->data = NULL;          
                
    // Define the function that is used during upload, i.e. formats the category object
    curl_easy_setopt(curl, function_option, download);
    curl_easy_setopt(curl, data_option, data);
}

static void setup_download(CURL *curl, struct transfer_data *download_data) {
    initialise_download_operation(curl, download_data, CURLOPT_WRITEFUNCTION, CURLOPT_WRITEDATA);
}

static void setup_read_vclock(CURL *curl, struct transfer_data *header_data) {
    initialise_download_operation(curl, header_data, CURLOPT_HEADERFUNCTION, CURLOPT_HEADERDATA);
}

static struct cords_publication *create_or_update(const struct cords_publication *initial_publication, const char *vclock) {
    CURL *curl;
    CURLcode res;
    struct cords_publication *new_publication = NULL;    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        
        struct transfer_data data;
        data.transfered = 0;
        data.data = json_string(initial_publication);
        data.total = strlen(data.data);
        if(data.data) {            
            // Define the function that is used during upload, i.e. formats the category object
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, upload);
            curl_easy_setopt(curl, CURLOPT_READDATA, &data);
            
            struct transfer_data response;
            setup_download(curl, &response);
            
            char request_buffer[1024];
            sprintf(request_buffer, "http://devriak.market.onapp.com:10018/riak/%s/%s?returnbody=true", "publication", initial_publication->id);        
            curl_easy_setopt(curl, CURLOPT_URL, request_buffer);
            
            // In case of redirection, follow
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
         
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            //curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)14); // TODO Use proper files size
    
            struct curl_slist *headers = NULL;
            // Disable Expect: Continue 100 header
            static const char buf[] = "Expect:"; 
            headers = curl_slist_append(headers, buf);            
            // If we've been provided with a vclock, add it to the headers to ensure we do an update, and don't create a 
            // sibling
            if (vclock) {
                headers = curl_slist_append(headers, vclock);   
            }            
            
            headers = curl_slist_append(headers, "Content-Type: application/json"); 
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            
            res = curl_easy_perform(curl);
            
            if (CURLE_OK == res) {
                long http_code = 0;
                curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
                if (200 == http_code) {
                    new_publication = cords_publication_from_json(response.data);
                }
            }
            free(data.data);
        }
        curl_easy_cleanup(curl);
    }
    return new_publication;    
}

struct cords_publication *create(struct cords_publication *initial_publication) {
    return create_or_update(initial_publication, NULL);
}

struct publication_with_vclock {
    char *vclock;
    struct cords_publication *publication;
};

struct publication_with_vclock retrieve_with_vclock_from_id(const char *id) {
    struct publication_with_vclock retval = {0};
    
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        
        struct transfer_data data;
        setup_download(curl, &data);
        
        struct transfer_data header_data;
        setup_read_vclock(curl, &header_data);
        
        char request_buffer[1024];
        sprintf(request_buffer, "http://devriak.market.onapp.com:10018/riak/%s/%s", "publication", id);        
        curl_easy_setopt(curl, CURLOPT_URL, request_buffer);
        
        // In case of redirection, follow
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        
        res = curl_easy_perform(curl);
        
        if (CURLE_OK == res) {
            long http_code = 0;
            curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
            if (200 == http_code) {
                retval.publication = cords_publication_from_json(data.data);
                retval.vclock = vclock_from_headers(header_data.data);
            }
        }
        
        free(data.data);
        free(header_data.data);
        curl_easy_cleanup(curl);
    }
    return retval;     
}

struct cords_publication * retrieve_from_id(char *id) {
    struct publication_with_vclock retrieved = retrieve_with_vclock_from_id(id);
    free(retrieved.vclock);
    return retrieved.publication;
}

publication_list retrieve_from_filter(struct cords_publication_occi_filter *filter) { 
    publication_list retVal = {0};
    return retVal; 
}

// Riak has a vclock recorded for each object stored which is used for assisting with
// consistency in the face of different database nodes having different versions.
// In order to ensure Riak updates the correct node, we must first read it and 
// use the vclock returned when sending our update.
void update(char *id, struct cords_publication *updated_publication) { 
    struct publication_with_vclock retrieved = retrieve_with_vclock_from_id(id);
    if (retrieved.vclock && retrieved.publication) {
        create_or_update(updated_publication, retrieved.vclock);
        free(retrieved.vclock);
        free(retrieved.publication);
    }    
}

void del(char *id) {
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        
        char request_buffer[1024];
        sprintf(request_buffer, "http://devriak.market.onapp.com:10018/riak/%s/%s", "publication", id);        
        curl_easy_setopt(curl, CURLOPT_URL, request_buffer);
        
        // In case of redirection, follow
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        
        res = curl_easy_perform(curl);
        
        // TODO We don't return anything, so no need to check for success.  
        // However once we get more advanced and want to retry on failure, we might want to do something here.
//        if (CURLE_OK == res) {
//            long http_code = 0;
//            curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
//            if (200 == http_code) {
//            }
//        }
        curl_easy_cleanup(curl);
    }    
}
void  delete_all_matching_filter (struct cords_publication_occi_filter *filter) {}
cords_publication_id_list list_ids (struct cords_publication_occi_filter *filter) { 
    cords_publication_id_list retVal;
    memset(&retVal, 0, sizeof(cords_publication_id_list));
    return retVal; 
}
