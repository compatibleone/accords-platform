#ifndef _colog_c
#define _colog_c

#include <stdio.h>

#ifndef	public
#define	public
#endif
#ifndef	private
#define	private	static
#endif

int	maxcolumns=0;

struct	colog_request
{
	int	number;
	int	sent;
	int	from;
	int	received;
	int	by;
	char *	method;
	char *	object;
	char *	version;
};

struct	colog_reponse
{
	int	number;
	int	sent;
	int	from;
	int	received;
	int	by;
	char *	version;
	char *	status;
	char *	message;
};

struct	colog_module
{
	struct	colog_module * previous;
	struct	colog_module * next;
	struct	colog_request * first;
	struct 	colog_request * last;
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
	int	when;
	int	dir;
	int	response;
};

struct	colog_analsis
{
	struct colog_module * FirstModule;
	struct colog_module * LastModule;
	struct colog_event * FirstEvent;
	struct colog_event * LastEvent;
} 
Manager = 
{
	(struct colog_module *) 0,
	(struct colog_module *) 0
};

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

private	struct colog_event * allocate_event()
{
	struct	colog_event * eptr;
	if (!( eptr = (struct colog_event *) allocate( sizeof( struct colog_event ) )))
		return( eptr );
	else
	{
		memset(eptr,0,sizeof( struct colog_event ));
		return( eptr );
	}
}

private	struct	colog_module * resolve_by_pid( int pid )
{
	struct	colog_module * mptr;
	for (	mptr=Manager.FirstModule;
			mptr != (struct colog_module *) 0;
			mptr = mptr->next )
		if ( mptr->pid == pid )
			return( mptr );
	if (!( mptr = allocate_module()))
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
private	struct	colog_module * resolve_by_object( char * object )
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

private	struct	colog_module * resolve_by_url( struct url * uptr )
{
	struct	colog_module * mptr;
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

private	void	colog_sent_event( struct colog_event * eptr, char * wptr )
{
	struct	colog_module * mptr;
	struct	url * uptr;
	char *	method;
	char *	object;
	int	what;
	while ( *wptr == ' ' ) wptr++;
	wptr = scanpast(method=wptr,' ');
	if (!( what = colog_http_method( method ) ))
		return;
	while ( *wptr == ' ' ) wptr++;
	wptr = scanpast(object=wptr,' ');

	/* detect sent response */
	if ( what > 5 ) 
	{
		eptr->response = 1;
		return;
	}

	if (!( uptr = analyse_url( object )))
		return;

	else if (!( mptr = resolve_by_url( uptr )))
	{
		uptr = liberate_url( uptr );
		return;
	}
	else if (!( mptr->object = allocate_string( uptr->object ) ))
	{
		uptr = liberate_url( uptr );
		return;
	}
	else
	{
		mptr->event = eptr;
		eptr->to = mptr;
		uptr = liberate_url( uptr );
		return;
	}
}

private	void	colog_received_event( struct colog_event * eptr, char * wptr )
{
	struct	colog_module * mptr;
	struct	colog_module * self;
	struct	url * uptr;
	char *	method;
	char *	object;
	int	what;
	if (!( self = eptr->from ))
		return;
	while ( *wptr == ' ' ) wptr++;
	wptr = scanpast(method=wptr,' ');
	if (!( what = colog_http_method( method ) ))
		return;
	while ( *wptr == ' ' ) wptr++;
	wptr = scanpast(object=wptr,' ');

	/* detect received response */
	if ( what > 5 ) 
	{
		eptr->response = 1;
		return;
	}

	if (!( self->host ))
	{
		if (!( mptr = resolve_by_object( object ) ))
			return;
		else if (!( mptr->host ))
			return;
		else if (!( self->host = allocate_string( mptr->host ) ))
			return;
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
	return;
}



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

private	int	colog_use_line( char * buffer )
{
	char *	wptr=buffer;
	char *	vptr;
	int	when;
	int	what;
	char * who;
	int	dir;
	struct	colog_module * mptr;
	wptr = scanpast(vptr=wptr,':');
	when = atoi(vptr);
	wptr = scanpast(vptr=wptr,':');
	what = atoi(vptr);
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
	if (!( mptr = resolve_by_pid( what ) ))
		return( 27 );
	else if ( mptr->name )
		return( colog_use_event( mptr, wptr, when, dir ));
	else if (!( mptr->name = allocate_string( who ) ))
		return( 27 );
	else
		return( colog_use_event( mptr, wptr, when, dir ));
}

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

private	void	colog_show_events()
{
	struct	colog_event * eptr;
	struct	colog_module * fptr;
	struct	colog_module * tptr;
	printf("<p><table><tr><th colspan=3>COLOG Event List</th></tr>\n");
	printf("<tr><th>Time<th>From<th>To</tr>\n");
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
		printf("<tr><th>%u<td>%s %s<td> %s %s</tr>\n",
			eptr->when,
			( eptr->dir ? ( eptr->dir == 1 ? "Sent by" : "Received by" ) : " "),
			( (fptr = eptr->from) ? ( fptr->name ? fptr->name : "[unknown]") : "[unknown]" ),
			( eptr->dir ? ( eptr->dir == 1 ? "to" : "from" ) : " "),
			( (tptr = eptr->to) ? ( tptr->name ? tptr->name : 
			( tptr->host ? tptr->host : "[unknown]")) : "[unknown]" ));
	}
	printf("</table><p>\n");
	return;
}


private	void	colog_show_modules()
{
	struct	colog_module * mptr;
	printf("<p><table><tr><th colspan=3>COLOG Module List</th></tr>\n");
	printf("<tr><th>Name<th>Process<th>URL</tr>\n");
	for (	mptr=Manager.FirstModule;
		mptr !=(struct colog_module *) 0;
		mptr = mptr->next )
	{
		printf("<tr><td>%s<td>%u<td>%s:%u</tr>\n",
			( mptr->name ? mptr->name : "[unknown]" ),
			mptr->pid,
			( mptr->host ? mptr->host : "[unknown]" ),mptr->port);
	}
	printf("</table></div><p></body></html>\n");
	return;
}
private	void	colog_show_header()
{
	struct	colog_module * mptr;
	maxcolumns=0;
	printf("<tr>\n");
	for (	mptr=Manager.FirstModule;
		mptr !=(struct colog_module *) 0;
		mptr = mptr->next )
	{
		if ( mptr->name )
			printf("<th>%s</th>\n",mptr->name);
		else	printf("<th>%s:%u</th>\n",( mptr->host ? mptr->host : "unknown" ),mptr->port);
		mptr->column = ++maxcolumns;
	}
	printf("</tr>\n");
	return;
}

private	void	colog_show_detail()
{
	int	n;
	int	from=0;
	int	to=0;
	int	items=0;
	struct	colog_event * eptr;
	struct	colog_module * fptr;
	struct	colog_module * tptr;

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
		if (!( fptr = eptr->from ))
			continue;
		else if (!( tptr = eptr->to ))
			continue;

		items++;
		if ( items > 60 )
		{
			colog_show_header();
			items=1;
		}

		printf("<tr>");
		if ( fptr->column > tptr->column )
		{
			from = tptr->column;
			to = fptr->column;
		}
		else
		{
			from = fptr->column;
			to = tptr->column;
		}
		if ( from > 1 )
		{
			n = from-1;
			while ( n )
			{
				printf("<td class=nb><img src='vertical.png'></td>\n");
				n--;
			}
		}
		printf("<td class=nb><img src='fromleft.png'></td>\n");
		
		n = (to - from);
		if ( n > 1 )
		{
			n -= 1;
			while ( n )
			{
				printf("<td class=nb><img src='through.png'></td>\n");
				n--;
			}
		}
		printf("<td class=nb><img src='toright.png'></td>\n");
		n = maxcolumns - to;
		while ( n)
		{
			printf("<td class=nb><img src='vertical.png'></td>\n");
			n--;
		}
	}
	return;
}

private	void	colog_show_results()
{
	printf("<html><head><title>colog module list</title>\n");
	printf("<style>table { padding: 0px; } .nb { border-style: none; } .b { background-color: grey; border-style: solid; } </style></head>\n");
	printf("<body><div align=center>\n");
	printf("<p><table border=1>\n");
	colog_show_header();
	colog_show_detail();
	printf("</table></div><p></body></html>\n");
}

public	int	colog_analysis( char * filename )
{
	colog_operation( filename );
	colog_show_results();
	return( 0 );
}

	
#endif 	/* _colog_c */
	/* --------- */

