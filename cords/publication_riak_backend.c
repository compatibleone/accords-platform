
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <curl/curl.h>
#include <json.h>

#include "allocate.h"

#include "publication_occi_filter.h"
#include "publication.h"
#include "publication_backend_interface.h"
#include "publication_riak_backend.h"

#define MIN(a,b) ((a < b) ? (a) : (b))

#define CURL_RETRIES (3)

typedef enum {
    RIAK_OPTION_RETURN_OBJECT,
    RIAK_OPTION_NO_OBJECT
} riak_object_return;

union riak_object_list {
    cords_publication_id_list ids_only;
    cords_publication_list objects;
};

struct transfer_data {
    size_t total;
    size_t transfered;
    char *data;
};

static char ENABLE_SEARCH_CMD[] = "{\"props\":{\"precommit\":[{\"mod\":\"riak_search_kv_hook\",\"fun\":\"precommit\"}]}}";

// API Functions
static void init();
static void finalise();
static struct cords_publication *create(struct cords_publication *initial_publication);
static struct cords_publication *retrieve_from_id(char *id);
static cords_publication_list retrieve_from_filter(struct cords_publication_occi_filter *filter);
static void update(char *id, struct cords_publication *updated_publication);
static void del(char *id);
static void delete_all_matching_filter(struct cords_publication_occi_filter *filter);
static cords_publication_id_list list_ids(struct cords_publication_occi_filter *filter);

// Local Functions
static CURL *init_curl_common();
static union riak_object_list list_from_filter(struct cords_publication_occi_filter *filter, riak_object_return return_objects);
static void set_curl_query_url(CURL *curl, const char *bucket, const char *key, riak_object_return return_object);
static size_t upload(void *ptr, size_t size, size_t nmemb, void *userdata);
static long perform_curl_and_get_code(CURL *curl);
static int perform_curl_and_check(CURL *curl);

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

static void set_http_headers(CURL *curl, struct curl_slist *headers) {
    // Disable Expect: Continue 100 header
    static const char buf[] = "Expect:"; 
    headers = curl_slist_append(headers, buf);            
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
}

static void register_upload_data(CURL *curl, struct transfer_data *transfer, char *data) {
    transfer->transfered = 0;
    transfer->data = data;
    transfer->total = strlen(transfer->data);          
    // Define the function that is used during upload, i.e. formats the category object
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, upload);
    curl_easy_setopt(curl, CURLOPT_READDATA, transfer);
}

static void enable_riak_search() {
    CURL *curl = init_curl_common();
    if(curl) {
        int success = 0;
        unsigned retries;
        for(retries = CURL_RETRIES; retries > 0 && !success; retries--) {   
            struct transfer_data transfer;
            register_upload_data(curl, &transfer, ENABLE_SEARCH_CMD);

            struct curl_slist *headers = NULL;
            set_http_headers(curl, headers);
                            
            set_curl_query_url(curl, "publication", "", RIAK_OPTION_NO_OBJECT);
            
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

            success = (204 == perform_curl_and_get_code(curl));
        }
        curl_easy_cleanup(curl);
    } 
}

void init() {
    curl_global_init(CURL_GLOBAL_ALL);
    enable_riak_search();
}

void finalise() {
    curl_global_cleanup();
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

static char *json_string(const struct cords_publication *publication) {
    struct json_object *jo = json_object_new_object();
    
    if (publication->id) {
        json_object_object_add(jo, "id", json_object_new_string(publication->id));
    }
    if (publication->operator) {
        json_object_object_add(jo, "operator", json_object_new_string(publication->operator));
    }
    if (publication->identity) {
        json_object_object_add(jo, "identity", json_object_new_string(publication->identity));
    }
    
    char *retVal = allocate_string(json_object_to_json_string(jo));
    
    // Free the object
    json_object_put(jo);
    
    return retVal;
}

static struct cords_publication *cords_publication_from_json_object(struct json_object *jo) {
    json_bool success;
    
    struct json_object *id;
    success = json_object_object_get_ex(jo, "id", &id);
    if (success) {
        struct cords_publication *new_publication = allocate_cords_publication();
        if (!new_publication) {
            return NULL;
        }
        new_publication->id = allocate_string(json_object_get_string(id));
        
        struct json_object *operator;
        success = json_object_object_get_ex(jo, "operator", &operator);
        if (success) {
            new_publication->operator = allocate_string(json_object_get_string(operator));
        }
        
        struct json_object *identity;
        success = json_object_object_get_ex(jo, "identity", &identity);
        if (success) {
            new_publication->identity = allocate_string(json_object_get_string(identity));
        }
        return new_publication;
    }
    return NULL;
}

static struct cords_publication *cords_publication_from_json_string(const char *input) {
    struct json_object *jo = json_tokener_parse(input);            
    struct cords_publication *new_publication = cords_publication_from_json_object(jo);    
    json_object_put(jo); // Free the object
    return new_publication;
}

static const char VCLOCK_HEADER[] = "X-Riak-Vclock: ";
static char *vclock_from_headers(const char *headers) {
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

static CURL *init_curl_common() {
    CURL *curl = curl_easy_init();
    if(curl) {
        // CURLOPT_VERBOSE is useful to enable during debug
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

        // In case of redirection, follow
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    }
    return curl;    
}

static void set_curl_query_url(CURL *curl, const char *bucket, const char *key, riak_object_return return_object) {
    char request_buffer[1024];
    int written = sprintf(request_buffer, "http://devriak.market.onapp.com:10018/riak/%s/%s", bucket, key);
    if (RIAK_OPTION_RETURN_OBJECT == return_object) {
        sprintf(&request_buffer[written], "?returnbody=true");
    }
    curl_easy_setopt(curl, CURLOPT_URL, request_buffer);
}

long perform_curl_and_get_code(CURL *curl) {
    CURLcode res;
    res = curl_easy_perform(curl);
    
    if (CURLE_OK == res) {
        long http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        return http_code;
    }
    return -1;
}

int perform_curl_and_check(CURL *curl) {
    return (200 == perform_curl_and_get_code(curl));
}

static struct cords_publication *create_or_update(const struct cords_publication *initial_publication, const char *vclock) {
    CURL *curl;
    struct cords_publication *new_publication = NULL;    
    curl = init_curl_common();
    if(curl) {
        unsigned retries;
        for(retries = CURL_RETRIES; retries > 0 && NULL == new_publication; retries--) {
            char *data = json_string(initial_publication);
            if(data) {
                struct transfer_data transfer;
                register_upload_data(curl, &transfer, data);
                
                struct transfer_data response = {0};
                setup_download(curl, &response);
                
                set_curl_query_url(curl, "publication", initial_publication->id, RIAK_OPTION_RETURN_OBJECT);
                         
                curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        
                struct curl_slist *headers = NULL;            
                // If we've been provided with a vclock, add it to the headers to ensure we do an update, and don't create a 
                // sibling
                if (vclock) {
                    headers = curl_slist_append(headers, vclock);   
                }            
                set_http_headers(curl, headers);
    
                if(perform_curl_and_check(curl)) {
                    new_publication = cords_publication_from_json_string(response.data);
                }
                free(transfer.data);
            }
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
    CURL *curl = init_curl_common();
    if(curl) {
        unsigned retries;
        for(retries = CURL_RETRIES; retries > 0 && NULL == retval.publication; retries--) {
            struct transfer_data data = {0};
            setup_download(curl, &data);
            
            struct transfer_data header_data = {0};
            setup_read_vclock(curl, &header_data);
            
            set_curl_query_url(curl, "publication", id, RIAK_OPTION_NO_OBJECT);
                    
            if(perform_curl_and_check(curl)) {
                retval.publication = cords_publication_from_json_string(data.data);
                retval.vclock = vclock_from_headers(header_data.data);
            }
            
            free(data.data);
            free(header_data.data);
        }
        curl_easy_cleanup(curl);
    }
    return retval;     
}

struct cords_publication * retrieve_from_id(char *id) {
    struct publication_with_vclock retrieved = retrieve_with_vclock_from_id(id);
    free(retrieved.vclock);
    return retrieved.publication;
}

cords_publication_list retrieve_from_filter(struct cords_publication_occi_filter *filter) { 
    union riak_object_list list = list_from_filter(filter, RIAK_OPTION_RETURN_OBJECT); 
    return list.objects;
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
    CURL *curl = init_curl_common();
    if(curl) {
        int success = 0;
        unsigned retries;
        for(retries = CURL_RETRIES; retries > 0 && !success; retries--) {    
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
            set_curl_query_url(curl, "publication", id, RIAK_OPTION_NO_OBJECT);                
            long code = perform_curl_and_get_code(curl);
            if (204 == code || 404 == code) {
                success = 1;
            }
        }
        curl_easy_cleanup(curl);
    }    
}

void  delete_all_matching_filter(struct cords_publication_occi_filter *filter) {
    cords_publication_id_list to_delete = list_ids(filter);
    unsigned i;
    for(i = 0; i < to_delete.count; i++) {
        del(to_delete.ids[i]);
    }
    cords_publication_free_id_list(&to_delete);
}

static char *search_query(const struct cords_publication_occi_filter *filter) {
    char buf[256];
    unsigned written = 0;
    if(filter->id) {
        if(written > 0) {
            written += sprintf(&buf[written], "%%20AND%%20");
        }
        written += sprintf(&buf[written], "id:%s", filter->attributes->id);
    }

    if(filter->operator) {
        if(written > 0) {
            written += sprintf(&buf[written], "%%20AND%%20");
        }
        written += sprintf(&buf[written], "operator:%s", filter->attributes->operator);
    }

    if(filter->identity) {
        if(written > 0) {
            written += sprintf(&buf[written], "%%20AND%%20");
        }
        written += sprintf(&buf[written], "identity:%s", filter->attributes->identity);
    }
    return allocate_string(buf);
}


static void publication_list_from_json_array(struct array_list *items, cords_publication_list *list) {
    list->publications = malloc(items->length * sizeof(char *));
    unsigned i;
    for(i = 0; i < items->length; i++) {
        struct json_object *fields;
        json_object_object_get_ex(items->array[i], "fields", &fields);
        if(fields) {
            struct cords_publication *publication = cords_publication_from_json_object(fields);            
            // This filtering means that we may have malloc'd more memory than we needed for 
            // the list.  However the object is likely to be short-lived, so we don't worry about it
            if (publication) {
                list->publications[list->count++] = publication;
            }
        }
        else {
            // It might be nice to have some error handling here.  It makes the
            // interface robust to ignore any bad objects returned, but if there
            // was a way to log it we should do so
        }
    }
}

static void publication_id_list_from_json_array(struct array_list *items, cords_publication_id_list *list) {       
    list->ids = malloc(items->length * sizeof(char *));
    unsigned i;
    for(i = 0; i < items->length; i++) {
        struct json_object *id_json;
        json_object_object_get_ex(items->array[i], "id", &id_json);
        const char *id = json_object_get_string(id_json);
        if(id) {
            list->ids[list->count++] = allocate_string(id);
        }
        else {
            // It might be nice to have some error handling here.  It makes the
            // interface robust to ignore any bad objects returned, but if there
            // was a way to log it we should do so
        }
    }
}

static void publication_list_from_search_json(struct json_object *jo, union riak_object_list *list, riak_object_return return_objects) {
    struct json_object *response;
    json_object_object_get_ex(jo, "response", &response);
    struct json_object *docs;
    json_object_object_get_ex(response, "docs", &docs);
    struct array_list *items = json_object_get_array(docs);
    if (items) {
        if (RIAK_OPTION_RETURN_OBJECT == return_objects) {
            publication_list_from_json_array(items, &list->objects);
        }
        else {
            publication_id_list_from_json_array(items, &list->ids_only);
        }
    }
}

static void publication_list_from_list_json(struct json_object *jo, cords_publication_id_list *list) {
    struct json_object *keys;
    json_object_object_get_ex(jo, "keys", &keys);
    struct array_list *items = json_object_get_array(keys);  
    if (items) {
        list->ids = malloc(items->length * sizeof(char *));
        if (list->ids) {
            int i;
            for(i = 0; i < items->length; i++) {
                const char *id = json_object_get_string(items->array[i]);
                if(id) {
                    char *new_id = allocate_string(id);
                    if(new_id) {
                        list->ids[list->count++] = new_id;
                    }
                }
                else {
                    // It might be nice to have some error handling here.  It makes the
                    // interface robust to ignore any bad objects returned, but if there
                    // was a way to log it we should do so
                }
            }
        }
    }
}

static void set_query_url(CURL *curl, unsigned n_filters, struct cords_publication_occi_filter *filter, const char *bucket) {
    char request_buffer[1024];
    char *query;
    if(0 == n_filters) {
        // List - warning, shouldn't be used in production for performance reasons
        sprintf(request_buffer, "http://devriak.market.onapp.com:10018/riak/%s?keys=true&props=false", bucket);
    }
    else {
        // Single item filter...possibly a candidate for replacing with i2 search
        query = search_query(filter);
        if(query) {
            // Here we request up to 100,000 results.  What happens if there are more than 100,000 matches?
            // Don't find out!  
            sprintf(request_buffer, "http://devriak.market.onapp.com:10018/solr/%s/select?wt=json&rows=100000&q=%s", bucket, query);
            liberate(query);
        }
    }
    curl_easy_setopt(curl, CURLOPT_URL, request_buffer);
}

static union riak_object_list list_from_curl_response(const char *response, unsigned n_filters, riak_object_return return_objects) {  
    union riak_object_list retVal = {0}; 
    // In order to reduce nesting here, we're relying on the fact that json-c behaves nicely
    // when passed null pointers.  We don't need to check the return values from json-c calls
    // if all we're doing is passing them back to json-c functions.
    struct json_object *jo = json_tokener_parse(response);
    if(0 == n_filters) {                
        // In production, may want to ban this (assert(0) or warning), since 
        // list all should be strongly discouraged
        cords_publication_id_list ids = {0};
        publication_list_from_list_json(jo, &ids);
        if (RIAK_OPTION_RETURN_OBJECT == return_objects) {
            retVal.objects.publications = malloc(ids.count * sizeof(struct cords_publication *));
            unsigned i;
            for(i = 0; i < ids.count; i++) {
                struct cords_publication *publication = retrieve_from_id(ids.ids[i]);
                if (publication) {
                    retVal.objects.publications[retVal.objects.count++] = publication;
                }
            }
            cords_publication_free_id_list(&ids);
        }
        else {
            retVal.ids_only = ids;
        }
    }
    else {
        publication_list_from_search_json(jo, &retVal, return_objects);
    }
    json_object_put(jo);
    return retVal;
}

union riak_object_list list_from_filter(struct cords_publication_occi_filter *filter, riak_object_return return_objects) { 
    union riak_object_list retVal = {0};
    CURL *curl = init_curl_common();
    if(curl) {
        int success = 0;
        unsigned retries;
        for(retries = CURL_RETRIES; retries > 0 && !success; retries--) {
            unsigned n_filters = cords_publication_count_filters(filter);
            set_query_url(curl, n_filters, filter, "publication");
            
            struct transfer_data response = {0};
            setup_download(curl, &response);
    
            if(perform_curl_and_check(curl)) {
                success = 1;  // If we got data back from curl, we don't want to retry, even if parsing the
                              // data is unsuccessful
                retVal = list_from_curl_response(response.data, n_filters, return_objects);                            
            }       
            free(response.data);
        }
    }
    curl_easy_cleanup(curl);    
    return retVal; 
}

cords_publication_id_list list_ids(struct cords_publication_occi_filter *filter) { 
    union riak_object_list list = list_from_filter(filter, RIAK_OPTION_NO_OBJECT); 
    return list.ids_only;
}
