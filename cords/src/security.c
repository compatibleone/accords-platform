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
#ifndef _security_c_
#define _security_c_

#include "element.h"

#include "security.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ s e c u r i t y 	*/
/*	----------------------------------------------	*/
public struct cords_security * liberate_cords_security(struct cords_security * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->identity )
			 sptr->identity = liberate(sptr->identity);
		if ( sptr->nature )
			 sptr->nature = liberate(sptr->nature);
		if ( sptr->scheme )
			 sptr->scheme = liberate(sptr->scheme);
		if ( sptr->privatekey )
			 sptr->privatekey = liberate(sptr->privatekey);
		if ( sptr->publickey )
			 sptr->publickey = liberate(sptr->publickey);
		sptr = liberate( sptr );
	}
	return((struct cords_security *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ c o r d s _ s e c u r i t y 	*/
/*	----------------------------------------	*/
public struct cords_security * reset_cords_security(struct cords_security * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->identity = (char*) 0;
		sptr->nature = (char*) 0;
		sptr->scheme = (char*) 0;
		sptr->privatekey = (char*) 0;
		sptr->publickey = (char*) 0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ s e c u r i t y 	*/
/*	----------------------------------------------	*/
public struct cords_security * allocate_cords_security()
{
	struct cords_security * sptr;
	if (!( sptr = allocate( sizeof( struct cords_security ) ) ))
		return( sptr );
	else	return( reset_cords_security(sptr) );
}

/*	----------------------------------------	*/
/*	x m l i n _ c o r d s _ s e c u r i t y 	*/
/*	----------------------------------------	*/
public int xmlin_cords_security(struct cords_security * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"identity") ))
		{
			if ( wptr->value ) { sptr->identity = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"nature") ))
		{
			if ( wptr->value ) { sptr->nature = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"scheme") ))
		{
			if ( wptr->value ) { sptr->scheme = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"privatekey") ))
		{
			if ( wptr->value ) { sptr->privatekey = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"publickey") ))
		{
			if ( wptr->value ) { sptr->publickey = allocate_string(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ s e c u r i t y 	*/
/*	------------------------------------------------	*/
public int rest_occi_cords_security(FILE * fh,struct cords_security * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.identity='%s'\r\n",prefix,nptr,(sptr->identity?sptr->identity:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.nature='%s'\r\n",prefix,nptr,(sptr->nature?sptr->nature:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.scheme='%s'\r\n",prefix,nptr,(sptr->scheme?sptr->scheme:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.privatekey='%s'\r\n",prefix,nptr,(sptr->privatekey?sptr->privatekey:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.publickey='%s'\r\n",prefix,nptr,(sptr->publickey?sptr->publickey:""));
	return(0);

}

#endif	/* _security_c_ */
