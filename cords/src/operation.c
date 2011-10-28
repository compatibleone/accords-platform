/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons Paternit‚ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */
#ifndef	_operation_c
#define	_operation_c

#include "xmlparse.h"

#include "element.h"

#define	_MAX_CORDS_SCHEMA	13

/*	---------------------------------	*/
/*	schema parser prototype functions	*/
/*	---------------------------------	*/
private	int	cords_account_parser( FILE * target, struct xml_element * eptr, char * domain );
private	int	cords_action_parser( FILE * target, struct xml_element * eptr, char * domain );
private	int	cords_core_parser( FILE * target, struct xml_element * eptr, char * domain );
private	int	cords_compute_parser( FILE * target, struct xml_element * eptr, char * domain );
private	int	cords_configuration_parser( FILE * target, struct xml_element * eptr, char * domain );
private	int	cords_image_parser( FILE * target, struct xml_element * eptr, char * domain );
private	int	cords_infrastructure_parser( FILE * target, struct xml_element * eptr, char * domain );
private	int	cords_network_parser( FILE * target, struct xml_element * eptr, char * domain );
private	int	cords_package_parser( FILE * target, struct xml_element * eptr, char * domain );
private	int	cords_manifest_parser( FILE * target, struct xml_element * eptr, char * domain );
private	int	cords_security_parser( FILE * target, struct xml_element * eptr, char * domain );
private	int	cords_storage_parser( FILE * target, struct xml_element * eptr, char * domain );
private	int	cords_user_parser( FILE * target, struct xml_element * eptr, char * domain );
private	int	cords_nested_parser(FILE * target, struct xml_element * eptr, char * domain );
private	int	cords_default_atb( struct occi_category * cptr);
private	int	cords_default_act( struct occi_category * cptr);

/*	---------------------------------	*/
/*	   schema parser routing table		*/
/*	---------------------------------	*/
private	struct	schema_parser
{
	char *	name;
	int	(*parser)(FILE*, struct xml_element *, char *);
	int	(*attributes)(struct occi_category * cptr);
	int	(*actions)(struct occi_category * cptr);
} CordsSchema[_MAX_CORDS_SCHEMA];

private	void	initialise_schema_parser()
{
	CordsSchema[0].name 	=  "account"; 	
	CordsSchema[0].parser 	= cords_account_parser;
	CordsSchema[0].attributes = cords_default_atb;
	CordsSchema[0].actions	= cords_default_act;
	CordsSchema[1].name	= "action";	
	CordsSchema[1].parser 	= cords_action_parser;
	CordsSchema[1].attributes = cords_default_atb;
	CordsSchema[1].actions	= cords_default_act;
	CordsSchema[2].name	= "core";	
	CordsSchema[2].parser 	= cords_core_parser;
	CordsSchema[2].attributes = cords_default_atb;
	CordsSchema[2].actions	= cords_default_act;
	CordsSchema[3].name	= "compute";	
	CordsSchema[3].parser 	= cords_compute_parser;
	CordsSchema[3].attributes = cords_default_atb;
	CordsSchema[3].actions	= cords_default_act;
	CordsSchema[4].name	= "configuration";	
	CordsSchema[4].parser 	= cords_configuration_parser;
	CordsSchema[4].attributes = cords_default_atb;
	CordsSchema[4].actions	= cords_default_act;
	CordsSchema[5].name	= "image";	
	CordsSchema[5].parser 	= cords_image_parser;
	CordsSchema[5].attributes = cords_default_atb;
	CordsSchema[5].actions	= cords_default_act;
	CordsSchema[6].name	= "infrastructure";
	CordsSchema[6].parser 	= cords_infrastructure_parser;
	CordsSchema[6].attributes = cords_default_atb;
	CordsSchema[6].actions	= cords_default_act;
	CordsSchema[7].name	= "network";
	CordsSchema[7].parser 	= cords_network_parser;
	CordsSchema[7].attributes = cords_default_atb;
	CordsSchema[7].actions	= cords_default_act;
	CordsSchema[8].name	= "package";
	CordsSchema[8].parser 	= cords_package_parser;
	CordsSchema[8].attributes = cords_default_atb;
	CordsSchema[8].actions	= cords_default_act;
	CordsSchema[9].name	= "manifest";
 	CordsSchema[9].parser 	= cords_manifest_parser;
	CordsSchema[9].attributes = cords_default_atb;
	CordsSchema[9].actions	= cords_default_act;
	CordsSchema[10].name	= "security";
 	CordsSchema[10].parser 	= cords_security_parser;
	CordsSchema[10].attributes = cords_default_atb;
	CordsSchema[10].actions	= cords_default_act;
	CordsSchema[11].name	= "storage";
 	CordsSchema[11].parser 	= cords_storage_parser;
	CordsSchema[11].attributes = cords_default_atb;
	CordsSchema[11].actions	= cords_default_act;
	CordsSchema[12].name	= "user";
	CordsSchema[12].parser 	= cords_user_parser;
	CordsSchema[12].attributes = cords_default_atb;
	CordsSchema[12].actions	= cords_default_act;
}

private	int	cords_default_atb( struct occi_category * cptr)
{
	return(0);
}

private	int	cords_default_act( struct occi_category * cptr)
{
	return(0);
}

/*	---------------------------------------------------	*/
/*	cords request element allocation and parsing units 	*/
/*	---------------------------------------------------	*/
private	int	cords_account_parser( FILE * target, struct xml_element * eptr, char * domain  )
{
	struct	cords_account * rptr;
	if (!( eptr ))
		return(118);
	else if (!( rptr = allocate_cords_account()))
		return(27);
	else 
	{
		// printf("cords account instance\n");
		xmlin_cords_account( rptr, eptr );
		rest_occi_cords_account(target,rptr,domain,eptr->name);
		eptr->payload = rptr;
		return( cords_nested_parser( target, eptr, domain ) );
	}
	return(0);
}

private	int	cords_action_parser( FILE * target, struct xml_element * eptr, char * domain  )
{
	struct	cords_action * rptr;
	if (!( eptr ))
		return(118);
	else if (!( rptr = allocate_cords_action()))
		return(27);
	else 
	{
		// printf("cords action instance\n");
		xmlin_cords_action( rptr, eptr );
		rest_occi_cords_action(target,rptr,domain,eptr->name);
		eptr->payload = rptr;
		return( cords_nested_parser( target, eptr, domain ) );
	}
	return(0);
}

private	int	cords_core_parser( FILE * target, struct xml_element * eptr, char * domain  )
{
	struct	cords_core * rptr;
	if (!( eptr ))
		return(118);
	else if (!( rptr = allocate_cords_core()))
		return(27);
	else 
	{
		// printf("cords core instance\n");
		xmlin_cords_core( rptr, eptr );
		rest_occi_cords_core(target,rptr,domain,eptr->name);
		eptr->payload = rptr;
		return( cords_nested_parser( target, eptr, domain ) );
	}
	return(0);
}

private	int	cords_compute_parser( FILE * target, struct xml_element * eptr, char * domain  )
{
	struct	cords_compute * rptr;
	if (!( eptr ))
		return(118);
	else if (!( rptr = allocate_cords_compute()))
		return(27);
	else 
	{
		// printf("cords compute instance\n");
		xmlin_cords_compute( rptr, eptr );
		rest_occi_cords_compute(target,rptr,domain,eptr->name);
		eptr->payload = rptr;
		return( cords_nested_parser( target, eptr, domain ) );
	}
	return(0);
}

private	int	cords_configuration_parser( FILE * target, struct xml_element * eptr, char * domain  )
{
	struct	cords_configuration * rptr;
	if (!( eptr ))
		return(118);
	else if (!( rptr = allocate_cords_configuration()))
		return(27);
	else 
	{
		// printf("cords configuration instance\n");
		xmlin_cords_configuration( rptr, eptr );
		rest_occi_cords_configuration(target,rptr,domain,eptr->name);
		eptr->payload = rptr;
		return( cords_nested_parser( target, eptr, domain ) );
	}
	return(0);
}

private	int	cords_image_parser( FILE * target, struct xml_element * eptr, char * domain  )
{
	struct	cords_image * rptr;
	if (!( eptr ))
		return(118);
	else if (!( rptr = allocate_cords_image()))
		return(27);
	else 
	{
		// printf("cords image instance\n");
		xmlin_cords_image( rptr, eptr );
		rest_occi_cords_image(target,rptr,domain,eptr->name);
		eptr->payload = rptr;
		return( cords_nested_parser( target, eptr, domain ) );
	}
	return(0);
}

private	int	cords_infrastructure_parser( FILE * target, struct xml_element * eptr, char * domain  )
{
	struct	cords_infrastructure * rptr;
	if (!( eptr ))
		return(118);
	else if (!( rptr = allocate_cords_infrastructure()))
		return(27);
	else 
	{
		// printf("cords infrastructure instance\n");
		xmlin_cords_infrastructure( rptr, eptr );
		rest_occi_cords_infrastructure(target,rptr,domain,eptr->name);
		eptr->payload = rptr;
		return( cords_nested_parser( target, eptr, domain ) );
	}
	return(0);
}

private	int	cords_network_parser( FILE * target, struct xml_element * eptr, char * domain  )
{
	struct	cords_network * rptr;
	if (!( eptr ))
		return(118);
	else if (!( rptr = allocate_cords_network()))
		return(27);
	else 
	{
		// printf("cords network instance\n");
		xmlin_cords_network( rptr, eptr );
		rest_occi_cords_network(target,rptr,domain,eptr->name);
		eptr->payload = rptr;
		return( cords_nested_parser( target, eptr, domain ) );
	}
	return(0);
}

private	int	cords_package_parser( FILE * target, struct xml_element * eptr, char * domain  )
{
	struct	cords_package * rptr;
	if (!( eptr ))
		return(118);
	else if (!( rptr = allocate_cords_package()))
		return(27);
	else 
	{
		// printf("cords package instance\n");
		xmlin_cords_package( rptr, eptr );
		rest_occi_cords_package(target,rptr,domain,eptr->name);
		eptr->payload = rptr;
		return( cords_nested_parser( target, eptr, domain ) );
	}
	return(0);
}

private	int	cords_manifest_parser( FILE * target, struct xml_element * eptr, char * domain  )
{
	struct	cords_manifest * rptr;
	if (!( eptr ))
		return(118);
	else if (!( rptr = allocate_cords_manifest()))
		return(27);
	else 
	{
		// printf("cords manifest instance\n");
		xmlin_cords_manifest( rptr, eptr );
		rest_occi_cords_manifest(target,rptr,domain,eptr->name);
		eptr->payload = rptr;
		return( cords_nested_parser( target, eptr, domain ) );
	}
}
private	int	cords_security_parser( FILE * target, struct xml_element * eptr, char * domain  )
{
	struct	cords_security * rptr;
	if (!( eptr ))
		return(118);
	else if (!( rptr = allocate_cords_security()))
		return(27);
	else 
	{
		// printf("cords security instance\n");
		xmlin_cords_security( rptr, eptr );
		rest_occi_cords_security(target,rptr,domain,eptr->name);
		eptr->payload = rptr;
		return( cords_nested_parser( target, eptr, domain ) );
	}
	return(0);
}

private	int	cords_storage_parser( FILE * target, struct xml_element * eptr, char * domain  )
{
	struct	cords_storage * rptr;
	if (!( eptr ))
		return(118);
	else if (!( rptr = allocate_cords_storage()))
		return(27);
	else 
	{
		// printf("cords storage instance\n");
		xmlin_cords_storage( rptr, eptr );
		rest_occi_cords_storage(target,rptr,domain,eptr->name);
		eptr->payload = rptr;
		return( cords_nested_parser( target, eptr, domain ) );
	}
}
private	int	cords_user_parser( FILE * target, struct xml_element * eptr, char * domain  )
{
	struct	cords_user * rptr;
	if (!( eptr ))
		return(118);
	else if (!( rptr = allocate_cords_user()))
		return(27);
	else 
	{
		// printf("cords user instance\n");
		xmlin_cords_user( rptr, eptr );
		rest_occi_cords_user(target,rptr,domain,eptr->name);
		eptr->payload = rptr;
		return( cords_nested_parser( target, eptr, domain ) );
	}
	return(0);
}

public	int	cords_file_parser( FILE * target, struct xml_element * eptr, char * domain )
{
	int	i;
	if (!( eptr ))
		return(118);
	else if (!( eptr->name ))
		return(118);
	else
	{
		initialise_schema_parser();
		for ( i=0; i < _MAX_CORDS_SCHEMA; i++ )
			if (!( strcmp( eptr->name, CordsSchema[i].name ) ))
				return( (*CordsSchema[i].parser)(target,eptr,domain) );
		return(78);
	}
}

private	int	cords_nested_parser( FILE * target, struct xml_element * eptr, char * domain  )
{
	int	status=0;
	struct	xml_element * xptr;
	for (	xptr=eptr->first;
		xptr != (struct xml_element *) 0;
		xptr = xptr->next )
	{
		if (!(status = cords_file_parser(target, xptr, domain )))
			continue;
		else if ( status = 78 )
			continue;
		else	return( status );
	}
	return(0);
}

public	void	liberate_cords_document( struct xml_element * document )
{
	document = document_drop( document );
	return;
}


public	int	operation( char * sptr, char * domain, char * result)
{
	struct	xml_element * document;
	FILE * target;
	if (!( document = document_parse_file( sptr )))
		return(40);
	else if (!( target = fopen( result, "w" ) ))
	{
		liberate_cords_document( document );
		return( 46 );
	}
	else
	{
		cords_file_parser(target, document, domain );
		fclose( target );
		liberate_cords_document( document );
		return(0);
	}

}

/*	--------------------------------------------------	*/
/*		c o r d s _ b u i l d _ d o m a i n		*/
/*	--------------------------------------------------	*/
/*	this function will build the complete CORDS domain	*/
/*	as a list of OCCI categories.				*/
/*	--------------------------------------------------	*/
public	struct	occi_category *	cords_build_domain( char * domain )
{
	char	schema[1024];
	struct	occi_category * first = (struct occi_category *) 0;
	struct	occi_category * last  = (struct occi_category *) 0;
	struct	occi_category * optr  = (struct occi_category *) 0;
	int	i;

	initialise_schema_parser();

	sprintf(schema,"http://scheme.ogf.org/%s#",domain);

	for (	i=0; i < _MAX_CORDS_SCHEMA; i++ )
	{
		if (!( optr = occi_create_category( 
			domain,
			CordsSchema[i].name,
			schema,
			"kind",
			"http://schema.ogf.org/occi/core#resource",
			"compatible one cords category" ) ))
			break;
		else if (!( optr->previous = last ))
			first = optr;
		else	optr->previous->next = optr;
		last = optr;
		if ( (*CordsSchema[i].attributes)( optr ) != 0 )
			break;
		else if ( (*CordsSchema[i].actions)( optr ) != 0 )
			break;
	}

	return( first );
}

#endif	


