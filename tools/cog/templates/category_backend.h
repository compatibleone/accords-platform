#ifndef _publication_backend_h
#define _publication_backend_h

typedef struct id_list_t
{
	int count;
	char **ids;
} id_list;

typedef struct publication_list_t
{
	int count;
	struct cords_publication **publications;
} publication_list;

struct	publication_backend_interface
{
	void (*init)();
	void (*finalise)();
	struct cords_publication *(*create)  (int allocate_uuid, struct cords_publication *initial_publication);
	struct cords_publication *(*retrieve_from_id)(char *id);
	publication_list (*retrieve_from_filter)(struct cords_publication *filter);
	void (*update)  (char *id, struct cords_publication *updated_publication);
	void (*del)     (char *id);
	void (*delete_all_matching_filter) (struct cords_publication *filter);	// Delete all publications matching filter
	id_list (*list) (struct cords_publication *filter); // Return all ids of publications matching filter
};

void free_id_list(id_list *list);
void free_publication_list(publication_list *list);

#endif
