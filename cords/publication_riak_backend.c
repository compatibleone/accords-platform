
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "publication_occi_filter.h"
#include "publication.h"
#include "publication_backend_interface.h"
#include "publication_riak_backend.h"

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

void init() {}
void finalise() {}
struct cords_publication * create  (struct cords_publication *initial_publication) { return NULL; }
struct cords_publication * retrieve_from_id(char *id) { return NULL; }
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
