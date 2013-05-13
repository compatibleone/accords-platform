#ifndef	_cordspublic_h
#define	_cordspublic_h

#include "standard.h"
#include "element.h"

public	struct	xml_element * cords_parse_file( char * sptr );
public	int	cords_file_parser( FILE * target, struct xml_element * eptr, char * domain );
public	int	operation( char * sptr, char * domain, char * result);
public	void	liberate_cords_document( struct  xml_element * document );
public	struct	occi_category *	cords_build_domain( char * domain );

#endif	/* _cordspublic_h */
	/* -------------- */

