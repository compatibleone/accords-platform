
#ifndef	_cp_h
#define	_cp_h

#include "standard.h"
#include "document.h"
#include "occiresolver.h"
#include "cordslang.h"

/*	--------------------------	*/
/*	public function prototypes
/*	--------------------------	*/

public	struct	occi_response *	cords_select_category(struct xml_element * document, char * agent, char * tls);
public	struct	occi_response *	cords_retrieve_category(struct xml_element * document, char * id, char * agent, char * tls);
public	struct	occi_response * cords_resolve_category(struct xml_element * document, struct xml_atribut * kptr, char * agent, char * tls);
public	struct	occi_response * cords_create_category( struct xml_element * document, char * agent, char * tls);
public	struct	occi_response * cords_invoke_action( char * resource, char * action, char * agent, char * tls );
public	struct	occi_response * cords_create_link( char * from, char * to, char * agent, char * tls );
public	struct	occi_response * cords_retrieve_named_instance_list(char * category,char * name, char * value,char * agent, char * tls);
public	struct	occi_response * cords_retrieve_named_instance( struct occi_response * yptr, char * agent, char * tls );
public	struct	occi_response * cords_create_instance( 
char * category, char * agent, struct occi_element * eptr, char * tls );

public	int	cords_delete_instance( char * uri, char * category, char * agent, char * tls );
public	int	cords_resolve_location( struct occi_response * zptr, struct xml_element * document ); 
public	int 	cords_parse_element(struct xml_element * document, char * agent, char * tls, int level);
public	int	cords_terminate_level( struct xml_element * dptr, char * agent, char * tls );

public	struct	xml_element  * 	cords_serialise_document( struct xml_element * document, char * result );
public	struct	xml_element  *  cords_drop_document( struct xml_element * document );
public	struct	xml_element  * 	cords_document_parser( char * host, char * filename, char * agent, char * tls );

public  char *	cords_extract_location( struct occi_response * rptr );
public	char *	cords_build_host( struct occi_response * zptr, char * id );

#endif	/* _cp_h */
	/* ----- */








