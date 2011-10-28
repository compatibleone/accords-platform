#ifndef	_connection_h
#define	_connection_h

#include <sys/types.h>
#include <sys/socket.h>

struct	connection {
	void	*		parent;
	void	*		object;
	void	*		newobject;
	void	*		context;
	int			socket;
	struct	sockaddr	remote;
	int			newsocket;
	unsigned long		inbound;
	unsigned long		outbound;
	void			(*closure)(void *);
	};

#define	CONNECTIONPTR	struct connection * 

#endif	/* _connection_h */
	/* ------------- */


