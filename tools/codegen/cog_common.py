import os

import cog

from codegen import parse_and_generate_code as parse
from codegen import category_for_file

import codegen_types.ctypes as ctypes

# Example invocations from tools directiory:
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=publication.h -D model_dir="../model" -o tmp/output.xsd cog/templates/schema.xsd
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=publication.h -D model_dir='../model' -o tmp/publication.h cog/templates/category.h 
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=publication.h -D model_dir='../model' -o tmp/publication_node_backend.h cog/templates/node_backend.h
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=publication.h -D model_dir='../model' -o tmp/publication_node_backend.c cog/templates/node_backend.c
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=publication.h -D model_dir='../model' -o tmp/occi_publication_rest.c cog/templates/occi.c  
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=publication.h -D model_dir='../model' -o tmp/publication_backend_interface.h cog/templates/category_backend_interface.h
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=publication.h -D model_dir='../model' -o tmp/publication_occi_filter.h cog/templates/category_filter.h
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=publication.h -D model_dir='../model' -o tmp/publication.c cog/templates/category.c      
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=publication.h -D model_dir='../model' -o tmp/occipublication.c cog/templates/occicategory_unsplit.c


# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/occiauthitem.c cog/templates/occi.c  
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=authitem.h -D model_dir="../model" -o tmp/authitem.xsd cog/templates/schema.xsd
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/authitem.h cog/templates/category.h 
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/authitem_node_backend.h cog/templates/node_backend.h
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/authitem_node_backend.c cog/templates/node_backend.c
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/authiftem_backend.h cog/templates/category_backend.h
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/authitem_occi_filter.h cog/templates/category_filter.h
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/occiauthitem.c cog/templates/occicategory_unsplit.c 
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/authitem.c cog/templates/category.c

# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=xlink.h -D model_dir='../model' -o tmp/occixlink.c cog/templates/occicategory_unsplit.c

# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=azconfig.h -D model_dir='../model' -o tmp/azconfig.h cog/templates/category.h 
# python cog/3rdParty/cogapp/__main__.py -ed -I . -D cog_category_file=azconfig.h -D model_dir='../model' -o tmp/azconfig_node_backend.c cog/templates/node_backend.c

def _link_special_case():
    return _category_name() == "cords_xlink"

def parse_models(model_dir):
    global models
    # Not re-parsing the models each time gives a massive speed-up
    if 'models' not in globals():
        print "Parsing models..."
        models = parse([model_dir], None, None, None)
        print "...done"    

def init_models(model_dir, filename):
    global models
    global category_file
    global category
    global _has_name
    parse_models(model_dir)
    category_file = filename
    category = category_for_file(category_file, models)
    _has_name = False
    for name, _ in category.backend_type_list():
        if name == "name":
            _has_name = True
            break

def _type_conversion(type_name):
    if type_name == "char *":
        return "string"
    if type_name == "int":
        return "integer"
    return "string"

def _category_name():
    return category.struct_name 

def _filename_root():
    return _name_root(category_file)

def category_name():
    cog.inline()
    cog.out(_category_name())   
    
def split_category_name():
    cog.inline()
    split_name = ' '.join(_category_name())
    cog.out(split_name)

def category_xsd(): 
    cog.outl('<xsd:complexType name="{0}">'.format(category.struct_name))
    for name, type_name in category.backend_type_list():
        cog.outl(r'    <xsd:attribute name="{0}" type="xsd:{1}"/>'.format(name, _type_conversion(type_name)))


def node_h_guard():
    _generic_h_guard('{0}_node_backend'.format(category.struct_name))
    
def category_h_guard():
    _generic_h_guard(category.struct_name)
    
def _name_root(filename):
    filename_root, _ = os.path.splitext(filename)
    return filename_root

def filename_root():
    cog.inline()
    cog.out(_filename_root())

def _generic_h_guard(filename):
    guard = '_{0}_h'.format(_name_root(filename))
    
    cog.outl('#ifndef {0}'.format(guard))    
    cog.outl('#define {0}'.format(guard))
    
def category_h_struct():
    cog.outl('struct {0}'.format(category.struct_name)) 
        
def category_h_members():
    try:
        for name, type_name in category.backend_type_list():
            cog.outl('{0} {1};'.format(type_name, name))
    except ValueError:
        # We know more details here than in ctypes.from_platform type, so catch and add details
        raise(ValueError('Unexpected type {0} for member "{1}" of category "{2}"'.format(type_name, name, _category_name())))

def category_filters():
    cog.outl('struct {0} *attributes;'.format(_category_name()))
    for name, _ in category.backend_type_list():
        cog.outl('int {0};'.format(name))
    
def pass_category_filter():
    category_name = _category_name()
    _format_category(None,                      
                     ["    if (filter->{0}) {{",
                      "        if(!( pptr->{0} ))",
                      "            return(0);",
                      "        else if ( strcmp(pptr->{0},filter->attributes->{0}) != 0)",
                      "            return(0);",
                      "    }}"],
                      "    if ( filter->{0} && ( pptr->{0} != filter->attributes->{0} )) return(0);")                 
        
def load_attributes():
    _format_category("if ((aptr = document_atribut( vptr, \"{0}\" )) != (struct xml_atribut *) 0)",
                     "    {1}->{0} = document_atribut_string(aptr);",
                     "    {1}->{0} = document_atribut_value(aptr);")
                 
def save_attributes():
    _format_category("fprintf(h,\" {0}=%c\",0x0022);",
                     "fprintf_xml_string_attribute(h,\"%s\",(pptr->{0}?pptr->{0}:\"\"));",
                     "fprintf(h,\"%u\",pptr->{0});",
                     "fprintf(h,\"%c\",0x0022);")
        
def clone():
    _format_category("if (original->{0}) {{",
                     "    success = success && (copy->{0} = allocate_string(original->{0}));",
                     "    copy->{0} = original->{0};",
                     "}}")                    

def set_field():
    _format_category("if (!( strcmp( field_name, \"{0}\" ) ))",
                     "    pptr->{0} = allocate_string(value);",
                     "    pptr->{0} = atoi(value);",
                     include_id = False) 
    
def activate_filter():
    _format_category(["if (!( strcmp( field_name, \"{0}\" ) ))",
                      "    target->{0} = 1;"],
                     include_id = False)      
    
def occi_response():
    _format_category(None,
                     "sprintf(cptr->buffer,\"%s.%s.{0}=%c%s%c\",optr->domain,optr->id,0x0022,pptr->{0},0x0022);",
                     "sprintf(cptr->buffer,\"%s.%s.{0}=%c%u%c\",optr->domain,optr->id,0x0022,pptr->{0},0x0022);",
                     ["if (!( hptr = rest_response_header( aptr, \"X-OCCI-Attribute\",cptr->buffer) ))",
                     "    return(internal_failure_response(aptr));"],
                     include_id = False)    
    
def occi_builder():
    _format_category(None,
                     "if (!( optr = occi_add_attribute(optr, \"{0}\",0,0) ))",                     
                     "if (!( optr = occi_add_attribute(optr, \"{0}\",0,0) ))",
                     "    return(optr);",
                     include_id = False)
    if not _link_special_case():
        cog.out( 
"""if (!( optr = occi_add_action( optr,"DELETE","",delete_action_{0})))
    return( optr );""".format(_category_name()))
    
def occi_headers():
    _format_category(["if (!( hptr = allocate_rest_header()))",
                      "    return(first);",
                      "else    if (!( hptr->previous = last))",
                      "    first = hptr;",
                      "else    hptr->previous->next = hptr;",
                      "last = hptr;",
                      "if (!( hptr->name = allocate_string(\"X-OCCI-Attribute\")))",
                      "    return(first);"],
                     "sprintf(buffer,\"occi.{2}.{0}='%s'\\r\\n\",(sptr->{0}?sptr->{0}:\"\"));",    
                     "sprintf(buffer,\"occi.{2}.{0}='%u'\\r\\n\",sptr->{0});",
                     ["if (!( hptr->value = allocate_string(buffer)))",
                      "    return(first); "],
                     include_id = False)        

def liberate():
    _format_category(None,
                     ["if ( sptr->{0} )",
                     "    sptr->{0} = liberate(sptr->{0});"])
    
def reset():
#     _format_category(None,
#                      "sptr->{0} = (char*) 0;",
#                      "sptr->{0} =  0;",
#                      skip_legacy_types = False)
    
    for name, type_name in category.backend_type_list(skip_legacy_types = False):
        if type_name == "int":
            cog.outl("sptr->{0} = 0;".format(name)) # It would actually be harmless to cast to int.  However for compatibility with Strukt, and ease of comparing output, we suppress it
        else:
            cog.outl("sptr->{0} = ({1}) 0;".format(name, type_name))
        
        
        
def xmlin():
    _format_category(["else if (!( strcmp(wptr->name,\"{0}\") ))",
                      "{{"],
                     "    if ( wptr->value ) {{ sptr->{0} = allocate_string(wptr->value); }}",
                     "    if ( wptr->value ) {{ sptr->{0} = atoi(wptr->value); }}",
                     "}}",
                     include_id = False)
    
def rest_print():
    _format_category(None,
                     "fprintf(fh,\"X-OCCI-Attribute: %s.%s.{0}='%s'\\r\\n\",prefix,nptr,(sptr->{0}?sptr->{0}:\"\"));",
                     "fprintf(fh,\"X-OCCI-Attribute: %s.%s.{0}='%u'\\r\\n\",prefix,nptr,sptr->{0});")
    
def json_from_category():
    _format_category(None,
                     ["if({1}->{0}) {{",
                     "    json_object_object_add(jo, \"{0}\", json_object_new_string({1}->{0}));",
                     "}}"],
                     "json_object_object_add(jo, \"{0}\", json_object_new_int64({1}->{0}));")
                    
def category_from_json():
    _format_category(["struct json_object *{0};",
                      "success = json_object_object_get_ex(jo, \"{0}\", &{0});",
                      "if (success) {{"],
                     "    new_{1}->{0} = allocate_string(json_object_get_string({0}));",
                     "    new_{1}->{0} = json_object_get_int64({0});",
                     "}}",
                     include_id = False) 

def riak_query_from_filter():
    _format_category(["if(filter->{0}) {{",
                      "    if(written > 0) {{",
                      "        written += sprintf(&buf[written], \"%%20AND%%20\");",
                      "    }}"],
                     "    written += sprintf(&buf[written], \"{0}:%s\", filter->attributes->{0});",
                     "    written += sprintf(&buf[written], \"{0}:%d\", filter->attributes->{0});",
                     "}}")                                           
    
def _format_category(prefix = None, string_format = None, int_format = None, suffix = None, include_id = True):
    for name, type_name in category.backend_type_list(include_id, skip_legacy_types = True):
        _output_lines(prefix, name)
        if (type_name == "char *"):
            _output_lines(string_format, name, _filename_root(), _category_name())
        elif(type_name == "int"):
            _output_lines(int_format, name, _filename_root(), _category_name())
        else:
            raise(ValueError('Unexpected type {0} for member "{1}" of category "{2}"'.format(type_name, name, _category_name())))
        _output_lines(suffix)
    
def _output_lines(lines, *args):       
    if lines:
        # Lines can be a single line, or an array of lines.
        if not isinstance(lines, basestring):
            for line in lines:
                cog.outl(line.format(*args))
        else:
            cog.outl(lines.format(*args))
        
def node_type():
    cog.inline()
    if _link_special_case():
        cog.out("link")
    else:
        cog.out("kind")
        
def node_category():
    cog.inline()
    if _link_special_case():
        cog.out("link")
    else:
        cog.out(_category_name())
    
def post_actions():    
    if not _link_special_case():
        cog.out(
"""else if (!( strncmp( rptr->parameters, "action=", strlen("action=")) ))
    return({0}_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
else if (!( strncmp( rptr->parameters, "mixin=", strlen("mixin=")) ))
    return({0}_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
else if (!( strncmp( rptr->parameters, "link=", strlen("link=")) ))
    return({0}_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );""".format(_category_name()))
        
def scheme_kind():
    cog.inline()
    if _link_special_case():
        cog.out("link")
    else:
        cog.out("resource")
        
def crud_delete_action():
    if not _link_special_case():
        cog.out(
"""/*    ------------------------------------    */
/*    c r u d   d e l e t e   a c t i o n     */
/*    ------------------------------------    */
private struct rest_response * delete_action_{0}(struct occi_category * optr, 
struct rest_client * cptr,  
struct rest_request * rptr,  
struct rest_response * aptr,  
void * vptr )
{{
    aptr = liberate_rest_response( aptr );
    return( occi_{0}_delete(optr,cptr,rptr));
}}
""".format(_category_name()))

def post_mixin_and_action():
    if not _link_special_case():
        cog.out(
"""/*    --------------------------------------------------------------------------------------------    */
/*    o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n     */
/*    --------------------------------------------------------------------------------------------    */
private struct rest_response * {0}_post_mixin(
    struct occi_category * optr, struct rest_client * cptr,
    struct rest_request * rptr, struct rest_response * aptr,char * id)
{{
    return ({0}_bad_request_response(aptr, id));
}}

/*    ----------------------------------------------------------------------------------------------    */
/*    o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n     */
/*    ----------------------------------------------------------------------------------------------    */
private struct rest_response * {0}_post_action(
    struct occi_category * optr, struct rest_client * cptr,
    struct rest_request * rptr, struct rest_response * aptr,char * id)
{{
    struct rest_header * hptr;
    struct occi_interface * iptr;
    struct occi_action * fptr;
    struct {0} * {1};
    char * reqhost;
    char * mptr;
    if (!( {1} = {0}_backend->retrieve_from_id(id) ))
        return( not_found_html_response(aptr) );
    mptr = (rptr->parameters+strlen("action="));
    for ( fptr=optr->firstact;
        fptr != (struct occi_action *) 0;
        fptr = fptr->next )
        if (!( strncmp( mptr, fptr->name, strlen( fptr->name )) )) {{
            struct rest_response *retVal = occi_invoke_action(fptr,optr,cptr,rptr,aptr,{1});
            {0}_backend->update(id, {1});
            liberate_{0}({1});
            return(retVal);
        }}
    liberate_{0}({1});
    return( rest_html_response( aptr, 400, "Incorrect Action Request"));
}}
""".format(_category_name(), _filename_root()))
        
def filter_by_name_header():
    if _has_name:
        cog.outl("struct {0} *(*retrieve_from_name)(const char *name);".format(_category_name()))
        
def retrieve_from_name_declaration():
    if _has_name:
        cog.outl("private struct {0} * {0}_retrieve_from_name(const char *name);".format(_category_name()))
        
def retrieve_from_name_add_to_struct():
    if _has_name:
        cog.outl("{0}_retrieve_from_name }};".format(_category_name()))
    else:
        cog.outl("};")
        
def retrieve_from_name_locate_funcs():
    if _has_name:
        cog.out(
"""private struct {0}* ll_locate_{0}_by_name(const char *name) {{
    struct occi_kind_node * nptr;
    struct {0} * current;
    for ( nptr = {0}_first;
        nptr != (struct occi_kind_node *) 0;
        nptr = nptr->next ) {{
        if ((current = nptr->contents) && current->name && 0 == strcmp(name, current->name)) {{
            return current;
        }}
    }}
    return NULL;
}}

private struct {0}* {0}_retrieve_from_name(const char *name) {{
    struct {0} *retVal;
    pthread_mutex_lock( &list_{0}_control );
    retVal = ll_locate_{0}_by_name(name);
    pthread_mutex_unlock( &list_{0}_control );
    return( retVal );
}}""".format(_category_name()))           
        
def profile(function_name, filter_name = None, save = True):
    cog.outl(
"""#ifdef BACKEND_PROFILING
    {0}_backend_profile.{1}++;""".format(_filename_root(), function_name))
    if filter_name:
        cog.outl("    {2}_record_filter_count(filter, &{0}_backend_profile.{1});".format(
            _filename_root(), filter_name, _category_name()))
    if save:
        cog.outl("    save_backend_profile(autosave_{0}_name, &{1}_backend_profile);".format(_category_name(), _filename_root()))
    else:
        cog.outl("    long long start = profile_get_time();")
    cog.outl("#endif")
    
def profile_end(function_name):
    cog.outl("#ifdef BACKEND_PROFILING")
    cog.outl("    long long elapsed = profile_get_time() - start;")
    cog.outl("    {0}_backend_profile.times.{1} += elapsed;".format(_filename_root(), function_name))
    cog.outl("    profile_print(&{0}_backend_profile, CATEGORY_BUCKET, elapsed, start_of_backend);".format(_filename_root()))
    cog.outl("#endif")

def count_filters():
    _format_category(None,
                     "if (filter->{0}) count++;",
                     "if (filter->{0}) count++;") 
         
def link_backend():
    return _category_name() == "cords_xlink"

def riak_backend():
    #return (_category_name() == 'cords_publication') or link_backend() #TODO Hardcoding switch for now
    return False
         
def backend_include():    
    if not riak_backend():
        cog.outl("#include \"{0}_node_backend.h\"".format(_filename_root()))
    else:
        cog.outl("#include \"{0}_riak_backend.h\"".format(_filename_root()))

def backend_init():
    cog.outl("//Backend is {0}".format(category.backend.plugin))
    cog.out("{0}_backend = ".format(_category_name()))
    if not riak_backend():
        cog.out("{0}_node_interface_func();    // TODO There's no obvious place to delete this pointer on completion.  Find somewhere!".format(
            _category_name()))
    else:
        cog.out("{0}_riak_backend_interface();".format(_category_name()))
        
def backend_pre_create():
    if link_backend():
        cog.outl("update_source_id(initial_{0});".format(_filename_root()))     

def backend_pre_update():
    if link_backend():
        cog.outl("update_source_id({0});".format(_filename_root()))
