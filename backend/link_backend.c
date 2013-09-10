#include <assert.h>

#include "occi.h"

#include "xlink_occi_filter.h"
#include "xlink.h"
#include "xlink_backend_interface.h"

#include "link_backend.h"

static xlink_list list_of_links;
static int link_index = 0; 

static void initialise_links() {
    // From the usage, it appears that only a single thread should be accessing these functions at any time.
    // If it's attempted to initialise the list while the list is still being parsed, that indicates a
    // problem which will require locking to solve.
    assert(0 == link_index);
    assert(0 == list_of_links.count);
    
    assert(cords_xlink_backend);
    assert(cords_xlink_backend->retrieve_from_filter);
    // TODO We don't do any actual filtering here, because currently it's not possible to do the 
    // right sort of filtering, i.e. filter on partial match for source id.
    // Once the backend supports it, or we change the filter so a complete match is possible, we can
    // update this code.
    struct cords_xlink_occi_filter filter;
    memset(&filter, 0, sizeof(struct cords_xlink_occi_filter));
    filter.attributes = allocate_cords_xlink();
    list_of_links = cords_xlink_backend->retrieve_from_filter(&filter);
    liberate_cords_xlink(filter.attributes);
}

const struct cords_xlink *initialise_and_get_first_link(const char *source_id) {
    initialise_links();
    return next_link(source_id);
}

const struct cords_xlink *initialise_and_get_last_link(const char *source_id) {
    initialise_links();
    if (list_of_links.count > 0) {
        link_index = list_of_links.count - 1;
    }
    return previous_link(source_id);
}

static const struct cords_xlink *valid_current_link(const char *source_id) {    
    const struct cords_xlink *found, *current;
    current = list_of_links.xlinks[link_index];
    found = NULL;
    if (current && current->source && current->target) {
        char *wptr = occi_category_id( current->source );
        if (wptr) {
            if ( 0 == strcmp( wptr, source_id )) {
                found = current;
            }
            liberate( wptr );
        }
    }
    return found;
}

static const struct cords_xlink *check_found(const struct cords_xlink *found) { 
    if (!found) {
        link_index = 0;
        free_xlink_list(&list_of_links);
    }
    return found;
}

const struct cords_xlink *next_link(const char *source_id) {    
    const struct cords_xlink *found;
    
    for (found = NULL; !found && link_index < list_of_links.count; link_index++) {
        found = valid_current_link(source_id);
    }
        
    return check_found(found);
}

const struct cords_xlink *previous_link(const char *source_id) {    
    const struct cords_xlink *found;
    
    for (found = NULL; !found && link_index > 0; link_index--) {
        found = valid_current_link(source_id);
    }
        
    return check_found(found);
}
