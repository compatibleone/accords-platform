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
#ifndef _agency_c_
#define _agency_c_

#include "element.h"

#include "agency.h"

/*	----------------------------------	*/
/*	l i b e r a t e _ a g e n c y 	*/
/*	----------------------------------	*/
public struct agency * liberate_agency(struct agency * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->servicename )
			 sptr->servicename = liberate(sptr->servicename);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->identity )
			 sptr->identity = liberate(sptr->identity);
		if ( sptr->port )
			 sptr->port = liberate(sptr->port);
		if ( sptr->domain )
			 sptr->domain = liberate(sptr->domain);
		if ( sptr->publisher )
			 sptr->publisher = liberate(sptr->publisher);
		if ( sptr->user )
			 sptr->user = liberate(sptr->user);
		if ( sptr->password )
			 sptr->password = liberate(sptr->password);
		sptr = liberate( sptr );
	}
	return((struct agency *) 0);

}

/*	----------------------------	*/
/*	r e s e t _ a g e n c y 	*/
/*	----------------------------	*/
public struct agency * reset_agency(struct agency * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->servicename = (char*) 0;
		sptr->description = (char*) 0;
		sptr->identity = (char*) 0;
		sptr->port = (char*) 0;
		sptr->domain = (char*) 0;
		sptr->publisher = (char*) 0;
		sptr->user = (char*) 0;
		sptr->password = (char*) 0;
		sptr->process =  0;
		sptr->start =  0;
		sptr->uptime =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------	*/
/*	a l l o c a t e _ a g e n c y 	*/
/*	----------------------------------	*/
public struct agency * allocate_agency()
{
	struct agency * sptr;
	if (!( sptr = allocate( sizeof( struct agency ) ) ))
		return( sptr );
	else	return( reset_agency(sptr) );
}

/*	----------------------------	*/
/*	x m l i n _ a g e n c y 	*/
/*	----------------------------	*/
public int xmlin_agency(struct agency * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"servicename") ))
		{
			if ( wptr->value ) { sptr->servicename = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"identity") ))
		{
			if ( wptr->value ) { sptr->identity = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"port") ))
		{
			if ( wptr->value ) { sptr->port = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"domain") ))
		{
			if ( wptr->value ) { sptr->domain = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"publisher") ))
		{
			if ( wptr->value ) { sptr->publisher = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"user") ))
		{
			if ( wptr->value ) { sptr->user = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"password") ))
		{
			if ( wptr->value ) { sptr->password = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"process") ))
		{
			if ( wptr->value ) { sptr->process = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"start") ))
		{
			if ( wptr->value ) { sptr->start = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"uptime") ))
		{
			if ( wptr->value ) { sptr->uptime = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------	*/
/*	r e s t _ o c c i _ a g e n c y 	*/
/*	------------------------------------	*/
public int rest_occi_agency(FILE * fh,struct agency * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.servicename='%s'\r\n",prefix,nptr,(sptr->servicename?sptr->servicename:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.identity='%s'\r\n",prefix,nptr,(sptr->identity?sptr->identity:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.port='%s'\r\n",prefix,nptr,(sptr->port?sptr->port:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.domain='%s'\r\n",prefix,nptr,(sptr->domain?sptr->domain:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.publisher='%s'\r\n",prefix,nptr,(sptr->publisher?sptr->publisher:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.user='%s'\r\n",prefix,nptr,(sptr->user?sptr->user:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.password='%s'\r\n",prefix,nptr,(sptr->password?sptr->password:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.process='%u'\r\n",prefix,nptr,sptr->process);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.start='%u'\r\n",prefix,nptr,sptr->start);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.uptime='%u'\r\n",prefix,nptr,sptr->uptime);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _agency_c_ */
