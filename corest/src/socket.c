#ifndef	_socket_c
#define	_socket_c

#include "global.h"

extern	int	check_debug();

private	pthread_mutex_t socket_control = PTHREAD_MUTEX_INITIALIZER;

private	_socket_type=AF_INET;

public	void	set_socket_ipv6()
{
	_socket_type=AF_INET6;
	return;
}
	
public	void	set_socket_ipv4()
{
	_socket_type=AF_INET;
	return;
}
	
public	int	get_socket_type()
{
	return( _socket_type );
}

/*	------------------------------------------	*/
/*		s o c k e t _ s e l e c t		*/
/*	------------------------------------------	*/
public	int	socket_select( int h, int s, int f )
{
	int	status;
	if ( check_debug() & _DEBUG_SOCKET ) printf( "socket_select(%u,%u,%u)\n",h,s,f);
	struct	timeval TimeOut;
	fd_set	r;
	fd_set 	w;
	fd_set	x;
	FD_ZERO(&r);
	FD_ZERO(&w);
	FD_ZERO(&x);
	FD_SET(h,&r);
	TimeOut.tv_sec = (unsigned long) s;
	TimeOut.tv_usec = (unsigned long) f;
	if ((status = select( h+1, &r, &w, &x ,&TimeOut )) > 0 )
		return(1);
	else	return(status);
}

/*	------------------------------------------	*/
/*		s o c k e t _ w r i t e  		*/
/*	------------------------------------------	*/
public	int	socket_write( int h, char * bptr )
{
	int	blen  = strlen( bptr );
	if ( check_debug() & _DEBUG_SOCKET  ) printf( "socket_write(%u,%s,%u)\n",h,bptr,blen);
	return( send( h, bptr, blen , 0 ) );
}

/*	------------------------------------------	*/
/*		s o c k e t _ r e c v			*/
/*	------------------------------------------	*/
public	int	socket_recv( int h, char * bptr, int blen )
{
	if ( check_debug() & _DEBUG_SOCKET  ) printf( "socket_recv(%u,%u)\n",h,blen);
	return( recv( h, bptr, blen , 0 ) );
}

/*	------------------------------------------	*/
/*		s o c k e t _ s h u t d o w n		*/
/*	------------------------------------------	*/
public	int	socket_shutdown( int h, int p)
{
	if ( check_debug() & _DEBUG_SOCKET   )	{ printf( "socket_shutdown(%u,%u)\n",h,p);	}
	if ( h != 0) 	{ shutdown( h, p ); 				}
	return( 0 );
}

/*	------------------------------------------	*/
/*		s o c k e t _ g e t c h			*/
/*	------------------------------------------	*/
public	int	socket_getch(struct connection *  h)
{
	int	status;
	char	result;
	if ((status = socket_select( h->socket, 0, 0 )) == -1)
		return( status );
	else if ( connection_read( h, &result, 1) != 1 )
		return(-1);
	else	return((result & 0x00FF));
}

void	socket_lock(int h, char * f)
{
	char	buffer[1024];
	sprintf(buffer,"%s_socket_lock(%u)",f,h);
	rest_log_debug( buffer );
	pthread_mutex_lock( &socket_control );
	return;
}

void	socket_unlock(int h, char * f)
{
	char	buffer[1024];
	sprintf(buffer,"%s_socket_unlock(%u)",f,h);
	rest_log_debug( buffer );
	pthread_mutex_unlock( &socket_control );
	return;
}

/*	------------------------------------------	*/
/*		s o c k e t _ c l o s e			*/
/*	------------------------------------------	*/
public	int	socket_close( int h )
{
	socket_lock(h,"close");
	if ( check_debug() & _DEBUG_SOCKET  ) 	{printf( "socket_close(%u)\n",h);	}
	if ( h != 0) 	{ close( h ); 				}
	socket_unlock(-1,"close");
	return(0);
}

/*	------------------------------------------	*/
/*		s o c k e t _ c r e a t e		*/
/*	------------------------------------------	*/
public	int	socket_create( int a, int b, int c )
{
	int	h;
	int	optval=60;

	socket_lock(-1,"create");

	if ( check_debug() & _DEBUG_SOCKET  ) 
		printf( "socket_create(%u,%u,%u)\n",a,b,c);

	if ((h = socket(a,b,c)) < 0)
		h = -1;
	else
	{
		optval = 60;
		(void) setsockopt(h, SOL_SOCKET, (SO_KEEPALIVE | SO_REUSEADDR), (int *) & optval, sizeof( optval ));
		optval = 1;
		(void) setsockopt(h, SOL_TCP, TCP_NODELAY, (int *) & optval, sizeof( optval ));
	}
	socket_unlock(h,"create");
	return(h);
}

/*	------------------------------------------	*/
/*		s o c k e t _ c o n n e c t		*/
/*	------------------------------------------	*/
public	int	socket_connect( int h, char * u,int port )
{
	unsigned  char tempxfer[4];
	char *	wptr;
	struct  hostent *hp=(struct hostent *) 0;
	struct sockaddr_in address;
	struct sockaddr_in server;

	if ( check_debug() & _DEBUG_SOCKET ) 
	{
		printf( "socket_connect(%u,%s,%u)\n",h,u,port);
	}
	address.sin_family = get_socket_type();
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(0);
	if ( bind(h, & address, sizeof( struct sockaddr_in)) < 0 ) 
	{
		socket_close( h ) ;
		return(0);
	}
     	if (!(hp = gethostbyname(u)))
		return( 0 );
	else 	
	{
		server.sin_family = get_socket_type();
		memcpy(tempxfer, hp->h_addr_list[0],4);
		memcpy(&server.sin_addr.s_addr,tempxfer,4);
		server.sin_port = htons(port);

		if ( connect( h, 
			(struct sockaddr *) & server,
			sizeof( struct sockaddr ) ) < 0 )
			return(0);
		else	return(1);
	}
}

/*	--------------------------------------------------	*/
/*		s o c k e t _ t r y _ c o n n e c t		*/
/*	--------------------------------------------------	*/
private	int	socket_alarm=0;
private	void	socket_try_catcher( int s ) { socket_alarm=1; }
public	int	socket_try_connect( int h, char * u,int port, int timeout )
{
	int	status=0;
	void *	vptr;
	unsigned  char tempxfer[4];
	char *	wptr;
	struct  hostent *hp=(struct hostent *) 0;
	struct sockaddr_in address;
	struct sockaddr_in server;

	if ( check_debug() & _DEBUG_SOCKET ) 
	{
		printf( "socket_connect(%u,%s,%u)\n",h,u,port);
	}
	address.sin_family = get_socket_type();
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(0);
	if ( bind(h, & address, sizeof( struct sockaddr_in)) < 0 ) 
	{
		socket_close( h ) ;
		return(0);
	}
     	if (!(hp = gethostbyname(u)))
		return( 0 );
	else 	
	{
		server.sin_family = get_socket_type();
		memcpy(tempxfer, hp->h_addr_list[0],4);
		memcpy(&server.sin_addr.s_addr,tempxfer,4);
		server.sin_port = htons(port);
		socket_alarm=0;
		vptr = signal(SIGALRM,socket_try_catcher);
		alarm( timeout );
		status = connect( h, 
			(struct sockaddr *) & server,
			sizeof( struct sockaddr ) );
		alarm(0);
		vptr = signal(SIGALRM,vptr);
		if ( socket_alarm )
			return(0);
		else if ( status < 0 )
			return(0);
		else	return(1);
	}
}

/*	------------------------------------------	*/
/*		s o c k e t _ l i s t e n		*/
/*	------------------------------------------	*/
public	int	socket_listen( int h, int port, int max )
{
	unsigned  char tempxfer[4];
	char *	wptr;
	struct  hostent *hp=(struct hostent *) 0;
	struct sockaddr_in address;
	struct sockaddr_in server;

	if ( check_debug() & _DEBUG_SOCKET ) 
		printf( "socket_listen(%u,%u,%u)\n",h,port,max);

	address.sin_family = get_socket_type();
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);
	if ( bind(h, & address, sizeof( struct sockaddr_in)) < 0 ) {
		socket_close( h ) ;
		return(0);
		}
	else if ( listen( h, max ) < 0 ) {
		socket_close( h );
		return(-1);
		}
	else	return(1);

}

/*	------------------------------------------	*/
/*		f r e e _ s o c k e t 			*/
/*	------------------------------------------	*/
int	free_socket( int s )
{
	if ( socket ) {
		socket_shutdown(s,2);
		socket_close(s);
		}
	return(0);
}

/*	------------------------------------------	*/
/*		s o c k e t _ g e t l i n e		*/
/*	------------------------------------------	*/
int	socket_getline( struct connection * cptr, char * b, int blen )
{
	unsigned char c[4];
	int		o=0;
	int		l = sizeof( struct connection );
	if ( check_debug() & _DEBUG_SOCKET ) 
	{
		printf( "socket_getline()\n");
	}
	while (o < blen) {
		if ( connection_read( cptr, c, 1 ) <= 0 )
			break;
		else if ( c[0] == '\r' )
			continue;
		else if ( c[0] == '\n' )
			break;
		else if ( c[0] == '\t' )
			c[0] = ' ';
		else	{
			*(b+o) = c[0];
			o++;
			}
		}
	*(b+o) = 0;
	return(o);
}

/*	------------------------------------------	*/
/*		s o c k e t _ a c c e p t		*/
/*	------------------------------------------	*/
int	socket_accept(int h, void * vptr, int * vlen )
{
	int	result;
	socket_lock(h,"accept");
	result = accept(h, vptr, vlen );
	socket_unlock(result,"accept");
	return( result );
}

	/* --------- */
#endif	/* _socket_c */
	/* --------- */


