[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file)]]]
[[[end]]]
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <curl/curl.h>
#include <json.h>

#include "allocate.h"

#include "FILENAME_ROOT_occi_filter.h"
#include "FILENAME_ROOT.h"
#include "FILENAME_ROOT_backend_interface.h"
#include "riak_backend.h"
#include "FILENAME_ROOT_riak_backend.h"

union riak_object_list {
    CATEGORY_NAME_id_list ids_only;
    CATEGORY_NAME_list objects;
};

// API Functions
static void init();
static void finalise();
static struct CATEGORY_NAME *create(struct CATEGORY_NAME *initial_FILENAME_ROOT);
static struct CATEGORY_NAME *retrieve_from_id(char *id);
static CATEGORY_NAME_list retrieve_from_filter(struct CATEGORY_NAME_occi_filter *filter);
static void update(char *id, struct CATEGORY_NAME *updated_FILENAME_ROOT);
static void del(char *id);
static void delete_all_matching_filter(struct CATEGORY_NAME_occi_filter *filter);
static CATEGORY_NAME_id_list list_ids(struct CATEGORY_NAME_occi_filter *filter);

// Local Functions
static union riak_object_list list_from_filter(struct CATEGORY_NAME_occi_filter *filter, riak_object_return return_objects);

struct FILENAME_ROOT_backend_interface *  CATEGORY_NAME_riak_backend_interface() {
    struct FILENAME_ROOT_backend_interface riak_interface =
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
    struct FILENAME_ROOT_backend_interface *interface_ptr =
            (struct FILENAME_ROOT_backend_interface *) malloc (sizeof(struct FILENAME_ROOT_backend_interface));
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
    enable_riak_search("FILENAME_ROOT");
}

void finalise() {
    curl_global_cleanup();
}

static char *json_string(const struct CATEGORY_NAME *FILENAME_ROOT) {
    struct json_object *jo = json_object_new_object();
    
    [[[cog t.json_from_category()]]]
    [[[end]]]
    
    char *retVal = allocate_string(json_object_to_json_string(jo));
    
    // Free the object
    json_object_put(jo);
    
    return retVal;
}

static struct CATEGORY_NAME *CATEGORY_NAME_from_json_object(struct json_object *jo) {
    json_bool success;
    
    struct json_object *id;
    success = json_object_object_get_ex(jo, "id", &id);
    if (success) {
        struct CATEGORY_NAME *new_FILENAME_ROOT = allocate_CATEGORY_NAME();
        if (!new_FILENAME_ROOT) {
            assert(0); // Out of memory!
            return NULL;
        }
        new_FILENAME_ROOT->id = allocate_string(json_object_get_string(id));
        
        [[[cog t.category_from_json()]]]
        [[[end]]]
         
        return new_FILENAME_ROOT;
    }
    return NULL;
}

static struct CATEGORY_NAME *CATEGORY_NAME_from_json_string(const char *input) {
    struct json_object *jo = json_tokener_parse(input);            
    struct CATEGORY_NAME *new_FILENAME_ROOT = CATEGORY_NAME_from_json_object(jo);    
    json_object_put(jo); // Free the object
    return new_FILENAME_ROOT;
}

static struct CATEGORY_NAME *create_or_update(const struct CATEGORY_NAME *initial_FILENAME_ROOT, const char *vclock) {
    CURL *curl;
    struct CATEGORY_NAME *new_FILENAME_ROOT = NULL;    
    curl = init_curl_common();
    if(curl) {
        unsigned retries;
        for(retries = CURL_RETRIES; retries > 0 && NULL == new_FILENAME_ROOT; retries--) {
            char *data = json_string(initial_FILENAME_ROOT);
            if(data) {
                struct transfer_data transfer;
                register_upload_data(curl, &transfer, data);
                
                struct transfer_data response = {0};
                setup_download(curl, &response);
                
                set_curl_query_url(curl, "FILENAME_ROOT", initial_FILENAME_ROOT->id, RIAK_OPTION_RETURN_OBJECT);
                         
                curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        
                struct curl_slist *headers = NULL;            
                // If we've been provided with a vclock, add it to the headers to ensure we do an update, and don't create a 
                // sibling
                if (vclock) {
                    headers = curl_slist_append(headers, vclock);   
                }            
                headers = set_http_headers(curl, headers);
    
                if(perform_curl_and_check(curl, headers)) {
                    new_FILENAME_ROOT = CATEGORY_NAME_from_json_string(response.data);
                }
                free(transfer.data);
                free(response.data);
            }
        }
        curl_easy_cleanup(curl);
    }
    return new_FILENAME_ROOT;    
}

struct CATEGORY_NAME *create(struct CATEGORY_NAME *initial_FILENAME_ROOT) {
    return create_or_update(initial_FILENAME_ROOT, NULL);
}

struct FILENAME_ROOT_with_vclock {
    char *vclock;
    struct CATEGORY_NAME *FILENAME_ROOT;
};

static struct FILENAME_ROOT_with_vclock retrieve_with_vclock_from_id(const char *id) {
    struct FILENAME_ROOT_with_vclock retval = {0};    
    CURL *curl = init_curl_common();
    if(curl) {
        unsigned retries;
        for(retries = CURL_RETRIES; retries > 0 && NULL == retval.FILENAME_ROOT; retries--) {
            free(retval.vclock);  // Just in case we're in a retry and it was allocated
            struct transfer_data data = {0};
            setup_download(curl, &data);
            
            struct transfer_data header_data = {0};
            setup_read_vclock(curl, &header_data);
            
            set_curl_query_url(curl, "FILENAME_ROOT", id, RIAK_OPTION_NO_OBJECT);
                    
            if(perform_curl_and_check(curl, NULL)) {
                retval.FILENAME_ROOT = CATEGORY_NAME_from_json_string(data.data);
                retval.vclock = vclock_from_headers(header_data.data);
            }
            
            free(data.data);
            free(header_data.data);
        }
        curl_easy_cleanup(curl);
    }
    return retval;     
}

struct CATEGORY_NAME * retrieve_from_id(char *id) {
    struct FILENAME_ROOT_with_vclock retrieved = retrieve_with_vclock_from_id(id);
    free(retrieved.vclock);
    return retrieved.FILENAME_ROOT;
}

CATEGORY_NAME_list retrieve_from_filter(struct CATEGORY_NAME_occi_filter *filter) { 
    union riak_object_list list = list_from_filter(filter, RIAK_OPTION_RETURN_OBJECT); 
    return list.objects;
}

// Riak has a vclock recorded for each object stored which is used for assisting with
// consistency in the face of different database nodes having different versions.
// In order to ensure Riak updates the correct node, we must first read it and 
// use the vclock returned when sending our update.
void update(char *id, struct CATEGORY_NAME *updated_FILENAME_ROOT) { 
    struct FILENAME_ROOT_with_vclock retrieved = retrieve_with_vclock_from_id(id);
    if (retrieved.vclock && retrieved.FILENAME_ROOT) {
        liberate_CATEGORY_NAME(create_or_update(updated_FILENAME_ROOT, retrieved.vclock));
        free(retrieved.vclock);
        liberate_CATEGORY_NAME(retrieved.FILENAME_ROOT);
    }
    else {
        assert(0); // Unexpected failure to retrieve original object
    }
}

void del(char *id) {
    CURL *curl = init_curl_common();
    if(curl) {
        int success = 0;
        unsigned retries;
        for(retries = CURL_RETRIES; retries > 0 && !success; retries--) {    
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
            set_curl_query_url(curl, "FILENAME_ROOT", id, RIAK_OPTION_NO_OBJECT);                
            long code = perform_curl_and_get_code(curl, NULL);
            if (204 == code || 404 == code) {
                success = 1;
            }
        }
        curl_easy_cleanup(curl);
    }    
}

void  delete_all_matching_filter(struct CATEGORY_NAME_occi_filter *filter) {
    CATEGORY_NAME_id_list to_delete = list_ids(filter);
    unsigned i;
    for(i = 0; i < to_delete.count; i++) {
        del(to_delete.ids[i]);
    }
    CATEGORY_NAME_free_id_list(&to_delete);
}

static char *search_query(const struct CATEGORY_NAME_occi_filter *filter) {
    assert(filter);
    char buf[256];
    unsigned written = 0;
    [[[cog t.riak_query_from_filter()]]]
    [[[end]]]
    assert(written > 0); // Shouldn't get here unless at least one filter was set
    return allocate_string(buf);
}


static void FILENAME_ROOT_list_from_json_array(struct array_list *items, CATEGORY_NAME_list *list) {
    list->FILENAME_ROOTs = malloc(items->length * sizeof(char *));
    unsigned i;
    for(i = 0; i < items->length; i++) {
        struct json_object *fields;
        json_object_object_get_ex(items->array[i], "fields", &fields);
        if(fields) {
            struct CATEGORY_NAME *FILENAME_ROOT = CATEGORY_NAME_from_json_object(fields);            
            // This filtering means that we may have malloc'd more memory than we needed for 
            // the list.  However the object is likely to be short-lived, so we don't worry about it
            if (FILENAME_ROOT) {
                list->FILENAME_ROOTs[list->count++] = FILENAME_ROOT;
            }
            else {
            }
        }
        else {
            // It might be nice to have some error handling here.  It makes the
            // interface robust to ignore any bad objects returned, but if there
            // was a way to log it we should do so
        }
    }
}

static void FILENAME_ROOT_id_list_from_json_array(struct array_list *items, CATEGORY_NAME_id_list *list) {       
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

static void FILENAME_ROOT_list_from_search_json(struct json_object *jo, union riak_object_list *list, riak_object_return return_objects) {
    struct json_object *response;
    json_object_object_get_ex(jo, "response", &response);
    struct json_object *docs;
    json_object_object_get_ex(response, "docs", &docs);
    struct array_list *items = json_object_get_array(docs);
    if (items) {
        if (RIAK_OPTION_RETURN_OBJECT == return_objects) {
            FILENAME_ROOT_list_from_json_array(items, &list->objects);
        }
        else {
            FILENAME_ROOT_id_list_from_json_array(items, &list->ids_only);
        }
    }
}

static void FILENAME_ROOT_list_from_list_json(struct json_object *jo, CATEGORY_NAME_id_list *list) {
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
                    else {
                        assert(0); // Out of memory
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

static union riak_object_list list_from_curl_response(const char *response, unsigned n_filters, riak_object_return return_objects) {  
    union riak_object_list retVal = {0}; 
    // In order to reduce nesting here, we're relying on the fact that json-c behaves nicely
    // when passed null pointers.  We don't need to check the return values from json-c calls
    // if all we're doing is passing them back to json-c functions.
    struct json_object *jo = json_tokener_parse(response);
    if(0 == n_filters) {                
        // In production, may want to ban this (assert(0) or warning), since 
        // list all should be strongly discouraged
        CATEGORY_NAME_id_list ids = {0};
        FILENAME_ROOT_list_from_list_json(jo, &ids);
        if (RIAK_OPTION_RETURN_OBJECT == return_objects) {
            retVal.objects.FILENAME_ROOTs = malloc(ids.count * sizeof(struct CATEGORY_NAME *));
            unsigned i;
            for(i = 0; i < ids.count; i++) {
                struct CATEGORY_NAME *FILENAME_ROOT = retrieve_from_id(ids.ids[i]);
                if (FILENAME_ROOT) {
                    retVal.objects.FILENAME_ROOTs[retVal.objects.count++] = FILENAME_ROOT;
                }
                else {
                }
            }
            CATEGORY_NAME_free_id_list(&ids);
        }
        else {
            retVal.ids_only = ids;
        }
    }
    else {
        FILENAME_ROOT_list_from_search_json(jo, &retVal, return_objects);
    }
    json_object_put(jo);
    return retVal;
}

union riak_object_list list_from_filter(struct CATEGORY_NAME_occi_filter *filter, riak_object_return return_objects) { 
    union riak_object_list retVal = {0};
    CURL *curl = init_curl_common();
    if(curl) {
        int success = 0;
        unsigned retries;
        for(retries = CURL_RETRIES; retries > 0 && !success; retries--) {
            unsigned n_filters = CATEGORY_NAME_count_filters(filter);
            char *query = NULL;
            if(n_filters) {
                query = search_query(filter);
            }
            set_search_url(curl, n_filters, "FILENAME_ROOT", query);
            liberate(query);            
            
            struct transfer_data response = {0};
            setup_download(curl, &response);
    
            if(perform_curl_and_check(curl, NULL)) {
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

CATEGORY_NAME_id_list list_ids(struct CATEGORY_NAME_occi_filter *filter) { 
    union riak_object_list list = list_from_filter(filter, RIAK_OPTION_NO_OBJECT); 
    return list.ids_only;
}
