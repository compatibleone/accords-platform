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
#ifndef _onconfig_c_
#define _onconfig_c_

#include "element.h"

#include "onconfig.h"

/*	------------------------------------	*/
/*	l i b e r a t e _ o n _ c o n f i g 	*/
/*	------------------------------------	*/
public struct on_config * liberate_on_config(struct on_config * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->user )
			 sptr->user = liberate(sptr->user);
		if ( sptr->password )
			 sptr->password = liberate(sptr->password);
		if ( sptr->authenticate )
			 sptr->authenticate = liberate(sptr->authenticate);
		if ( sptr->agent )
			 sptr->agent = liberate(sptr->agent);
		if ( sptr->host )
			 sptr->host = liberate(sptr->host);
		if ( sptr->version )
			 sptr->version = liberate(sptr->version);
		if ( sptr->namespace )
			 sptr->namespace = liberate(sptr->namespace);
		if ( sptr->base )
			 sptr->base = liberate(sptr->base);
		if ( sptr->tls )
			 sptr->tls = liberate(sptr->tls);
		sptr = liberate( sptr );
	}
	return((struct on_config *) 0);

}

/*	------------------------------	*/
/*	r e s e t _ o n _ c o n f i g 	*/
/*	------------------------------	*/
public struct on_config * reset_on_config(struct on_config * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->description = (char*) 0;
		sptr->user = (char*) 0;
		sptr->password = (char*) 0;
		sptr->authenticate = (char*) 0;
		sptr->agent = (char*) 0;
		sptr->host = (char*) 0;
		sptr->version = (char*) 0;
		sptr->namespace = (char*) 0;
		sptr->base = (char*) 0;
		sptr->tls = (char*) 0;
		sptr->current =  0;
	}
	return(sptr);

}

/*	------------------------------------	*/
/*	a l l o c a t e _ o n _ c o n f i g 	*/
/*	------------------------------------	*/
public struct on_config * allocate_on_config()
{
	struct on_config * sptr;
	if (!( sptr = allocate( sizeof( struct on_config ) ) ))
		return( sptr );
	else	return( reset_on_config(sptr) );
}

/*	------------------------------	*/
/*	x m l i n _ o n _ c o n f i g 	*/
/*	------------------------------	*/
public int xmlin_on_config(struct on_config * sptr,struct xml_element * eptr)
{
	struct xml_element * wptr;
	if (!( eptr )) return(0);
	if (!( sptr )) return(0);
	for ( wptr=eptr->first; wptr != (struct xml_element *) 0; wptr=wptr->next)
	{
		if (!( strcmp(wptr->name,"id") ))
		{
			if ( wptr->value ) { sptr->id = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"user") ))
		{
			if ( wptr->value ) { sptr->user = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"password") ))
		{
			if ( wptr->value ) { sptr->password = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"authenticate") ))
		{
			if ( wptr->value ) { sptr->authenticate = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"agent") ))
		{
			if ( wptr->value ) { sptr->agent = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"host") ))
		{
			if ( wptr->value ) { sptr->host = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"version") ))
		{
			if ( wptr->value ) { sptr->version = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"namespace") ))
		{
			if ( wptr->value ) { sptr->namespace = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"base") ))
		{
			if ( wptr->value ) { sptr->base = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"tls") ))
		{
			if ( wptr->value ) { sptr->tls = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"current") ))
		{
			if ( wptr->value ) { sptr->current = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------	*/
/*	r e s t _ o c c i _ o n _ c o n f i g 	*/
/*	--------------------------------------	*/
public int rest_occi_on_config(FILE * fh,struct on_config * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.user='%s'\r\n",prefix,nptr,(sptr->user?sptr->user:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.password='%s'\r\n",prefix,nptr,(sptr->password?sptr->password:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.authenticate='%s'\r\n",prefix,nptr,(sptr->authenticate?sptr->authenticate:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.agent='%s'\r\n",prefix,nptr,(sptr->agent?sptr->agent:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.host='%s'\r\n",prefix,nptr,(sptr->host?sptr->host:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.version='%s'\r\n",prefix,nptr,(sptr->version?sptr->version:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.namespace='%s'\r\n",prefix,nptr,(sptr->namespace?sptr->namespace:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.base='%s'\r\n",prefix,nptr,(sptr->base?sptr->base:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.tls='%s'\r\n",prefix,nptr,(sptr->tls?sptr->tls:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.current='%u'\r\n",prefix,nptr,sptr->current);
	return(0);

}

#endif	/* _onconfig_c_ */
