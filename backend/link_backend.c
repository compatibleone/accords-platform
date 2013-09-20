#include <assert.h>

#include "occi.h"

#include "xlink_occi_filter.h"
#include "xlink.h"
#include "xlink_backend_interface.h"

#include "link_backend.h"

static cords_xlink_list list_of_links;
static int link_index = 0; 

static void initialise_links(const char *source_id) {
    // From the usage, it appears that only a single thread should be accessing these functions at any time.
    // If it's attempted to initialise the list while the list is still being parsed, that indicates a
    // problem which will require locking to solve.
    assert(0 == link_index);
    assert(0 == list_of_links.count);
    
    assert(cords_xlink_backend);
    assert(cords_xlink_backend->retrieve_from_filter);
    struct cords_xlink_occi_filter filter = {0};
    filter.attributes = allocate_cords_xlink();
    if(!filter.attributes) {
        assert(0);
        return;
    }
    filter.source_id = 1;
    filter.attributes->source_id = allocate_string(source_id);
    list_of_links = cords_xlink_backend->retrieve_from_filter(&filter);
    liberate_cords_xlink(filter.attributes);
}

const struct cords_xlink *initialise_and_get_first_link(const char *source_id) {
    initialise_links(source_id);
    return next_link();
}

const struct cords_xlink *initialise_and_get_last_link(const char *source_id) {
    initialise_links(source_id);
    if (list_of_links.count > 0) {
        link_index = list_of_links.count - 1;
    }
    return previous_link();
}

static const struct cords_xlink *valid_current_link() {    
    const struct cords_xlink *current;
    current = list_of_links.xlinks[link_index];
    if (current && current->source && current->target) {
        return current;
    }
    return NULL;
}

static const struct cords_xlink *check_found(const struct cords_xlink *found) { 
    if (!found) {
        link_index = 0;
        free_cords_xlink_list(&list_of_links);
    }
    return found;
}

const struct cords_xlink *next_link() {    
    const struct cords_xlink *found;
    
    for (found = NULL; !found && link_index < list_of_links.count; link_index++) {
        found = valid_current_link();
    }
        
    return check_found(found);
}

const struct cords_xlink *previous_link() {    
    const struct cords_xlink *found;
    
    for (found = NULL; !found && link_index > 0; link_index--) {
        found = valid_current_link();
    }
        
    return check_found(found);
}

void update_source_id(struct cords_xlink *link) {
    link->source_id = occi_category_id(link->source);   
}
