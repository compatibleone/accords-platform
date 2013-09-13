
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
#include "riak_backend.h"
#include "publication_riak_backend.h"

union riak_object_list {
    cords_publication_id_list ids_only;
    cords_publication_list objects;
};

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
static union riak_object_list list_from_filter(struct cords_publication_occi_filter *filter, riak_object_return return_objects);

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
    enable_riak_search("publication");
}

void finalise() {
    curl_global_cleanup();
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
            assert(0); // Out of memory!
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
                headers = set_http_headers(curl, headers);
    
                if(perform_curl_and_check(curl, headers)) {
                    new_publication = cords_publication_from_json_string(response.data);
                }
                free(transfer.data);
                free(response.data);
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
            free(retval.vclock);  // Just in case we're in a retry and it was allocated
            struct transfer_data data = {0};
            setup_download(curl, &data);
            
            struct transfer_data header_data = {0};
            setup_read_vclock(curl, &header_data);
            
            set_curl_query_url(curl, "publication", id, RIAK_OPTION_NO_OBJECT);
                    
            if(perform_curl_and_check(curl, NULL)) {
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
        liberate_cords_publication(create_or_update(updated_publication, retrieved.vclock));
        free(retrieved.vclock);
        liberate_cords_publication(retrieved.publication);
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
            set_curl_query_url(curl, "publication", id, RIAK_OPTION_NO_OBJECT);                
            long code = perform_curl_and_get_code(curl, NULL);
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
    assert(filter);
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
    assert(written > 0); // Shouldn't get here unless at least one filter was set
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
                else {
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
            char *query = NULL;
            if(n_filters) {
                query = search_query(filter);
            }
            set_search_url(curl, n_filters, "publication", query);
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

cords_publication_id_list list_ids(struct cords_publication_occi_filter *filter) { 
    union riak_object_list list = list_from_filter(filter, RIAK_OPTION_NO_OBJECT); 
    return list.ids_only;
}
