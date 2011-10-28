#ifndef	_global_h
#define	_global_h
#define SYSTEM_SSL 1
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
# include <sys/ipc.h>
# include <sys/sem.h>
#ifdef	SYSTEM_SSL
#include <openssl/ssl.h>
#endif
#include <errno.h>
#include <signal.h>
#include "allocate.h"

#define	public
#define	private static
#define	true 1
#define	false 0
#define	function int

#define	_SASL_DIGEST_MD5	2
#define	_SASL_ANONYMOUS		1
#define	_SASL_PLAIN		0
#define	_DEBUG_SOCKET		16

#include "connection.h"
#include "element.h"

#endif	/* _global_h */
	/* --------- */

