#ifndef _colog_c
#define _colog_c

#include <stdio.h>

#ifndef	public
#define	public
#endif
#ifndef	private
#define	private	static
#endif

private void    colog_flush_page( int mode );
private	int	colog_sent_request(int when,int pid,int tid,char * who,int dir, char * method, char * object);
private	int	colog_received_request(int when,int pid,int tid,char * who,int dir, char * method, char * object);
private	int	colog_sent_response(int when,int pid,int tid,char * who,int dir, char * object);
private	int	colog_received_response(int when,int pid,int tid,char * who,int dir, char * object);
private	FILE *	colog_handle=(FILE*) 0;
private	int	colog_page=0;
private	int	colog_lastpage=0;
private	int	colog_events=0;
private	char *	colog_output="colog";
private	int	colog_page_size=48;

int	maxcolumns=0;

struct	colog_reponse;

struct	colog_module
{
	struct	colog_module * previous;
	struct	colog_module * next;
	struct	colog_request * first;
	struct 	colog_request * last;
	int	trash;
	int	pid;
	char *	name;
	char *	host;
	int	port;
	char *	object;
	int	column;
	struct colog_event * event;
};

struct	colog_event
{
	struct	colog_event * previous;
	struct	colog_event * next;
	struct	colog_module * from;
	struct 	colog_module * to;
	char *	method;
	char *	object;
	int	when;
	int	number;
	int	dir;
	int	status;
	char *	message;
	int	response;
};

struct	colog_request
{
	struct	colog_request 	* previous;
	struct	colog_request 	* next;
	struct	colog_request	* received;
	struct	colog_response	* response;
	struct	colog_event   	* event;
	struct	colog_module  	* from;
	struct	colog_module  	* to;
	int	when;
	int	pid;
	int	tid;
	char *	method;
	struct url * object;
};

struct	colog_response
{
	struct	colog_response  * previous;
	struct	colog_response  * next;
	struct	colog_request	* request;
	struct	colog_event	* event;
	struct	colog_module  	* from;
	struct	colog_module  	* to;
	int	when;
	int	pid;
	int	tid;
	int	received;
	char *	version;
	int	status;
	char *	message;
};

struct	colog_analsis
{
	struct colog_module * FirstTrash;
	struct colog_module * LastTrash;
	struct colog_module * FirstModule;
	struct colog_module * LastModule;
	struct colog_event * FirstEvent;
	struct colog_event * LastEvent;
	struct colog_request * FirstRequest;
	struct colog_request * LastRequest;
} 
Manager = 
{
	(struct colog_module *) 0,
	(struct colog_module *) 0,
	(struct colog_module *) 0,
	(struct colog_module *) 0,
	(struct colog_event *) 0,
	(struct colog_event *) 0,
	(struct colog_request *) 0,
	(struct colog_request *) 0
};

/*	---------------------------------	*/
/*	      d r o p _ m o d u l e 		*/
/*	---------------------------------	*/
private	void	drop_module( struct colog_module * mptr )
{
	if ( mptr )
	{
		if (!( mptr->previous ))
			Manager.FirstModule = mptr->next;
		else	mptr->previous->next = mptr->next;
		if (!( mptr->next ))
			Manager.LastModule = mptr->previous;
		else	mptr->next->previous = mptr->previous;
	}
}

/*	---------------------------------	*/
/*	  a l l o c a t e _ m o d u l e		*/
/*	---------------------------------	*/
private	struct colog_module * allocate_module()
{
	struct	colog_module * mptr;
	if (!( mptr = (struct colog_module *) allocate( sizeof( struct colog_module ) )))
		return( mptr );
	else
	{
		memset(mptr,0,sizeof( struct colog_module ));
		return( mptr );
	}
}

/*	---------------------------------	*/
/*	 l i b e r a t e _ r e q u e s t 	*/
/*	---------------------------------	*/
private	struct colog_request * liberate_request(struct colog_request * rptr)
{
	if ( rptr )
	{
		if ( rptr->method )
			rptr->method = liberate( rptr->method );

		if ( rptr->object )
			rptr->object = liberate_url( rptr->object );

		liberate( rptr );
	}
	return((struct colog_request *) 0);
}

/*	---------------------------------	*/
/*	 a l l o c a t e _ r e q u e s t	*/
/*	---------------------------------	*/
private	struct colog_request * allocate_request()
{
	struct	colog_request * rptr;
	if (!( rptr = (struct colog_request *) allocate( sizeof( struct colog_request ) )))
		return( rptr );
	else
	{
		memset(rptr,0,sizeof( struct colog_request ));
		return( rptr );
	}
}

/*	---------------------------------	*/
/*	l i b e r a t e _ r e s p o n s e	*/
/*	---------------------------------	*/
private	struct colog_response * liberate_response(struct colog_response * rptr)
{
	if ( rptr )
	{
		liberate( rptr );
	}
	return((struct colog_response *) 0);
}

/*	---------------------------------	*/
/*	a l l o c a t e _ r e s p o n s e	*/
/*	---------------------------------	*/
private	struct colog_response * allocate_response()
{
	struct	colog_response * rptr;
	if (!( rptr = (struct colog_response *) allocate( sizeof( struct colog_response ) )))
		return( rptr );
	else
	{
		memset(rptr,0,sizeof( struct colog_response ));
		return( rptr );
	}
}

/*	---------------------------------	*/
/*	   a l l o c a t e _ e v e n t		*/
/*	---------------------------------	*/
private	struct colog_event * allocate_event()
{
	struct	colog_event * eptr;
	if (!( eptr = (struct colog_event *) allocate( sizeof( struct colog_event ) )))
		return( eptr );
	else
	{
		memset(eptr,0,sizeof( struct colog_event ));
		eptr->number = ++colog_events;
		return( eptr );
	}
}

/*	-----------------------------------------	*/
/*	 l o c a t e _ m o d u l e  b y _ p i d 	*/
/*	----------------------------------------	*/
private	struct	colog_module * locate_module_by_pid( int pid )
{
	struct	colog_module * mptr;
	for (	mptr=Manager.FirstModule;
			mptr != (struct colog_module *) 0;
			mptr = mptr->next )
	{
		if ( mptr->pid != pid )
			continue;
		else	break;
	}
	return( mptr );
}

/*	---------------------------------	*/
/*	   r e s o l v e _ b y _ p i d 		*/
/*	---------------------------------	*/
private	struct	colog_module * resolve_module_by_pid( int pid )
{
	struct	colog_module * mptr;
	if ((mptr = locate_module_by_pid( pid )) != (struct colog_module *) 0)
		return( mptr );
	else if (!( mptr = allocate_module()))
			return( mptr );
	else 
	{
		mptr->pid = pid;
		if (!( mptr->previous = Manager.LastModule ))
			Manager.FirstModule = mptr;
		else	mptr->previous->next = mptr;
		Manager.LastModule = mptr;
		return( mptr );
	}
}

/*	---------------------------------	*/
/*	r e s o l v e _ b y _ o b j e c t	*/
/*	---------------------------------	*/
private	struct	colog_module * resolve_module_by_object( char * object )
{
	struct	colog_module * mptr;
	for (	mptr=Manager.FirstModule;
		mptr != (struct colog_module *) 0;
		mptr = mptr->next )
	{
		if (!( mptr->object ))
			continue;
		else if ( strcmp( mptr->object, object ) != 0 )
			continue;
		else	break;
	}
	return( mptr );
}

/*	---------------------------------	*/
/*	   r e s o l v e _ b y _ u r l 		*/
/*	---------------------------------	*/
private	struct	colog_module * resolve_module_by_url( struct url * uptr )
{
	struct	colog_module * mptr;

	if (!( uptr ))
		return((struct colog_module *) 0);
	else if (!( uptr->host ))
	{
		if (( mptr = resolve_module_by_object( uptr->object )))
			return( mptr );
		else if (!( mptr = allocate_module()))
			return( mptr );
		else if (!( mptr->object = allocate_string( uptr->object ) ))
			return((struct colog_module *) 0 );
		else
		{
			if (!( mptr->previous = Manager.LastModule ))
				Manager.FirstModule = mptr;
			else	mptr->previous->next = mptr;
			Manager.LastModule = mptr;
			return( mptr );
		}
	}
	else
	{
		for (	mptr=Manager.FirstModule;
			mptr != (struct colog_module *) 0;
			mptr = mptr->next )
		{
			if (!( mptr->host ))
				continue;
			else if ( strcmp( mptr->host, uptr->host ) != 0 )
				continue;
			else if ( mptr->port != uptr->port )
				continue;
			else	return( mptr );
		}
	}
	if (!( mptr = allocate_module()))
		return( mptr );
	else if (!( mptr->host = allocate_string( uptr->host ) ))
		return((struct colog_module *) 0 );
	else
	{
		mptr->port = uptr->port;
		if (!( mptr->previous = Manager.LastModule ))
			Manager.FirstModule = mptr;
		else	mptr->previous->next = mptr;
		Manager.LastModule = mptr;
		return( mptr );
	}
}

/*	-------------------------------------------	*/
/*	r e s o l v e _ r e q u e s t _ b y _ u r l 	*/
/*	-------------------------------------------	*/
private	struct	colog_request * resolve_request_by_url( struct url * optr )
{
	struct	url * uptr;
	struct	colog_request * rptr;
	for (	rptr=Manager.FirstRequest;
		rptr != (struct colog_request *) 0;
		rptr = rptr->next )
	{
		if (!( uptr = rptr->object ))
			continue;
		if (!( uptr->host ))
			continue;
		else if ( strcmp( uptr->host, optr->host ) != 0 )
			continue;
		else if ( uptr->port != optr->port )
			continue;
		else	return( rptr );
	}
	return( rptr );
}

/*	----------------------------------	*/
/*	   r e s o l v e _ r e q u e s t 	*/
/*	----------------------------------	*/
private	struct	colog_request * resolve_request( char * method, char * object )
{
	struct	colog_request * rptr;
	for (	rptr=Manager.FirstRequest;
		rptr != (struct colog_request *) 0;
		rptr = rptr->next )
	{
		if ( rptr->received )
			continue;
		else if (!( rptr->method ))
			continue;
		else if ( strcasecmp( rptr->method, method ) )
			continue;
		else if (!( rptr->object ))
			continue;
		else if (!( rptr->object->object ))
			continue;
		else if (!( strcmp( rptr->object->object, object ) ))
			break;
	}
	return( rptr );
}

/*	----------------------------------	*/
/*	 r e s o l v e _ r e s p on d e d 	*/
/*	----------------------------------	*/
private	struct	colog_request * resolve_responded( int pid, int tid )
{
	struct	colog_request * rptr;
	struct	colog_request * qptr;
	for (	rptr=Manager.FirstRequest;
		rptr != (struct colog_request *) 0;
		rptr = rptr->next )
	{
		if (!(qptr = rptr->received ))
			continue;
		else if (!( qptr->response ))
			continue;
		else if ( rptr->pid != pid )
			continue;
		else if ( rptr->tid != tid )
			continue;
		else	break;
	}
	return( rptr );
}

/*	----------------------------------	*/
/*	 r e s o l v e _ r e c e i v e d	*/
/*	----------------------------------	*/
private	struct	colog_request * resolve_received( int pid, int tid )
{
	struct	colog_request  * qptr;
	struct	colog_response * aptr;
	struct	colog_request  * rptr;
	for (	qptr=Manager.FirstRequest;
		qptr != (struct colog_request *) 0;
		qptr = qptr->next )
	{
		/* ----------------------------- */
		/* eliminate if not yet received */
		/* ----------------------------- */
		if (!(rptr = qptr->received ))
			continue;
		/* -------------------------- */
		/* eliminate if response sent */
		/* -------------------------- */
		else if ((aptr = rptr->response) != (struct colog_response *) 0)
			continue;
		/* ------------------------- */
		/* ensure correct process id */
		/* ------------------------- */
		else if ( rptr->pid != pid )
			continue;
		/* ------------------------- */
		/* ensure correct  thread id */
		/* ------------------------- */
		else if ( rptr->tid != tid )
			continue;
		else	break;
	}
	return( qptr );
}

/*	---------------------------------	*/
/*	   c o l o g _ s c a n p a s t		*/
/*	---------------------------------	*/
private	char *	scanpast( char * mptr, int c )
{
	if (!( mptr ))
		return( mptr );
	while ( *mptr )
	{
		if ( *mptr == c )
		{
			*(mptr++) = 0;
			break;
		}
		else	mptr++;
	}
	return( mptr );
}

/*	---------------------------------	*/
/*	c o l o g _ h t t p _ m e t h o d	*/
/*	---------------------------------	*/
private	int	colog_http_method( char * mptr )
{
	if (!( strcasecmp( mptr, "GET" ) ))
		return( 1 );
	else if (!( strcasecmp( mptr, "PUT" ) ))
		return( 2 );
	else if (!( strcasecmp( mptr, "POST" ) ))
		return( 3 );
	else if (!( strcasecmp( mptr, "DELETE" ) ))
		return( 4 );
	else if (!( strcasecmp( mptr, "HEAD" ) ))
		return( 5 );
	else if (!( strcasecmp( mptr, "HTTP/1.1" ) ))
		return( 6 );
	else if (!( strcasecmp( mptr, "HTTP/1.0" ) ))
		return( 6 );
	else	return( 0 );
}

/*	---------------------------------	*/
/*	 c o l o g _ s e n t _ e v e n t  	*/
/*	---------------------------------	*/
private	int	colog_sent_event( struct colog_event * eptr, char * wptr )
{
	struct	colog_module * mptr;
	struct	url * uptr;
	char *	method;
	char *	object;
	int	what;
	while ( *wptr == ' ' ) wptr++;
	wptr = scanpast(method=wptr,' ');
	if (!( what = colog_http_method( method ) ))
		return(30);
	else if (!( eptr->method = allocate_string( method )))
		return(27);
	while ( *wptr == ' ' ) wptr++;
	wptr = scanpast(object=wptr,' ');

	/* detect sent response */
	if ( what > 5 ) 
	{
		eptr->response = 1;
		return(31);
	}

	if (!( uptr = analyse_url( object )))
		return(31);

	else if (!( mptr = resolve_module_by_url( uptr )))
	{
		uptr = liberate_url( uptr );
		return(31);
	}
	else if (!( mptr->object = allocate_string( uptr->object ) ))
	{
		uptr = liberate_url( uptr );
		return(27);
	}
	else
	{
		mptr->event = eptr;
		eptr->to = mptr;
		uptr = liberate_url( uptr );
		return(0);
	}
}

/*	---------------------------------------		*/
/*	c o l o g _ r e c e i v e d _ e v e n t 	*/
/*	---------------------------------------		*/
private	int	colog_received_event( struct colog_event * eptr, char * wptr )
{
	struct	colog_module * mptr;
	struct	colog_module * self;
	struct	url * uptr;
	char *	method;
	char *	object;
	int	what;
	if (!( self = eptr->from ))
		return(118);
	while ( *wptr == ' ' ) wptr++;
	wptr = scanpast(method=wptr,' ');
	if (!( what = colog_http_method( method ) ))
		return(30);
	else if (!( eptr->method = allocate_string( method )))
		return(27);
	while ( *wptr == ' ' ) wptr++;
	wptr = scanpast(object=wptr,' ');

	/* detect received response */
	if ( what > 5 ) 
	{
		eptr->response = 1;
		return(31);
	}

	if (!( self->host ))
	{
		if (!( mptr = resolve_module_by_object( object ) ))
			return(78);
		else if (!( mptr->host ))
			return(118);
		else if (!( self->host = allocate_string( mptr->host ) ))
			return(27);
		else 
		{
			self->port = mptr->port;
			if (mptr->event != (struct colog_event *) 0)
			{
				mptr->event->to = self;
				eptr->to = mptr->event->from;
			}
			drop_module( mptr );
			if ( self->object )
				self->object = liberate( self->object );
		}
	}
	return(0);
}

/*	---------------------------------	*/
/*	  c o l o g _ u s e _ e v e n t 	*/
/*	---------------------------------	*/
private	int	colog_use_event( struct colog_module * mptr, char * wptr, int when, int dir )
{
	struct colog_event * eptr;
	if (!( eptr = allocate_event() ))
		return( 27 );
	else if (!( eptr->previous = Manager.LastEvent ))
		Manager.FirstEvent = eptr;
	else	eptr->previous->next = eptr;

	Manager.LastEvent = eptr;
	eptr->when = when;
	eptr->dir  = dir;
	eptr->from = mptr;

	/* sending request */
	if ( dir == 1 )
		colog_sent_event( eptr, wptr );
	else if ( dir == 2 )
		colog_received_event( eptr, wptr );
	return( 0 );
}

/*	-------------------------------------	*/
/*	c o l o g _ r e q u e s t _ e v e n t 	*/
/*	-------------------------------------	*/
private	int	colog_request_event( struct colog_request * rptr )
{
	struct colog_event * eptr;
	if (!( eptr = allocate_event() ))
		return( 27 );
	else if (!( eptr->method = allocate_string(rptr->method)))
		return(27);
	else if (!( eptr->previous = Manager.LastEvent ))
		Manager.FirstEvent = eptr;
	else	eptr->previous->next = eptr;
	Manager.LastEvent = eptr;

	eptr->when = rptr->when;
	eptr->dir  = 1;
	if (( rptr->object )
	&&  ( rptr->object->object))
		eptr->object = allocate_string( rptr->object->object );
	eptr->from = rptr->from;
	eptr->to   = rptr->to;
	rptr->event= eptr;
	return( 0 );
}

/*	--------------------------------------	*/
/*	c o l o g _ r e s p o n s e_ e v e n t 	*/
/*	--------------------------------------	*/
private	int	colog_response_event( struct colog_request * qptr,struct colog_response * aptr )
{
	struct colog_event * eptr;

	if (!( eptr = allocate_event() ))
		return( 27 );
	else if (!( eptr->previous = Manager.LastEvent ))
		Manager.FirstEvent = eptr;
	else	eptr->previous->next = eptr;

	Manager.LastEvent = eptr;
	eptr->when   = qptr->when;
	eptr->dir    = 2;
	eptr->status = aptr->status;
	if ( aptr->message )
		eptr->message = allocate_string( aptr->message );
	eptr->from   = qptr->to;
	eptr->to     = qptr->from;
	aptr->event  = eptr;
	return( 0 );
}

/*	-----------------------------------	*/
/*		d r o p _ m o d u l e		*/
/*	-----------------------------------	*/
private	void	forget_module( struct colog_module * mptr )
{
	if (!( mptr->trash ))
	{
		if (!( mptr->previous ))
			Manager.FirstModule = mptr->next;
		else	mptr->previous->next = mptr->next;
		if (!( mptr->next ))
			Manager.LastModule = mptr->previous;
		else	mptr->next->previous = mptr->previous;
		mptr->trash = 1;
		mptr->previous = mptr->next = (struct colog_module *) 0;
		if (!( mptr->previous = Manager.LastTrash ))
			Manager.FirstTrash = mptr;
		else	mptr->previous->next = mptr;
		Manager.LastTrash = mptr;
	}
	return;
}

/*	-------------------------------------------	*/
/*	c o l o g _ r e c e i v e d _ r e q u e s t	*/
/*	-------------------------------------------	*/
private	int	colog_received_request(int when,int pid,int tid,char * who,int dir, char * method, char * object)
{
	struct	colog_request * rptr;
	struct	colog_request * qptr;
	struct	colog_module  * mptr;
	struct	colog_module  * xptr;
	int	status;

	if (!( rptr = allocate_request()))
		return(27);
	else if (!( rptr->method = allocate_string( method ) ))
	{
		liberate_request( rptr );
		return(27);
	}
	else if (!( rptr->object = analyse_url( object ) ))
	{
		liberate_request( rptr );
		return(27);
	}
	else
	{
		rptr->when = when;
		rptr->pid  = pid;
		rptr->tid  = tid;
		/* ----------------------------------- */
		/* is the original request on the heap */
		/* ----------------------------------- */
		if (!( qptr = resolve_request( rptr->method, object )))
		{
			if ((status = colog_sent_request(when,0,0,"unknown",1,method, object )) != 0)
				return( status );
			else if (!( qptr = resolve_request( rptr->method, object )))
				return(78);
		}
		if (!( mptr = qptr->to ))
		{
			if (!( xptr = resolve_module_by_pid( pid ) ))
			{
				liberate_request( rptr );
				return( 27 );
			}
			else
			{
				qptr->to = xptr;
				if ( qptr->event )
					qptr->event->to = xptr;
			}
		}
		else if (!( mptr->pid )) 
		{
			if (!( xptr = locate_module_by_pid( pid ) ))
			{
				if (!(mptr->name = allocate_string( who ) ))
				{
					liberate_request( rptr );
					return( 27 );
				}
				else	mptr->pid = pid;
			}
			else
			{
				if ((!( xptr->host )) && ( mptr->host ))
				{
					if (!( xptr->host = allocate_string( mptr->host ) ))
					{
						liberate_request( rptr );
						return( 27 );
					}
					else	xptr->port = mptr->port;
				}				
				forget_module( mptr );
				qptr->to = xptr;
				if ( qptr->event )
					qptr->event->to = xptr;
			}
		}
		rptr->to = qptr->to;
		rptr->from = qptr->from;

		/* ----------------------- */
		/* would add an event here */
		/* ----------------------- */
		if ( check_verbose() )
			printf("recv request event(%u,%u,%u,%s,%s,%s)\n",when,pid,tid,who,method,object);		
		qptr->received = rptr;
		return(0);
	}
}

/*	----------------------------------------	*/
/*		d r o p _ r e q u e s t			*/
/*	----------------------------------------	*/
private	struct colog_request * drop_request( struct colog_request * rptr )
{
	struct	colog_request * qptr;
	struct	colog_response * aptr;
	
	if ( rptr )
	{
		if ( check_verbose() )
			printf("drop request\n");
		if ((qptr = rptr->received) != (struct colog_request *) 0)
		{
			if ((aptr = qptr->response) != (struct colog_response *) 0)
				aptr = liberate_response( aptr );
			qptr = liberate_request( qptr );
		}
		if (!( rptr->previous ))
			Manager.FirstRequest = rptr->next;
		else	rptr->previous->next = rptr->next;
		if (!( rptr->next ))
			Manager.LastRequest = rptr->previous;
		else	rptr->next->previous = rptr->previous;
		rptr = liberate_request( rptr );
	}
	return((struct colog_request *) 0);
}

/*	---------------------------------------------	*/
/*	c o l o g _ r e c e i v e d _ r e s p o n s e 	*/	
/*	---------------------------------------------	*/
private	int	colog_received_response(int when,int pid,int tid,char * who,int dir, char *wptr)
{
	struct	colog_request  * qptr;
	struct	colog_request  * rptr;
	struct	colog_response * aptr;
	/* --------------------------------------------------------------- */
	/* locate a request which has been received by this process.thread */
	/* --------------------------------------------------------------- */
	if (!( qptr = resolve_responded( pid, tid ) ))
		return( 78 );
	else
	{
		/* ------------------------- */
		/* event would be added here */
		/* ------------------------- */
		if ( check_verbose() )
			printf("recv reponse event(%u,%u,%u,%s,%s)\n",when,pid,tid,who,wptr);
		drop_request( qptr );
		return(0);
	}
}

/*	-------------------------------------------	*/
/*	c o l o g _ r e c e i v e d _ m e s s a g e	*/	
/*	-------------------------------------------	*/
private	int	colog_received_message(int when,int pid,int tid,char * who,int dir, char *wptr)
{
	char *	method;
	int	what;

	/* ------------------------- */
	/* collect method or version */
	/* ------------------------- */
	while ( *wptr == ' ' ) wptr++;
	wptr = scanpast(method=wptr,' ');

	switch (( what = colog_http_method( method ) ))
	{
	case	1	:	/* GET 	*/
	case	2	:	/* POST */
	case	3	:	/* PUT 	*/
	case	4	:	/* DELETE */
	case	5	:	/* HEAD */
		return( colog_received_request( when, pid, tid, who, dir, method, wptr ) );
	case	6	:	/* HTTP/1.X */
		return( colog_received_response( when, pid, tid, who, dir, wptr ) );
	default		:
		return(30);
	}
}

/*	-----------------------------------	*/
/*	c o l o g _ s e n t _ r e q u e s t	*/	
/*	-----------------------------------	*/
private	int	colog_sent_request(int when,int pid,int tid,char * who,int dir, char * method, char * object)
{
	struct	colog_request * rptr;

	if (!( rptr = allocate_request()))
		return(27);
	else if (!( rptr->method = allocate_string( method ) ))
	{
		liberate_request( rptr );
		return(27);
	}
	else if (!( rptr->object = analyse_url( object ) ))
	{
		liberate_request( rptr );
		return(27);
	}
	else if (!( rptr->from = resolve_module_by_pid( pid ) ))
	{
		liberate_request( rptr );
		return( 27 );
	}
	else if ((!( rptr->from->name )) && (!( rptr->from->name = allocate_string( who ) )))
	{
		liberate_request( rptr );
		return( 27 );
	}
	else if (!( rptr->to = resolve_module_by_url( rptr->object ) ))
	{
		liberate_request( rptr );
		return( 27 );
	}
	else
	{
		rptr->when = when;
		rptr->pid  = pid;
		rptr->tid  = tid;
		if (!( rptr->previous = Manager.LastRequest ))
			Manager.FirstRequest = rptr;
		else	rptr->previous->next = rptr;
		Manager.LastRequest = rptr;
		if ( check_verbose() )
			printf("sent request event(%u,%u,%u,%s,%s,%s)\n",when,pid,tid,who,method,object);		
		return( colog_request_event( rptr ) );
	}
}

/*	-------------------------------------	*/
/*	c o l o g _ s e n t _ r e s p o n s e 	*/	
/*	-------------------------------------	*/
private	int	colog_sent_response(int when,int pid,int tid,char * who,int dir, char *wptr)
{
	struct	colog_request  * qptr;
	struct	colog_request  * rptr;
	struct	colog_response * aptr;
	char *	vptr;
	/* --------------------------------------------------------------- */
	/* locate a request which has been received by this process.thread */
	/* --------------------------------------------------------------- */
	if (!( qptr = resolve_received( pid, tid ) ))
		return( 78 );
	else if (!( rptr = qptr->received ))
		return( 118 );
	else if (!( aptr = allocate_response()))
		return( 27 );
	else
	{
		rptr->response = aptr;
		while ( *wptr == ' ' ) wptr++;	
		wptr = scanpast(vptr=wptr,' ');
		aptr->status = atoi(vptr);
		aptr->message = allocate_string( wptr );
		if ( check_verbose() )
			printf("sent reponse event(%u,%u,%u,%s,%s)\n",when,pid,tid,who,wptr);
		return( colog_response_event( qptr, aptr ) );
		return(0);
	}				
}

/*	-----------------------------------	*/
/*	c o l o g _ s e n t _ m e s s a g e	*/	
/*	-----------------------------------	*/
private	int	colog_sent_message(int when,int pid,int tid,char * who,int dir, char *wptr)
{
	char *	method;
	int	what;

	/* ------------------------- */
	/* collect method or version */
	/* ------------------------- */
	while ( *wptr == ' ' ) wptr++;
	wptr = scanpast(method=wptr,' ');

	switch (( what = colog_http_method( method ) ))
	{
	case	1	:	/* GET 	*/
	case	2	:	/* POST */
	case	3	:	/* PUT 	*/
	case	4	:	/* DELETE */
	case	5	:	/* HEAD */
		return( colog_sent_request( when, pid, tid, who, dir, method, wptr ) );
	case	6	:	/* HTTP/1.X */
		return( colog_sent_response( when, pid, tid, who, dir, wptr ) );
	default		:
		return(30);
	}
}

/*	---------------------------------	*/
/*	   c o l o g _ u s e _ l i n e		*/
/*	---------------------------------	*/
private	int	colog_use_line( char * buffer )
{
	char *	wptr=buffer;
	char *	vptr;
	int	when;
	int	what;
	int	thrd;
	char * 	who;
	int	dir;
	struct	colog_module * mptr;

	/* ------------------ */
	/* collect time stamp */
	/* ------------------ */
	wptr = scanpast(vptr=wptr,':');
	when = atoi(vptr);

	/* ------------------ */
	/* collect process id */
	/* ------------------ */
	wptr = scanpast(vptr=wptr,':');
	what = atoi(vptr);

	/* ------------------ */
	/* collect pthread id */
	/* ------------------ */
	wptr = scanpast(vptr=wptr,':');
	thrd = atoi(vptr);

	if (!( what )) return(0);

	/* ------------------ */
	/* collect modulename */
	/* ------------------ */
	while ( *wptr == ' ' ) wptr++;
	wptr = scanpast(who=wptr,'/');
	wptr = scanpast(wptr,' ');

	/* ------------------ */
	/* collect  direction */
	/* ------------------ */
	while ( *wptr == ' ' ) wptr++;
	wptr = scanpast(vptr=wptr,' ');
	while ( *wptr == ' ' ) wptr++;

	if (!( strcmp( vptr, "<--" ) ))
		colog_received_message(when,what,thrd,who,2,wptr);
	else if (!( strcmp( vptr, "-->" ) ))
		colog_sent_message(when,what,thrd,who,1,wptr);
	return(0);
}

/*	---------------------------------	*/
/*	   c o l o g _ u s e _ l i n e		*/
/*	---------------------------------	*/
private	int	colog_old_use_line( char * buffer )
{
	char *	wptr=buffer;
	char *	vptr;
	int	when;
	int	what;
	int	thrd;
	char * 	who;
	int	dir;
	struct	colog_module * mptr;
	wptr = scanpast(vptr=wptr,':');
	when = atoi(vptr);
	wptr = scanpast(vptr=wptr,':');
	what = atoi(vptr);
	wptr = scanpast(vptr=wptr,':');
	thrd = atoi(vptr);
	if (!( what )) return(0);
	while ( *wptr == ' ' ) wptr++;
	wptr = scanpast(who=wptr,'/');
	wptr = scanpast(wptr,' ');
	while ( *wptr == ' ' ) wptr++;
	wptr = scanpast(vptr=wptr,' ');
	while ( *wptr == ' ' ) wptr++;
	if (!( strcmp( vptr, "<--" ) ))
		dir = 2;
	else if (!( strcmp( vptr, "-->" ) ))
		dir = 1;
	else	dir = 0;
	if (!( mptr = resolve_module_by_pid( what ) ))
		return( 27 );
	else if ( mptr->name )
		return( colog_use_event( mptr, wptr, when, dir ));
	else if (!( mptr->name = allocate_string( who ) ))
		return( 27 );
	else
		return( colog_use_event( mptr, wptr, when, dir ));
}

/*	---------------------------------	*/
/*	  c o l o g _ o p e r a t i o n		*/
/*	---------------------------------	*/
private	int	colog_operation( char * mptr )
{
	FILE * h;
	int		status;
	char   buffer[2048];
	if (!( h = fopen( mptr, "r" ) ))
		return( 40 );
	else
	{
		while ( fgets( buffer, 2048, h ) > 0 )
			if ((status = colog_use_line( buffer )) != 0)
				break;
		fclose( h );
		return( 0 );
	}
}

/*	---------------------------------	*/
/*	c o l o g _ s h o w _ e v e n t s	*/
/*	---------------------------------	*/
private	void	colog_show_events()
{
	struct	colog_event * eptr;
	struct	colog_module * fptr;
	struct	colog_module * tptr;
	fprintf(colog_handle,"<p><table><tr><th class=ath colspan=3>COLOG Event List</th></tr>\n");
	fprintf(colog_handle,"<tr><th class=ath>Time<th class=ath>From<th class=ath>To</tr>\n");
	for (	eptr=Manager.FirstEvent;
		eptr !=(struct colog_event *) 0;
		eptr = eptr->next )
	{
		if ( eptr->response )
			continue;
		else if (!( eptr->dir ))
			continue;
		else if ( eptr->dir > 1 )
			continue;
		fprintf(colog_handle,"<tr><th class=ath>%u<td class=atd>%s %s<td class=atd> %s %s</tr>\n",
			eptr->when,
			( eptr->dir ? ( eptr->dir == 1 ? "Sent by" : "Received by" ) : " "),
			( (fptr = eptr->from) ? ( fptr->name ? fptr->name : "unknown") : "unknown" ),
			( eptr->dir ? ( eptr->dir == 1 ? "to" : "from" ) : " "),
			( (tptr = eptr->to) ? ( tptr->name ? tptr->name : 
			( tptr->host ? tptr->host : "unknown")) : "unknown" ));
	}
	fprintf(colog_handle,"</table><p>\n");
	return;
}

/*	---------------------------------	*/
/*	c o l o g _ s h o w _ m o d u l e	*/
/*	---------------------------------	*/
private	void	colog_show_modules()
{
	struct	colog_module * mptr;
	fprintf(colog_handle,"<p><table width='90%'><tr><th class=ath colspan=4>COLOG Module List</th></tr>\n");
	fprintf(colog_handle,"<tr><th class=ath>Number<th class=ath>Name<th class=ath>Process<th class=ath>URL</tr>\n");
	maxcolumns=0;
	for (	mptr=Manager.FirstModule;
		mptr !=(struct colog_module *) 0;
		mptr = mptr->next )
	{
		mptr->column = ++maxcolumns;

		fprintf(colog_handle,"<tr><th class=ath>%u<td class=atd>%s", mptr->column, ( mptr->name ? mptr->name : "&nbsp;" ));

		if ( mptr->pid )
			fprintf(colog_handle,"<td class=atd>%u",mptr->pid);
		else	fprintf(colog_handle,"<td class=atd>&nbsp;");

		fprintf(colog_handle,"<td class=atd>%s", ( mptr->host ? mptr->host : "&nbsp;" ));

		if ( mptr->port )
			fprintf(colog_handle,":%u",mptr->port);

		fprintf(colog_handle,"</tr>\n");
	}
	fprintf(colog_handle,"</table><p>\n");
	return;
}

/*	---------------------------------	*/
/*	c o l o g _ s h o w _ h e a d e r	*/
/*	---------------------------------	*/
private	void	colog_show_header(int top, int mode)
{
	struct	colog_module * mptr;
	fprintf(colog_handle,"<tr><th class=ath>#<th class=ath>");
	if ( top )
	{
		if ( colog_page > 1 )
		{
			fprintf(colog_handle,"<div align=center><a href='%s%u.html'>First</a></div>\n",
				colog_output, 1);
			fprintf(colog_handle,"<div align=center><a href='%s%u.html'>Page %u</a></div>\n",
				colog_output, colog_page - 1, colog_page - 1);
		}
		else if ( colog_page_size )
		{
			colog_lastpage = ((colog_events / colog_page_size) + 1);
		}
	}
	else if ( mode )
	{
		fprintf(colog_handle,"<div align=center><a href='%s%u.html'>Page %u</a></div>\n",
			colog_output,colog_page+1, colog_page+1);
		if ( colog_lastpage )
		{
			fprintf(colog_handle,"<div align=center><a href='%s%u.html'>Last</a></div>\n",
				colog_output,colog_lastpage);
		}
	}
	fprintf(colog_handle,"<th class=mth>Information\n");
	for (	mptr=Manager.FirstModule;
		mptr !=(struct colog_module *) 0;
		mptr = mptr->next )
	{
		if ( mptr->column )
		{
			fprintf(colog_handle,"<th class=ath>%u",mptr->column);
			if ( mptr->name )
				fprintf(colog_handle,"<br><font size=1>%s</font>",mptr->name);
			fprintf(colog_handle,"</th>\n");
		}
		else if ( mptr->name )
			fprintf(colog_handle,"<th class=ath>%s</th>\n",mptr->name);
		else	fprintf(colog_handle,"<th class=ath>%s:%u</th>\n",( mptr->host ? mptr->host : "unknown" ),mptr->port);
	}
	fprintf(colog_handle,"</tr>\n");
	return;
}

private void    colog_start_page( int mode )
{
	char	buffer[1024];
	colog_page++;
	sprintf(buffer,"%s%u.html",colog_output,colog_page);
	colog_handle = fopen( buffer, "w" );
        fprintf(colog_handle,"<html><head><title>Accords Platform LOG Analysis Report</title>\n");
       	fprintf(colog_handle,"<link href=\"analyse.css\" rel=\"STYLESHEET\" type=\"text/css\" media=\"SCREEN\">\n");
      	fprintf(colog_handle,"<link href=\"analyse.css\" rel=\"STYLESHEET\" type=\"text/css\" media=\"PRINT\">\n");
       	fprintf(colog_handle,"</style></head>\n");
       	fprintf(colog_handle,"<body><div align=center><h1>Accords Platform LOG Analysis Report</h1><p>\n");
        fprintf(colog_handle,"<p><table border=0>\n");
        colog_show_header(1,0);
        return;
}

private void    colog_flush_page( int mode )
{
        colog_show_header(0,mode);
       	fprintf(colog_handle,"</table>");
        colog_show_modules();
	fprintf(colog_handle,"</div><p></body></html>\n");
	fclose( colog_handle );
	colog_handle = (FILE *) 0;
	if ( mode ) { colog_start_page( mode ); }
        return;
}

/*	---------------------------------	*/
/*	c o l o g _ s h o w _ d e t a i l	*/
/*	---------------------------------	*/
private	void	colog_show_detail(int mode)
{
	int	n;
	int	from=0;
	int	to=0;
	char *	ifrom;
	char *	ito;
	int	items=0;
	struct	colog_event * eptr;
	struct	colog_module * fptr;
	struct	colog_module * tptr;
	struct	colog_module * left;
	struct	colog_module * right;
	char *	ifr;
	char *	ifl;
	char *	itr;
	char *	itl;
	char *	ith;
	int	total=0;

	for (	eptr=Manager.FirstEvent;
		eptr !=(struct colog_event *) 0;
		eptr = eptr->next )
	{
		if (!( fptr = eptr->from ))
			continue;
		else if (!( tptr = eptr->to ))
			continue;

		/* -------------------------------- */
		/* handle pagination with title bar */
		/* -------------------------------- */
		items++;
		if ( items > colog_page_size)
		{
			colog_flush_page(1);
			items=1;
		}

		switch ( eptr->dir )
		{
		case	1	:
			fprintf(colog_handle,"<tr><th class=ath>%u<th class=ath>%s<td class=msg>%s",++total,
				(eptr->method ? eptr->method : ""),
				(eptr->object ? eptr->object : ""));
			ifr = "reqfr.png";
			itl = "reqtl.png";
			ifl = "reqfl.png";
			itr = "reqtr.png";
			ith = "reqth.png";
			break;
		case	2	:
			fprintf(colog_handle,"<tr><th class=ath>%u<th class=ath>%u<th class=msg>%s",++total,eptr->status,(eptr->message ? eptr->message : "&nbsp;"));
			ifr = "repfr.png";
			itl = "reptl.png";
			ifl = "repfl.png";
			itr = "reptr.png";
			ith = "repth.png";
			break;
		default		:
			continue;
		}

		/* -------------------------------------- */
		/* reorganise as from smallest to largest */
		/* column id adjusting representation     */
		/* -------------------------------------- */
		if ( fptr->column > tptr->column )
		{
			right = fptr;
			left = tptr;
			from = tptr->column;
			to = fptr->column;
			ito = ifr;
			ifrom = itl;
		}
		else
		{
			right = tptr;
			left = fptr;
			from = fptr->column;
			to = tptr->column;
			ifrom = ifl;
			ito = itr;
		}

		/* ----------------------- */
		/* left hand empty columns */
		/* ----------------------- */
		if ( from > 1 )
		{
			n = from-1;
			while ( n )
			{
				fprintf(colog_handle,"<td class=nb><img src='vertical.png'></td>\n");
				n--;
			}
		}

		/* -------------------------- */
		/* left hand image to or from */
		/* -------------------------- */
		fprintf(colog_handle,"<td class=nb><img src='%s' alt='%s'></td>\n",ifrom,
			( left->name ? left->name : "unknown" ));
		
		/* -------------- */
		/* central images */
		/* -------------- */
		n = (to - from);
		if ( n > 1 )
		{
			n -= 1;
			while ( n )
			{
				fprintf(colog_handle,"<td class=nb><img src='%s'></td>\n",ith);
				n--;
			}
		}

		/* --------------------------- */
		/* right hand image to or from */
		/* --------------------------- */
		fprintf(colog_handle,"<td class=nb><img src='%s' alt='%s'></td>\n",ito,
			( right->name ? right->name : "unknown" ));

		/* ------------------------ */
		/* right hand empty columns */
		/* ------------------------ */
		n = maxcolumns - to;
		while ( n)
		{
			fprintf(colog_handle,"<td class=nb><img src='vertical.png'></td>\n");
			n--;
		}
	}
	return;
}

/*	---------------------------------	*/
/*	c o l o g _ s h o w _ r e s u l t 	*/
/*	---------------------------------	*/
private	void	colog_show_result(int pages)
{
	colog_page_size = pages;
	colog_start_page(pages);
	colog_show_detail(pages);
	colog_flush_page(0);
}

/*	---------------------------------	*/
/*	  c o l o g _ a n a l y s i s 		*/
/*	---------------------------------	*/
public	int	colog_analysis( char * filename, char * output, int pages )
{
	if (!( colog_output = output ))
		colog_output = "colog";
	colog_operation( filename );
	colog_show_result(pages);
	return( 0 );
}
	
	/* -------- */
#endif 	/* _colog_c */
	/* -------- */

