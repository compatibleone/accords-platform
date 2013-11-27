/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */
#ifndef	_comodel_c
#define	_comodel_c

#include "standard.h"
#include "rest.h"
#include "accords.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"
#include "comodel.h"
#include "strukt.h"

#define	_COMODEL_VERSION "1.0.04"

/*	------------------------------------------------------------------	*/
/*	   a c c o r d s _ p l a t f o r m _ c o n f i g u r a t i o n		*/
/*	------------------------------------------------------------------	*/
struct	accords_configuration CoModel = 
{
	0,0, 
	0,0,0,0,
	(char *) 0,
	(char *) 0,
	_CORDS_DEFAULT_PUBLISHER,
	_CORDS_DEFAULT_OPERATOR,
	_CORDS_DEFAULT_USER,
	_CORDS_DEFAULT_PASSWORD,
	"http",  80,
	"xmpp",  8000,
	"domain",
	"comodel.xml",
	"europe",
	"storage",
	"comodel",
	(struct occi_category *) 0,
	(struct occi_category *) 0
};

private	int	testflag=0;
public	int	check_testflag()	{	return( testflag);	}
private	int	webflag=0;
public	int	check_webflag()		{	return( webflag);	}

/*	------------------------------------------------------------------	*/
/*	c o m o d e l _ o c c i _ p r o d u c i o n _ p a r a m e t e r s	*/
/*	------------------------------------------------------------------	*/
struct	occi_production
{
	char *	path;
	int	nature;
	int	backend;
	char *	prefix;
	struct	occi_component * first;
	struct	occi_component * last;
} production = 
{
	"",
	0, 0,
	"occi",
	(struct occi_component *) 0,
	(struct occi_component *) 0
};


/*	------------------------------------------------------------------	*/
/*			s t a n d a r d _ r e l a y s 				*/
/*	------------------------------------------------------------------	*/
public	int	check_debug()		{	return(CoModel.debug);		}
public	int	check_verbose()		{	return(CoModel.verbose);	}
public	char *	default_publisher()	{	return(CoModel.publisher);	}
public	char *	default_operator()	{	return(CoModel.operator);	}
public	char *	default_tls()		{	return(CoModel.tls);		}
public	char *	default_zone()		{	return(CoModel.zone);		}
public	int	failure( int e, char * m1, char * m2 )
{
	if ( e )
	{
		printf("\n*** failure %u",e);
		if ( m1 )
			printf(" : %s",m1);
		if ( m2 )
			printf(" : %s",m2);
		printf(" **`\n");
	}
	return( e );
}

/*	------------------------------------------------------------------	*/
/*			c o m o d e l _ l o a d 				*/
/*	------------------------------------------------------------------	*/
private	void	comodel_load()
{
	load_accords_configuration( &CoModel, "comodel" );
	return;
}

/*	------------------------------------------------------------------	*/
/*		c o m o d e l _ c a t e g o r y _ f i l t e r			*/
/*	------------------------------------------------------------------	*/
public int	comodel_category_filter( struct occi_category * cptr )
{
	if (!( cptr ))	
		return(0);
	else if (!( cptr->id )) 
		return(0);
	else if (!( strcmp( cptr->id, "link" ) ))
		return(0);
	else if (!( strcmp( cptr->id, "entity" ) ))
		return(0);
	else if (!( strcmp( cptr->id, "resource" ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			c o m o d e l _ b a n n e r 				*/
/*	------------------------------------------------------------------	*/
private	int	comodel_banner()
{
	printf("\n   CompatibleOne Model Generator : Version %s ",_COMODEL_VERSION);
	printf("\n   Beta Version : 27/11/2013");
	printf("\n   Copyright (c) 2011, 2013 Iain James Marshall, Prologue");
	printf("\n\n");
	return(0);
}

/*	------------------------------------------------------------------	*/
/*		l i b e r t e _ o c c i _ c o m p o n e n t			*/
/*	------------------------------------------------------------------	*/
private	struct	occi_component * liberate_occi_component(struct occi_component * cptr)
{
	struct	occi_category * optr;
	if ( cptr )
	{
		while ((optr = cptr->first) != (struct occi_category *) 0)
		{
			cptr->first = optr->next;
			liberate_occi_category( optr );
		}
		if ( cptr->name )
			liberate( cptr->name );
		liberate( cptr );
	}
	return((struct occi_component *) 0);
}

/*	------------------------------------------------------------------	*/
/*		a l l o c a t e _ o c c i _ c o m p o n e n t			*/
/*	------------------------------------------------------------------	*/
private	struct	occi_component *	allocate_occi_component()
{
	struct	occi_component * cptr;
	if (!( cptr = allocate( sizeof( struct occi_component ) ) ))
		return( cptr );
	else
	{
		memset(cptr,0,sizeof( struct occi_component ));
		return( cptr );
	}
}

/*	------------------------------------------------------------------	*/
/*		c o m o d e l _ a d d _ c o m p o n e n t			*/
/*	------------------------------------------------------------------	*/
private	struct	occi_component * comodel_add_component( char * nptr )
{
	struct	occi_component * cptr;
	if (!( cptr = allocate_occi_component() ))
		return( cptr );
	else if (!( cptr->name = allocate_string( nptr ) ))
		return( liberate_occi_component( cptr ) );
	else if (!( cptr->previous = production.last ))
		production.first = cptr;
	else	cptr->previous->next = cptr;
	return((production.last = cptr));
}

/*	------------------------------------------------------------------	*/
/*		c o m o d e l _ a d d _c a t e g o r y				*/
/*	------------------------------------------------------------------	*/
private	struct	occi_category * comodel_add_category( char * nptr )
{
	struct	occi_component * cptr;
	struct	occi_category * optr;

	/* add category to component */
	/* ------------------------- */
	if (!( cptr = production.last ))
		cptr = comodel_add_component( nptr );
	if (!( cptr ))
		return((struct occi_category *) 0);
	else if (!( optr = allocate_occi_category() ))
		return((struct occi_category *) 0);
	else if (!( optr->id = allocate_string( nptr ) ))
		return( liberate_occi_category( optr ) );
	else if (!( optr->previous = cptr->last ))
		cptr->first = optr;
	else	optr->previous->next = optr;
	cptr->last = optr;
	return( optr );
}

/*	------------------------------------------------------------------	*/
/*			c o m o d e l _ f e t c h 				*/
/*	------------------------------------------------------------------	*/
private	char *	comodel_fetch( char * sptr )
{
	char	buffer[8192];
	struct	rest_response * rptr;
	struct	rest_header * hptr;

	sprintf(buffer,"%s/-/",sptr);

	if ( check_verbose() )
		printf("fetching model description : %s\n",sptr);

	if (!( hptr = rest_create_header( "Accept", "application/xml" ) ))
		return((char *) 0);
	else if (!( rptr = rest_client_get_request( buffer, default_tls(), "CoModel/1.0a", hptr ) ))
		return((char *) 0);
	else if ( rptr->status > 299 )
		return((char *) 0);
	else if (!( rptr->body ))
		return((char *) 0);
	else if ( rptr->type != _FILE_BODY )
		return((char *) 0);
	else if (!( sptr = allocate_string( rptr->body ) ))
		return((char *) 0);
	else	return( sptr );		
}

/*	------------------------------------------------------------------	*/
/*			c o m o d e l _ g e n e r a t e _ h 			*/
/*	------------------------------------------------------------------	*/
private	int	comodel_generate_h( struct occi_category * optr )
{
	struct	occi_attribute * aptr;
	char	buffer[2048];
	FILE * h;
	if (!( optr ))
		return( 118 );
	else 	sprintf(buffer,"%s%s.h",production.path,optr->id);
	if (!( h = fopen( buffer,"w" ) ))
		return( 46 );
	else
	{
		fprintf(h,"#ifndef _cords_%s_h\n",optr->id);
		fprintf(h,"#define _cords_%s_h\n",optr->id);
		fprintf(h,"/* ----------------------------------------------- */\n");
		fprintf(h,"/* WARNING this file has been generated by COMODEL */\n");
		fprintf(h,"/* ----------------------------------------------- */\n");
		fprintf(h,"struct cords_%s\n{\n",optr->id);
		fprintf(h,"\tchar *\t%s;\n","id");
		if ( production.backend )
			fprintf(h,"\tchar *\t%s;\n","orderid");
		for (	aptr=optr->first;
			aptr != (struct occi_attribute *) 0;
			aptr = aptr->next )
		{
			if (!( aptr->name ))
				continue;
			else if (!( strcmp( aptr->name,"state") ))
				fprintf(h,"\tint\t%s;\n",aptr->name);
			else if (!( aptr->type ))
				fprintf(h,"\tchar * \t%s;\n",aptr->name);
			else if (!( strcmp( aptr->type,"none") ))
				fprintf(h,"\tchar * \t%s;\n",aptr->name);
			else if (!( strcmp( aptr->type,"string") ))
				fprintf(h,"\tchar * \t%s;\n",aptr->name);
			else if (!( strcmp( aptr->type,"xsd:string") ))
				fprintf(h,"\tchar * \t%s;\n",aptr->name);
			else if (!( strcmp( aptr->type,"int") ))
				fprintf(h,"\tint\t%s;\n",aptr->name);
			else if (!( strcmp( aptr->type,"xsd:int") ))
				fprintf(h,"\tint\t%s;\n",aptr->name);
			else 	fprintf(h,"\tchar * \t%s;\n",aptr->name);
		}
		fprintf(h,"};\n");
		fprintf(h,"#endif /* _cords_%s_h */\n",optr->id);
		fclose(h);
		return(0);
	}
}

/*	------------------------------------------------------------------	*/
/*		c o m o d e l _ g e n e r a t e _ f i l t e r _ h 		*/
/*	------------------------------------------------------------------	*/
private	int	comodel_generate_filter_h( struct occi_category * optr )
{
	struct	occi_attribute * aptr;
	char	buffer[2048];
	FILE * h;
	if (!( optr ))
		return( 118 );
	else 	sprintf(buffer,"%s%s_filter.h",production.path,optr->id);
	if (!( h = fopen( buffer,"w" ) ))
		return( 46 );
	else
	{
		fprintf(h,"#ifndef _cords_%s_filter_h\n",optr->id);
		fprintf(h,"#define _cords_%s_filter_h\n",optr->id);
		fprintf(h,"/* ----------------------------------------------- */\n");
		fprintf(h,"/* WARNING this file has been generated by COMODEL */\n");
		fprintf(h,"/* ----------------------------------------------- */\n");
		fprintf(h,"struct cords_%s_occi_filter\n{\n",optr->id);
		fprintf(h,"\tstruct cords_%s * attributes;\n",optr->id);
		fprintf(h,"\tint\t%s;\n","id");
		for (	aptr=optr->first;
			aptr != (struct occi_attribute *) 0;
			aptr = aptr->next )
		{
			if (!( aptr->name ))
				continue;
			else	fprintf(h,"\tint\t%s;\n",aptr->name);
		}
		fprintf(h,"};\n");
		fprintf(h,"#endif /* _cords_%s_filter_h */\n",optr->id);
		fclose(h);
		return(0);
	}
}

/*	------------------------------------------------------------------	*/
/*		c o m o d e l _ s h o w _ c a t e g o r y 			*/
/*	------------------------------------------------------------------	*/
private	void	comodel_show_category( struct occi_category * optr )
{
	struct	occi_attribute * aptr;
	struct	occi_action    * xptr;
	if ( optr )
	{
		printf("<category");
		if ( optr->id )
			printf(" term=\"%s\"",optr->id );

		if ( optr->class )
			printf(" class=\"%s\"",optr->class );

		if ( optr->scheme )
			printf(" scheme=\"%s\"",optr->scheme );

		if ( optr->rel )
			printf(" rel=\"%s\"",optr->rel);

		if ( optr->location )
			printf(" location=\"%s\"",optr->location );

		printf(">\n");
		for (	aptr=optr->first;
			aptr != (struct occi_attribute *) 0;
			aptr = aptr->next )
		{
			printf("<attribute name=\"%s\" required=\"%s\" immutable=\"%s\"",
				aptr->name,
				( aptr->mandatory ? "true" : "false" ),
				( aptr->immutable ? "true" : "false" ));
			if ( aptr->type ) printf(" type=\"%s\"",aptr->type);
			if ( aptr->units ) printf(" units=\"%s\"",aptr->units);
			if ( aptr->validation ) printf(" validation=\"%s\"",aptr->validation);
			if ( aptr->defaultvalue ) printf(" defaultvalue=\"%s\"",aptr->defaultvalue);
			printf(" index=\"%s\"/>\n",(aptr->indexed ? "true" : "false"));

		}
		for (	xptr=optr->firstact;
			xptr != (struct occi_action *) 0;
			xptr = xptr->next )
		{
			printf("<action id=\"%s\"/>\n",xptr->name);
		}

		printf("</category>\n");
	}
	return;
}

/*	------------------------------------------------------------------	*/
/*		c o m o d e l _ x m l _ c a t e g o r y 			*/
/*	------------------------------------------------------------------	*/
private	void	comodel_xml_category( FILE * h, struct occi_category * optr )
{
	struct	occi_attribute * aptr;
	struct	occi_action    * xptr;
	if ( optr )
	{
		fprintf(h,"<category");
		if ( optr->id )
			fprintf(h," term=\"%s\"",optr->id );

		if ( optr->class )
			fprintf(h," class=\"%s\"",optr->class );

		if ( optr->scheme )
			fprintf(h," scheme=\"%s\"",optr->scheme );

		if ( optr->rel )
			fprintf(h," rel=\"%s\"",optr->rel);

		if ( optr->location )
			fprintf(h," location=\"%s\"",optr->location );

		if ( optr->version )
			fprintf(h," version=\"%s\"",optr->version );

		fprintf(h,">\n");
		fprintf(h,"<attributes>\n");
		for (	aptr=optr->first;
			aptr != (struct occi_attribute *) 0;
			aptr = aptr->next )
		{
			fprintf(h,"<attribute name=\"%s\" required=\"%s\" immutable=\"%s\"",
				aptr->name,
				( aptr->mandatory ? "true" : "false" ),
				( aptr->immutable ? "true" : "false" ));
			if ( aptr->type ) fprintf(h," type=\"%s\"",aptr->type);
			if ( aptr->units ) fprintf(h," units=\"%s\"",aptr->units);
			if ( aptr->validation ) fprintf(h," validation=\"%s\"",aptr->validation);
			if ( aptr->defaultvalue ) fprintf(h," defaultvalue=\"%s\"",aptr->defaultvalue);
			fprintf(h," index=\"%s\"/>\n",(aptr->indexed ? "true" : "false"));

		}
		fprintf(h,"</attributes>\n");
		fprintf(h,"<actions>\n");
		for (	xptr=optr->firstact;
			xptr != (struct occi_action *) 0;
			xptr = xptr->next )
		{
			fprintf(h,"<action id=\"%s\"/>\n",xptr->name);
		}
		fprintf(h,"</actions>\n");
		fprintf(h,"</category>\n");
	}
	return;
}

/*	------------------------------------------------------------------	*/
/*			c o m o d e l _ a t t r i b u t e			*/
/*	------------------------------------------------------------------	*/
private	char *	comodel_attribute( struct xml_element * eptr, char * nptr )
{
	char *	sptr;
	struct	xml_atribut * aptr;
	if (!(aptr = document_atribut( eptr, nptr )))
		return( (char *) 0 );
	else if (!( sptr = allocate_string( aptr->value ) ))
		return( (char *) 0 );
	else if (!( sptr = occi_unquoted_value( sptr )))
		return( (char *) 0 );
	else	return( sptr );
}

/*	------------------------------------------------------------------	*/
/*			c o m o d e l _ b y _ c o l l e c t i o n		*/
/*	------------------------------------------------------------------	*/
struct	occi_category * comodel_by_collection( struct occi_category * optr, struct xml_element * dptr )
{
	char *	nptr;
	char *	vptr;
	int	required;
	int	immutable;
	if (!( nptr =  comodel_attribute(dptr,"name") ))
		return((struct occi_category *) 0);
	if (!( vptr =  comodel_attribute(dptr,"required") ))
		required = 0;
	else if (!( strcasecmp(vptr,"true") ))
		required = 1;
	else 	required = 0;

	if (!( vptr =  comodel_attribute(dptr,"immutable") ))
		immutable = 0;
	else if (!( strcasecmp(vptr,"true") ))
		immutable = 1;
	else 	immutable = 0;

	if (!( optr = occi_add_attribute( optr, nptr, required, immutable ) ))
		return( optr );

	if (!( vptr =  comodel_attribute(dptr,"index") ))
		optr->last->indexed=0;
	else if (!( strcasecmp(vptr,"true") ))
		optr->last->indexed=1;
	else 	optr->last->indexed=0;

	optr->last->type = comodel_attribute(dptr,"type");
	return( optr );
}

/*	------------------------------------------------------------------	*/
/*			c o m o d e l _ b y _ i n s t a n c e 			*/
/*	------------------------------------------------------------------	*/
struct	occi_category * comodel_by_instance( struct occi_category * optr, struct xml_element * dptr )
{
	char *	nptr;
	char *	vptr;
	int	required;
	int	immutable;
	if (!( nptr =  comodel_attribute(dptr,"name") ))
		return((struct occi_category *) 0);
	if (!( vptr =  comodel_attribute(dptr,"required") ))
		required = 0;
	else if (!( strcasecmp(vptr,"true") ))
		required = 1;
	else 	required = 0;

	if (!( vptr =  comodel_attribute(dptr,"immutable") ))
		immutable = 0;
	else if (!( strcasecmp(vptr,"true") ))
		immutable = 1;
	else 	immutable = 0;

	if (!( optr = occi_add_attribute( optr, nptr, required, immutable ) ))
		return( optr );

	if (!( vptr =  comodel_attribute(dptr,"index") ))
		optr->last->indexed=0;
	else if (!( strcasecmp(vptr,"true") ))
		optr->last->indexed=1;
	else 	optr->last->indexed=0;

	optr->last->type = comodel_attribute(dptr,"type");
	return( optr );
}

/*	------------------------------------------------------------------	*/
/*			c o m o d e l _ b y _ c a t e g o r y 			*/
/*	------------------------------------------------------------------	*/
struct	occi_category * comodel_by_attribute( struct occi_category * optr, struct xml_element * dptr )
{
	char *	nptr;
	char *	vptr;
	int	required;
	int	immutable;
	if (!( nptr =  comodel_attribute(dptr,"name") ))
		return((struct occi_category *) 0);

	if (!( vptr =  comodel_attribute(dptr,"required") ))
		required = 0;
	else if (!( strcasecmp(vptr,"true") ))
		required = 1;
	else 	required = 0;

	if (!( vptr =  comodel_attribute(dptr,"immutable") ))
		immutable = 0;
	else if (!( strcasecmp(vptr,"true") ))
		immutable = 1;
	else 	immutable = 0;

	if (!( optr = occi_add_attribute( optr, nptr, required, immutable ) ))
		return( optr );

	if (!( vptr =  comodel_attribute(dptr,"index") ))
		optr->last->indexed=0;
	else if (!( strcasecmp(vptr,"true") ))
		optr->last->indexed=1;
	else 	optr->last->indexed=0;

	optr->last->type = comodel_attribute(dptr,"type");
	optr->last->units = comodel_attribute(dptr,"units");
	optr->last->validation = comodel_attribute(dptr,"validation");
	optr->last->defaultvalue = comodel_attribute(dptr,"defaultvalue");
	return( optr );
}

/*	------------------------------------------------------------------	*/
/*			c o m o d e l _ b y _ c a t e g o r y 			*/
/*	------------------------------------------------------------------	*/
private	int	comodel_by_category( struct xml_element * eptr )
{
	char *	nptr;
	char *	vptr;
	char *	sptr;
	struct	xml_atribut * aptr;
	struct	occi_category * optr;
	struct	xml_element * dptr;
	for ( ; eptr != (struct xml_element *) 0; eptr = eptr->next )
	{
		if (!(aptr = document_atribut( eptr, "term" )))
			continue;
		else if (!( sptr = allocate_string( aptr->value ) ))
			continue;
		else if (!( sptr = occi_unquoted_value( sptr )))
			continue;
		else if (!( optr = comodel_add_category( sptr ) ))
			break;
		else	
		{
			optr->scheme   = comodel_attribute(eptr,"scheme");
			optr->class    = comodel_attribute(eptr,"class");
			optr->rel      = comodel_attribute(eptr,"rel");
			optr->location = comodel_attribute(eptr,"location");

			if (!( optr->version = comodel_attribute(eptr,"version") ))
				optr->version = allocate_string(_COMODEL_VERSION);
						
			if ((dptr=document_element( eptr, "attributes")) != (struct xml_element *) 0)
			{
				for (	dptr=document_element( dptr, "attribute");
					dptr != (struct xml_element *) 0;
					dptr = dptr->next )
				{
					if (!( dptr->name ))
						continue;
					else if (!( strcmp( dptr->name, "attribute" )))
					{
						if (!( optr = comodel_by_attribute( optr, dptr ) ))
							break;
						else	continue;
					}
					else if (!( strcmp( dptr->name, "instance" )))
					{
						if (!( optr = comodel_by_instance( optr, dptr ) ))
							break;
						else	continue;
					}
					else if (!( strcmp( dptr->name, "collection" )))
					{
						if (!( optr = comodel_by_collection( optr, dptr ) ))
							break;
						else	continue;
					}
					else	continue;
				}
			}

			if ((dptr=document_element( eptr, "actions")) != (struct xml_element *) 0)
			{
				for (	dptr=document_element( dptr, "action");
					dptr != (struct xml_element *) 0;
					dptr = dptr->next )
				{
					if (!( dptr->name ))
						continue;
					else if ( strcmp( dptr->name, "action" ) != 0 )
						continue;
					else if (!( nptr =  comodel_attribute(dptr,"id") ))
						continue;
					else if (!( optr = occi_add_action( optr, nptr,"", (void *) 0) ))
						break;
					else	continue;
				}
			}


			if ( check_verbose() )
			{
				printf("comodel loaded category : %s\n",sptr);
			}

			if ( check_debug() )
			{
				comodel_show_category( optr );
			}
			liberate( sptr );
		}

	}
	return( 0 );
}

/*	------------------------------------------------------------------	*/
/*		c o m o d e l _ b y _ c o m p o n e n t 			*/
/*	------------------------------------------------------------------	*/
private	int	comodel_by_component( struct xml_element * eptr)
{
	char *	sptr;
	int	status;
	struct	xml_atribut * aptr;
	struct	occi_component * cptr;
	for ( ; eptr != (struct xml_element *) 0; eptr = eptr->next )
	{
		if (!(aptr = document_atribut( eptr, "name" )))
			continue;
		else if (!( sptr = allocate_string( aptr->value ) ))
			continue;
		else if (!( sptr = occi_unquoted_value( sptr )))
			continue;
		else if (!( cptr = comodel_add_component( sptr )))
			break;
		else
		{
			deserialise_component_configuration(&cptr->configuration,eptr);
			cptr->isvalid=1;
			if ( check_verbose() )
				printf("comodel loaded component : %s\n",sptr);
			if ((status = comodel_by_category( eptr->first )) != 0)
				break;
			liberate( sptr );
		}
	}
	return( 0 );
}

/*	------------------------------------------------------------------	*/
/*			c o m o d e l _ b y _ m o d e l 			*/
/*	------------------------------------------------------------------	*/
private	int	comodel_by_model( struct xml_element * eptr)
{
	char *	sptr;
	int	status;
	struct	xml_atribut * aptr;
	for ( ; eptr != (struct xml_element *) 0; eptr = eptr->next )
	{
		if (!(aptr = document_atribut( eptr, "name" )))
			continue;
		else if (!( sptr = allocate_string( aptr->value ) ))
			continue;
		else if (!( sptr = occi_unquoted_value( sptr )))
			continue;
		else
		{
			if ( check_verbose() )
				printf("comodel loaded model : %s\n",sptr);
			if ((status = comodel_by_category( eptr->first )) != 0)
				break;
			liberate( sptr );
		}
	}
	return( 0 );
}

/*	------------------------------------------------------------------	*/
/*			c o m o d e l _ b y _ s c h e m a 			*/
/*	------------------------------------------------------------------	*/
private	int	comodel_by_schema( struct xml_element * eptr)
{
	if ((eptr = document_element( eptr, "model" )) != (struct xml_element *) 0)
		return( comodel_by_model( eptr ) );
	else	return( 78 );
}

/*	------------------------------------------------------------------	*/
/*		c o m o d e l _ p r e p a r e _ s t r u k t			*/
/*	------------------------------------------------------------------	*/
private	int	comodel_prepare_strukt(int mode)
{
	reset_strukt();
	switch ( mode )
	{
	case	0:	/* c structure generation */
		strukt_set_license("license.h");
		strukt_set_backend(production.backend);
		strukt_set_prefix("");
		strukt_set_genxml(1);
		strukt_set_genocci(1);
		break;
	case	1:	/* occi rest generation */
		strukt_set_license("license.h");
		strukt_set_backend(production.backend);
		strukt_set_prefix(production.prefix);
		strukt_set_gencrud(1);
		strukt_set_genrest(_OCCI_KIND);
		break;
	case	2:	/* schema generation */
		strukt_set_prefix("");
		strukt_set_genxml(1);
		strukt_set_schema(1);
		break;
	}
	return(0);
}

/*	------------------------------------------------------------------	*/
/*		c o m o d e l _ c _ p r o d u c t i o n 			*/
/*	------------------------------------------------------------------	*/
private	int	comodel_c_production()
{
	int	status;
	struct	occi_component 	* cptr;
	struct	occi_category 	* optr;
	char 	makename[1024];
	char 	buffer[1024];
	if ( check_verbose() )
		printf("comodel c production\n");

	switch ( production.backend )
	{
	case	1	:
		production.prefix = "sql";
		break;
	default		:
		production.prefix = "occi";
		break;
	}

	for (	cptr=production.first;
		cptr != (struct occi_component *) 0;
		cptr = cptr->next )
	{
		sprintf(buffer,"%s-configuration.xml",cptr->name);
		if ( cptr->isvalid )
		{
			/* --------------------------------------------------------------- */
			/* generate the component source if component configuration loaded */
			/* --------------------------------------------------------------- */
			save_accords_configuration( &cptr->configuration, buffer );
			generate_service_component( cptr->name, cptr->first, production.prefix );
			generate_service_makefile( cptr->name, cptr->first, production.prefix );
			if ( webflag ) generate_service_interface( cptr->name, cptr->first, production.prefix );
		}
		for (	optr=cptr->first;
			optr != (struct occi_category *) 0;
			optr = optr->next )
		{
			if (!( comodel_category_filter( optr ) ))
				continue;
			else if ((status = comodel_generate_h( optr )) != 0)
				return( status );
			else if ((status = comodel_generate_filter_h( optr )) != 0)
				return( status );
			else if ((status = comodel_prepare_strukt(0)) != 0)
				return(status);
			else if ((status = process( optr->id )) != 0)
				return( status );
			else if ((status = comodel_prepare_strukt(1)) != 0)
				return(status);
			else if ((status = process( optr->id )) != 0)
				return( status );
			else if ((status = comodel_prepare_strukt(2)) != 0)
				return(status);
			else if ((status = schema( optr->id )) != 0)
				return( status );
		}
	}
	return( status );
}

/*	------------------------------------------------------------------	*/
/*		c o m o d e l _ x _ p r o d u c t i o n 			*/
/*	------------------------------------------------------------------	*/
private	int	comodel_x_production()
{
	int	status;
	struct	occi_component 	* cptr;
	struct	occi_category 	* optr;
	char *	configuration;
	char	buffer[2048];
	FILE *	h;

	if ( check_verbose() )
		printf("comodel c production\n");

	for (	cptr=production.first;
		cptr != (struct occi_component *) 0;
		cptr = cptr->next )
	{
		if (!( configuration = serialise_component_configuration( &cptr->configuration ) ))
			continue;
		sprintf(buffer,"%s.xml",cptr->name);
		if (!( h = fopen( buffer, "w") ))
			return(46);
		else
		{
			fprintf(h,"<?xml version='1.0' encoding='UTF-8'?>\n");
			fprintf(h,"<component %s>\n",configuration);
			liberate( configuration );
			for (	optr=cptr->first;
				optr != (struct occi_category *) 0;
				optr = optr->next )
			{
				comodel_xml_category(h,optr);
			}
			fprintf(h,"</component>\n",cptr->name);
		}
	}
}

/*	------------------------------------------------------------------	*/
/*		c o m o d e l _ p r o d u c t i o n 				*/
/*	------------------------------------------------------------------	*/
private	int	comodel_production()
{
	if ( check_verbose() )
		printf("comodel production\n");

	switch ( production.nature )
	{
	case	1	:
		return( comodel_x_production() );
	case	0	:
		return( comodel_c_production() );
	default		:
		return(failure(30,"incorrection","production type") );
	}
}	

/*	------------------------------------------------------------------	*/
/*			c o m o d e l _ o p e r a t i o n			*/
/*	------------------------------------------------------------------	*/
private	int	comodel_operation( char * sptr )
{
	int	status;
	char *	nptr;
	struct	xml_element * document;
	struct	xml_element * eptr;

	if (!( sptr ))
		return(0);

	if (!( strncmp( sptr, "http", strlen( "http" ) )))
	{
		if (!( nptr = comodel_fetch( sptr ) ))
			return( failure( 404, "unable to fetch", sptr ) );
		else	sptr = nptr;
	}

	if ( check_verbose() )
		printf("parsing model description : %s\n",sptr);
	if (!( document = document_parse_file( sptr )))
		return( failure(40,"file parse failure",sptr) );
	else if ((eptr = document_element( document, "schema" )) != (struct xml_element *) 0)
		return( comodel_by_schema( eptr ) );
	else if ((eptr = document_element( document, "model" )) != (struct xml_element *) 0)
		return( comodel_by_model( eptr ) );
	else if ((eptr = document_element( document, "component" )) != (struct xml_element *) 0)
		return( comodel_by_component( eptr ) );
	else if ((eptr = document_element( document, "category" )) != (struct xml_element *) 0)
		return( comodel_by_category( eptr ) );
	else	return(failure(30,"incorrect document type",sptr));
}

/*	------------------------------------------------------------------	*/
/*				c o _ m o d e l					*/
/*	------------------------------------------------------------------	*/
private	int	co_model(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	comodel_load();
	while ( argi < argc )
	{
		if (!( aptr = argv[++argi] ))
			break;
		else if ( *aptr == '-' )
		{
			aptr++;
			switch( *(aptr++) )
			{
			case	'i'	:
				webflag=1;
				continue;
			case	't'	:
				testflag=1;
				continue;
			case	'e'	:
				set_xml_echo(1);
				continue;
			case	'v'	:
				CoModel.verbose=1;
				continue;
			case	'd'	:
				CoModel.debug = 0xFFFF;
				continue;
			case	'q'	:
				production.backend = 1;
				continue;
			case	'x'	:
				production.nature = 1;
				continue;
			case	'c'	:
				production.nature = 0;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = comodel_operation(aptr) ))
			continue;
		else	break;
	}
	if ( status )
		return(status);
	else	return( comodel_production() );
}

/*	------------------------------------------------------------------	*/
/*					m a i n 				*/
/*	------------------------------------------------------------------	*/
public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( comodel_banner() );
	else	return( co_model( argc, argv ) );
}


	/* --------- */
#endif	/* _comodel_c */
	/* --------- */

