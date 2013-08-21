import os

import cog

from codegen import parse_and_generate_code as parse
from codegen import category_for_file

import codegen_types.ctypes as ctypes

# Example invocations from tools directiory:
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir="../model" -o tmp/output.xsd cog/templates/schema.xsd
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir='../model' -o tmp/publication.h cog/templates/category.h 
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir='../model' -o tmp/publication_node_backend.h cog/templates/node_backend.h
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir='../model' -o tmp/publication_node_backend.c cog/templates/node_backend.c
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir='../model' -o tmp/occipublication.c cog/templates/occi.c  
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir='../model' -o tmp/publication_backend.h cog/templates/category_backend.h
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir='../model' -o tmp/publication_occi_filter.h cog/templates/category_filter.h
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir='../model' -o tmp/publication.c cog/templates/category.c      
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir='../model' -o tmp/occipublication.c cog/templates/occicategory_unsplit.c


# python -m cogapp -ed -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/occiauthitem.c cog/templates/occi.c  
# python -m cogapp -ed -D cog_category_file=authitem.h -D model_dir="../model" -o tmp/authitem.xsd cog/templates/schema.xsd
# python -m cogapp -ed -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/authitem.h cog/templates/category.h 
# python -m cogapp -ed -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/authitem_node_backend.h cog/templates/node_backend.h
# python -m cogapp -ed -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/authitem_node_backend.c cog/templates/node_backend.c
# python -m cogapp -ed -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/authitem_backend.h cog/templates/category_backend.h
# python -m cogapp -ed -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/authitem_occi_filter.h cog/templates/category_filter.h
# python -m cogapp -ed -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/occiauthitem.c cog/templates/occicategory_unsplit.c 
# python -m cogapp -ed -D cog_category_file=authitem.h -D model_dir='../model' -o tmp/authitem.c cog/templates/category.c

def init_models(model_dir, filename):
    global models
    global category_file
    global category
    category_file = filename
    models = parse([model_dir], None, None, None)
    category = category_for_file(category_file, models)

def _type_conversion(type_name):
    if type_name == "char *":
        return "string"
    if type_name == "int":
        return "integer"
    return "string"

def _category_name():
    return category.struct_name 

def category_name():
    cog.inline()
    cog.out(_category_name())   

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
    cog.out(_name_root(category_file))

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
    

def node_interface_declaration():
    name = _category_name()
    cog.outl("struct {0}_backend_interface *{1}_node_interface_func();".format(name, name))
    
def h_include():
    cog.outl("#include \"{0}\"".format(category_file.lower()))
    
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
                     "    pptr->{0} = document_atribut_string(aptr);",
                     "    pptr->{0} = document_atribut_value(aptr);")
                 
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
                     "    return( rest_html_response( aptr, 500, \"Server Failure\" ) );"],
                     include_id = False)    
    
def occi_builder():
    _format_category(None,
                     "if (!( optr = occi_add_attribute(optr, \"{0}\",0,0) ))",                     
                     "if (!( optr = occi_add_attribute(optr, \"{0}\",0,0) ))",
                     "    return(optr);",
                     include_id = False)
    
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
    
def _format_category(prefix = None, string_format = None, int_format = None, suffix = None, include_id = True):
    for name, type_name in category.backend_type_list(include_id, skip_legacy_types = True):
        _output_lines(prefix, name)
        if (type_name == "char *"):
            _output_lines(string_format, name, _name_root(category_file), _category_name())
        elif(type_name == "int"):
            _output_lines(int_format, name, _name_root(category_file), _category_name())
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
        

authorization_code = """/*    ------------------------------------------------------    */
/*    a d d _ o c c i _ a u t h o r i z a t i o n _ i t e m     */
/*    ------------------------------------------------------    */
public struct occi_authorization_item * add_occi_authorization_item(struct occi_authorization_cache * pptr)
{
    struct occi_authorization_item * sptr;
    if (!( sptr = allocate( sizeof( struct occi_authorization_item ) ) ))
        return( sptr );
    else if (!( sptr = reset_occi_authorization_item(sptr) ))
        return( sptr );
    else
    {
        if (!( sptr->previous = pptr->last ))
            pptr->first = sptr;
        else    sptr->previous->next = sptr;
        pptr->last = sptr;
        sptr->parent = pptr;
        return( sptr );
    }

}

/*    --------------------------------------------------------    */
/*    d r o p _ o c c i _ a u t h o r i z a t i o n _ i t e m     */
/*    --------------------------------------------------------    */
public struct occi_authorization_item * drop_occi_authorization_item(struct occi_authorization_item * sptr)
{
    if ( sptr )
    {
        if (!( sptr->parent )) return(sptr);
        if (!( sptr->previous ))
        {
            if (!( sptr->parent->first = sptr->next ))
                sptr->parent->last = (struct occi_authorization_item *) 0;
            else    sptr->parent->first->previous = (struct occi_authorization_item *) 0;
        }
        else if (!( sptr->previous->next = sptr->next ))
            sptr->parent->last = sptr->previous;
        if (!( sptr->next ))
        {
            if (!( sptr->parent->last = sptr->previous ))
                sptr->parent->first = (struct occi_authorization_item *) 0;
            else    sptr->parent->last->next = (struct occi_authorization_item *) 0;
        }
        else if (!( sptr->next->previous = sptr->previous ))
            sptr->parent->first = sptr->next;
        sptr = liberate_occi_authorization_item(sptr);
    }
    return((struct occi_authorization_item *) 0);
}
"""

def authorization_special_code():
    if _category_name() == "occi_authorization_item":
        cog.out(authorization_code)