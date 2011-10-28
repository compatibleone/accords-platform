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
#ifndef _storage_c_
#define _storage_c_

#include "element.h"

#include "storage.h"

/*	--------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ s t o r a g e 	*/
/*	--------------------------------------------	*/
public struct cords_storage * liberate_cords_storage(struct cords_storage * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->size )
			 sptr->size = liberate(sptr->size);
		sptr = liberate( sptr );
	}
	return((struct cords_storage *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ c o r d s _ s t o r a g e 	*/
/*	--------------------------------------	*/
public struct cords_storage * reset_cords_storage(struct cords_storage * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->size = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ s t o r a g e 	*/
/*	--------------------------------------------	*/
public struct cords_storage * allocate_cords_storage()
{
	struct cords_storage * sptr;
	if (!( sptr = allocate( sizeof( struct cords_storage ) ) ))
		return( sptr );
	else	return( reset_cords_storage(sptr) );
}

/*	--------------------------------------	*/
/*	x m l i n _ c o r d s _ s t o r a g e 	*/
/*	--------------------------------------	*/
public int xmlin_cords_storage(struct cords_storage * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"size") ))
		{
			if ( wptr->value ) { sptr->size = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ s t o r a g e 	*/
/*	----------------------------------------------	*/
public int rest_occi_cords_storage(FILE * fh,struct cords_storage * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.size='%s'\r\n",prefix,nptr,(sptr->size?sptr->size:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _storage_c_ */
