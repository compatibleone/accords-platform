#ifndef	_connections_h
#define	_connections_h

#include <sys/types.h>
#include <sys/socket.h>

struct	connection {
	void	*		parent;
	void	*		object;
	void	*		newobject;
	void	*		context;
	char	*		hostname;
	int			socket;
	struct	sockaddr	remote;
	int			newsocket;
	unsigned long		inbound;
	unsigned long		outbound;
	void			(*closure)(void *);
	};

#define	CONNECTIONPTR	struct connection * 

#endif	/* _connections_h */
	/* -------------- */


