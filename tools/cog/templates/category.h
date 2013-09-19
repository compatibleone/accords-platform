[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file); t.category_h_guard() ]]]
[[[end]]]

[[[cog t.category_h_struct() ]]]
[[[end]]]
{
	[[[cog t.category_h_members() ]]]
	[[[end]]]
};

typedef struct  CATEGORY_NAME_id_list_t
{
    int count;
    char **ids;
}  CATEGORY_NAME_id_list;

typedef struct CATEGORY_NAME_list_t
{
    int count;
    struct CATEGORY_NAME **FILENAME_ROOTs;
} CATEGORY_NAME_list;

struct CATEGORY_NAME * allocate_CATEGORY_NAME();
struct CATEGORY_NAME * liberate_CATEGORY_NAME(struct CATEGORY_NAME * optr);
struct CATEGORY_NAME * reset_CATEGORY_NAME(struct CATEGORY_NAME * sptr);

void CATEGORY_NAME_free_id_list( CATEGORY_NAME_id_list *list);
void free_CATEGORY_NAME_list(CATEGORY_NAME_list *list);

#endif
