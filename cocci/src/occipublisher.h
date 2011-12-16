#ifndef	_occipublisher_h
#define	_occipublisher_h


#include "occiclient.h"

#define	_DEFAULT_TLS		default_tls()
#define	__DEFAULT_TLS		"security/tls.xml"
#define	_DEFAULT_HOST		"https://publisher.compatibleone.fr:28001"
#define	_DEFAULT_ROOM		"marketplace"
#define	_DEFAULT_PUBLICATION	"publication"
#define	_DEFAULT_ENQUIRY	"enquiry"

struct	occi_publisher;

#include "occipub.h"

struct	occi_publisher
{
	char *	tls;
	char *	host;
	char *	room;
	char *	publication;
	char *	enquiry;
	char *	uri;
	char *	authorization;
	struct	occi_publication * first;
	struct	occi_publication * last;
};

	/* ---------------- */
#endif	/* _occipublisher_h */
	/* ---------------- */









