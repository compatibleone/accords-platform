/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _gw_c_
#define _gw_c_

#include "element.h"

#include "gw.h"

/*	----------------------	*/
/*	l i b e r a t e _ g w 	*/
/*	----------------------	*/
public struct gw * liberate_gw(struct gw * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->publicaddr )
			 sptr->publicaddr = liberate(sptr->publicaddr);
		if ( sptr->privateaddr )
			 sptr->privateaddr = liberate(sptr->privateaddr);
		if ( sptr->ethername )
			 sptr->ethername = liberate(sptr->ethername);
		if ( sptr->intercloudGW )
			 sptr->intercloudGW = liberate(sptr->intercloudGW);
		if ( sptr->contract )
			 sptr->contract = liberate(sptr->contract);
		if ( sptr->provider_type )
			 sptr->provider_type = liberate(sptr->provider_type);
		if ( sptr->provider_platform )
			 sptr->provider_platform = liberate(sptr->provider_platform);
		if ( sptr->connection )
			 sptr->connection = liberate(sptr->connection);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->state )
			 sptr->state = liberate(sptr->state);
		sptr = liberate( sptr );
	}
	return((struct gw *) 0);

}

/*	----------------	*/
/*	r e s e t _ g w 	*/
/*	----------------	*/
public struct gw * reset_gw(struct gw * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->publicaddr = (char*) 0;
		sptr->privateaddr = (char*) 0;
		sptr->ethername = (char*) 0;
		sptr->intercloudGW = (char*) 0;
		sptr->contract = (char*) 0;
		sptr->provider_type = (char*) 0;
		sptr->provider_platform = (char*) 0;
		sptr->connection = (char*) 0;
		sptr->account = (char*) 0;
		sptr->state = (char*) 0;
	}
	return(sptr);

}

/*	----------------------	*/
/*	a l l o c a t e _ g w 	*/
/*	----------------------	*/
public struct gw * allocate_gw()
{
	struct gw * sptr;
	if (!( sptr = allocate( sizeof( struct gw ) ) ))
		return( sptr );
	else	return( reset_gw(sptr) );
}

/*	----------------	*/
/*	x m l i n _ g w 	*/
/*	----------------	*/
public int xmlin_gw(struct gw * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"publicaddr") ))
		{
			if ( wptr->value ) { sptr->publicaddr = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"privateaddr") ))
		{
			if ( wptr->value ) { sptr->privateaddr = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"ethername") ))
		{
			if ( wptr->value ) { sptr->ethername = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"intercloudGW") ))
		{
			if ( wptr->value ) { sptr->intercloudGW = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"contract") ))
		{
			if ( wptr->value ) { sptr->contract = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"provider_type") ))
		{
			if ( wptr->value ) { sptr->provider_type = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"provider_platform") ))
		{
			if ( wptr->value ) { sptr->provider_platform = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"connection") ))
		{
			if ( wptr->value ) { sptr->connection = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = allocate_string(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------	*/
/*	r e s t _ o c c i _ g w 	*/
/*	------------------------	*/
public int rest_occi_gw(FILE * fh,struct gw * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.publicaddr='%s'\r\n",prefix,nptr,(sptr->publicaddr?sptr->publicaddr:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.privateaddr='%s'\r\n",prefix,nptr,(sptr->privateaddr?sptr->privateaddr:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.ethername='%s'\r\n",prefix,nptr,(sptr->ethername?sptr->ethername:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.intercloudGW='%s'\r\n",prefix,nptr,(sptr->intercloudGW?sptr->intercloudGW:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.contract='%s'\r\n",prefix,nptr,(sptr->contract?sptr->contract:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.provider_type='%s'\r\n",prefix,nptr,(sptr->provider_type?sptr->provider_type:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.provider_platform='%s'\r\n",prefix,nptr,(sptr->provider_platform?sptr->provider_platform:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.connection='%s'\r\n",prefix,nptr,(sptr->connection?sptr->connection:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%s'\r\n",prefix,nptr,(sptr->state?sptr->state:""));
	return(0);

}

#endif	/* _gw_cgw_c_ */
