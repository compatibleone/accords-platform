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
		return( eptr );
	}
}

/*	---------------------------------	*/
/*	   r e s o l v e _ b y _ p i d 		*/
/*	---------------------------------	*/
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

/*	---------------------------------	*/
/*	r e s o l v e _ b y _ o b j e c t	*/
/*	---------------------------------	*/
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

/*	---------------------------------	*/
/*	   r e s o l v e _ b y _ u r l 		*/
/*	---------------------------------	*/
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

/*	---------------------------------------		*/
/*	c o l o g _ r e c e i v e d _ e v e n t 	*/
/*	---------------------------------------		*/
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

/*	---------------------------------	*/
/*	   c o l o g _ u s e _ l i n e		*/
/*	---------------------------------	*/
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
			( (fptr = eptr->from) ? ( fptr->name ? fptr->name : "unknown") : "unknown" ),
			( eptr->dir ? ( eptr->dir == 1 ? "to" : "from" ) : " "),
			( (tptr = eptr->to) ? ( tptr->name ? tptr->name : 
			( tptr->host ? tptr->host : "unknown")) : "unknown" ));
	}
	printf("</table><p>\n");
	return;
}

/*	---------------------------------	*/
/*	c o l o g _ s h o w _ m o d u l e	*/
/*	---------------------------------	*/
private	void	colog_show_modules()
{
	struct	colog_module * mptr;
	printf("<p><table width='90%'><tr><th colspan=4>COLOG Module List</th></tr>\n");
	printf("<tr><th>Number<th>Name<th>Process<th>URL</tr>\n");
	maxcolumns=0;
	for (	mptr=Manager.FirstModule;
		mptr !=(struct colog_module *) 0;
		mptr = mptr->next )
	{
		mptr->column = ++maxcolumns;

		printf("<tr><th>%u<td>%s", mptr->column, ( mptr->name ? mptr->name : "&nbsp;" ));

		if ( mptr->pid )
			printf("<td>%u",mptr->pid);
		else	printf("<td>&nbsp;");

		printf("<td>%s", ( mptr->host ? mptr->host : "&nbsp;" ));

		if ( mptr->port )
			printf(":%u",mptr->port);

		printf("</tr>\n");
	}
	printf("</table><p>\n");
	return;
}

/*	---------------------------------	*/
/*	c o l o g _ s h o w _ h e a d e r	*/
/*	---------------------------------	*/
private	void	colog_show_header()
{
	struct	colog_module * mptr;
	printf("<tr>\n");
	for (	mptr=Manager.FirstModule;
		mptr !=(struct colog_module *) 0;
		mptr = mptr->next )
	{
		if ( mptr->column )
		{
			printf("<th>%u",mptr->column);
			if ( mptr->name )
				printf("<br><font size=1>%s</font>",mptr->name);
			printf("</th>\n");
		}
		else if ( mptr->name )
			printf("<th>%s</th>\n",mptr->name);
		else	printf("<th>%s:%u</th>\n",( mptr->host ? mptr->host : "unknown" ),mptr->port);
	}
	printf("</tr>\n");
	return;
}

/*	---------------------------------	*/
/*	c o l o g _ s h o w _ d e t a i l	*/
/*	---------------------------------	*/
private	void	colog_show_detail()
{
	int	n;
	int	from=0;
	int	to=0;
	int	items=0;
	char *	ifrom;
	char *	ito;
	struct	colog_event * eptr;
	struct	colog_module * fptr;
	struct	colog_module * tptr;
	struct	colog_module * left;
	struct	colog_module * right;

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

		/* -------------------------------- */
		/* handle pagination with title bar */
		/* -------------------------------- */
		items++;
		if ( items > 48 )
		{
			colog_show_header();
			items=1;
		}

		printf("<tr>");

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
			ito = "fromright.png";
			ifrom = "toleft.png";
		}
		else
		{
			right = tptr;
			left = fptr;
			from = fptr->column;
			to = tptr->column;
			ifrom = "fromleft.png";
			ito = "toright.png";
		}

		/* ----------------------- */
		/* left hand empty columns */
		/* ----------------------- */
		if ( from > 1 )
		{
			n = from-1;
			while ( n )
			{
				printf("<td class=nb><img src='vertical.png'></td>\n");
				n--;
			}
		}

		/* -------------------------- */
		/* left hand image to or from */
		/* -------------------------- */
		printf("<td class=nb><img src='%s' alt='%s'></td>\n",ifrom,
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
				printf("<td class=nb><img src='through.png'></td>\n");
				n--;
			}
		}

		/* --------------------------- */
		/* right hand image to or from */
		/* --------------------------- */
		printf("<td class=nb><img src='%s' alt='%s'></td>\n",ito,
			( right->name ? right->name : "unknown" ));

		/* ------------------------ */
		/* right hand empty columns */
		/* ------------------------ */
		n = maxcolumns - to;
		while ( n)
		{
			printf("<td class=nb><img src='vertical.png'></td>\n");
			n--;
		}
	}
	return;
}

/*	---------------------------------	*/
/*	c o l o g _ s h o w _ r e s u l t 	*/
/*	---------------------------------	*/
private	void	colog_show_result()
{
	printf("<html><head><title>colog module list</title>\n");
	printf("<style>\n");
	printf("table { border-width: 0px; padding: 0px; margin: 0px; }\n");
	printf("td  { width: 80px; border-width: 0px; padding: 0px; margin: 0px; }\n");
	printf("th  { width: 80px; border-width: 0px; background-color: grey; padding: 0px; margin: 0px; }\n");
	printf("tr  { border-width: 0px; padding: 0px; margin: 0px; }\n");
	printf(".nb { border-style: none; }\n");
	printf(".b  { background-color: grey; border-style: solid; }\n");
	printf("</style></head>\n");
	printf("<body><div align=center>\n");
	colog_show_modules();
	printf("<p><table border=0>\n");
	colog_show_header();
	colog_show_detail();
	colog_show_header();
	printf("</table></div><p></body></html>\n");
}

/*	---------------------------------	*/
/*	  c o l o g _ a n a l y s i s 		*/
/*	---------------------------------	*/
public	int	colog_analysis( char * filename )
{
	colog_operation( filename );
	colog_show_result();
	return( 0 );
}
	
	/* -------- */
#endif 	/* _colog_c */
	/* -------- */

