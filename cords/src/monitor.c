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
#ifndef _monitor_c_
#define _monitor_c_

#include "element.h"

#include "monitor.h"

/*	--------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ m o n i t o r 	*/
/*	--------------------------------------------	*/
public struct cords_monitor * liberate_cords_monitor(struct cords_monitor * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->service )
			 sptr->service = liberate(sptr->service);
		if ( sptr->type )
			 sptr->type = liberate(sptr->type);
		if ( sptr->report )
			 sptr->report = liberate(sptr->report);
		sptr = liberate( sptr );
	}
	return((struct cords_monitor *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ c o r d s _ m o n i t o r 	*/
/*	--------------------------------------	*/
public struct cords_monitor * reset_cords_monitor(struct cords_monitor * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->service = (char*) 0;
		sptr->type = (char*) 0;
		sptr->report = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ m o n i t o r 	*/
/*	--------------------------------------------	*/
public struct cords_monitor * allocate_cords_monitor()
{
	struct cords_monitor * sptr;
	if (!( sptr = allocate( sizeof( struct cords_monitor ) ) ))
		return( sptr );
	else	return( reset_cords_monitor(sptr) );
}

/*	--------------------------------------	*/
/*	x m l i n _ c o r d s _ m o n i t o r 	*/
/*	--------------------------------------	*/
public int xmlin_cords_monitor(struct cords_monitor * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"service") ))
		{
			if ( wptr->value ) { sptr->service = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"type") ))
		{
			if ( wptr->value ) { sptr->type = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"report") ))
		{
			if ( wptr->value ) { sptr->report = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ m o n i t o r 	*/
/*	----------------------------------------------	*/
public int rest_occi_cords_monitor(FILE * fh,struct cords_monitor * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.service='%s'\r\n",prefix,nptr,(sptr->service?sptr->service:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.type='%s'\r\n",prefix,nptr,(sptr->type?sptr->type:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.report='%s'\r\n",prefix,nptr,(sptr->report?sptr->report:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _monitor_c_ */
