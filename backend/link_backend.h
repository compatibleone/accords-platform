#ifndef _link_backend_h
#define _link_backend_h

const struct cords_xlink *initialise_links_list(const char *source_id);
const struct cords_xlink *next_link(const char *source_id);

#endif