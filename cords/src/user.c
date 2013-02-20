/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _user_c_
#define _user_c_

#include "element.h"

#include "user.h"

/*	--------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ u s e r 	*/
/*	--------------------------------------	*/
public struct cords_user * liberate_cords_user(struct cords_user * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->password )
			 sptr->password = liberate(sptr->password);
		if ( sptr->role )
			 sptr->role = liberate(sptr->role);
		if ( sptr->email )
			 sptr->email = liberate(sptr->email);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->authorization )
			 sptr->authorization = liberate(sptr->authorization);
		sptr = liberate( sptr );
	}
	return((struct cords_user *) 0);

}

/*	--------------------------------	*/
/*	r e s e t _ c o r d s _ u s e r 	*/
/*	--------------------------------	*/
public struct cords_user * reset_cords_user(struct cords_user * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct cords_user*) 0;
		sptr->next = (struct cords_user*) 0;
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->password = (char*) 0;
		sptr->role = (char*) 0;
		sptr->email = (char*) 0;
		sptr->account = (char*) 0;
		sptr->authorization = (char*) 0;
		sptr->when =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ u s e r 	*/
/*	--------------------------------------	*/
public struct cords_user * allocate_cords_user()
{
	struct cords_user * sptr;
	if (!( sptr = allocate( sizeof( struct cords_user ) ) ))
		return( sptr );
	else	return( reset_cords_user(sptr) );
}

/*	--------------------------------	*/
/*	x m l i n _ c o r d s _ u s e r 	*/
/*	--------------------------------	*/
public int xmlin_cords_user(struct cords_user * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"password") ))
		{
			if ( wptr->value ) { sptr->password = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"role") ))
		{
			if ( wptr->value ) { sptr->role = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"email") ))
		{
			if ( wptr->value ) { sptr->email = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"authorization") ))
		{
			if ( wptr->value ) { sptr->authorization = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"when") ))
		{
			if ( wptr->value ) { sptr->when = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ u s e r 	*/
/*	----------------------------------------	*/
public int rest_occi_cords_user(FILE * fh,struct cords_user * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.password='%s'\r\n",prefix,nptr,(sptr->password?sptr->password:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.role='%s'\r\n",prefix,nptr,(sptr->role?sptr->role:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.email='%s'\r\n",prefix,nptr,(sptr->email?sptr->email:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.authorization='%s'\r\n",prefix,nptr,(sptr->authorization?sptr->authorization:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.when='%u'\r\n",prefix,nptr,sptr->when);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _user_cuser_c_ */
