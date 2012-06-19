/* ------------------------------------------------------------------------------------ */
/*                               CompatibleOne Cloudware                                */
/* ------------------------------------------------------------------------------------ */
/*                                                                                      */
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a        */
/* disposition selon les termes de la licence Creative Commons Paternit<82> :           */
/*                                                                                      */
/*                              Pas d'Utilisation Commerciale                           */
/*                              Pas de Modification                                     */
/*                              3.0 non transcrit.                                      */
/*                                                                                      */
/* ------------------------------------------------------------------------------------ */
/*                      Copyright (c) 2011 Iain James Marshall for Prologue             */
/*                                 All rights reserved                                  */
/* ------------------------------------------------------------------------------------ */

#ifndef _ec2config_c_
#define _ec2config_c_
#include "element.h"
#include "ec2config.h"

/*****************************************************************************************/
//                               Liberate_cords_ec2config
/*****************************************************************************************/
public struct cords_ec2config * liberate_cords_ec2config(struct cords_ec2config * sptr)
{
	if( sptr )
	{
		if(sptr->id)
			sptr->id = liberate(sptr->id);
		if(sptr->name)
			sptr->name = liberate(sptr->name);
		if(sptr->description)
			sptr->description = liberate(sptr->description);
		if(sptr->accesskey)
			sptr->accesskey = liberate(sptr->accesskey);
		if(sptr->secretkey)
			sptr->secretkey = liberate(sptr->secretkey);
		if(sptr->authenticate)
			sptr->authenticate = liberate(sptr->authenticate);
		if(sptr->agent)
			sptr->agent = liberate(sptr->agent);
		if(sptr->host)
			sptr->host = liberate(sptr->host);
		if(sptr->version)
			sptr->version = liberate(sptr->version);
		if(sptr->namespace)
			sptr->namespace = liberate(sptr->namespace);
		if(sptr->base)
			sptr->base = liberate(sptr->base);
		if(sptr->tls)
			sptr->tls = liberate(sptr->tls);
		if(sptr->current)
			sptr->current = liberate(sptr->current);
		sptr = liberate(sptr);
	}
	return((struct cords_ec2config * )0);
}

/*****************************************************************************************/
//                            Reset_cords_ec2config
/*****************************************************************************************/
public struct cords_ec2config * reset_cords_ec2config(struct cords_ec2config * sptr)
{
	if( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->description = (char*) 0;
		sptr->accesskey = (char*) 0;
		sptr->secretkey = (char*) 0;
		sptr->authenticate = (char*) 0;
		sptr->agent = (char*) 0;
		sptr->host = (char*) 0;
		sptr->version = (char*) 0;
		sptr->namespace = (char*) 0;
		sptr->base = (char*) 0;
		sptr->tls = (char*) 0;
		sptr->current = (char*) 0;
	}
	return (sptr);
}

/*****************************************************************************************/
//                            Allocate_cords_ec2config
/*****************************************************************************************/
public struct cords_ec2config * allocate_cords_ec2config()
{
	struct cords_ec2config * sptr;
	if (!( sptr = allocate( sizeof( struct cords_ec2config ) ) ))
		return( sptr );
	else  return( reset_cords_ec2config(sptr) );
}

/*****************************************************************************************/
//                             Xmlin_cords_ec2config
/*****************************************************************************************/
public int xmlin_cords_ec2config(struct cords_ec2config * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"name") ))
		{
			if ( wptr->value ) { sptr->name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"accesskey") ))
		{
			if ( wptr->value ) { sptr->accesskey = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"secretkey") ))
		{
			if ( wptr->value ) { sptr->secretkey = allocate_string(wptr->value); }
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
			if ( wptr->value ) { sptr->current = allocate_string(wptr->value); }
		}
	}
	return(0);
}

/*****************************************************************************************/
//                         Rest_occi_cords_ec2config
/*****************************************************************************************/
public int rest_occi_cords_ec2config(FILE * fh,struct cords_ec2config * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.accesskey='%s'\r\n",prefix,nptr,(sptr->accesskey?sptr->accesskey:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.secretkey='%s'\r\n",prefix,nptr,(sptr->secretkey?sptr->secretkey:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.authenticate='%s'\r\n",prefix,nptr,(sptr->authenticate?sptr->authenticate:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.agent='%s'\r\n",prefix,nptr,(sptr->agent?sptr->agent:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.host='%s'\r\n",prefix,nptr,(sptr->host?sptr->host:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.version='%s'\r\n",prefix,nptr,(sptr->version?sptr->version:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.namespace='%s'\r\n",prefix,nptr,(sptr->namespace?sptr->namespace:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.base='%s'\r\n",prefix,nptr,(sptr->base?sptr->base:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.tls='%s'\r\n",prefix,nptr,(sptr->tls?sptr->tls:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.current='%s'\r\n",prefix,nptr,(sptr->current?sptr->current:""));
	return(0);
}

#endif
