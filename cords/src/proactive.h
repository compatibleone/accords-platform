
#ifndef	_proactive_h
#define	_proactive_h

/*!
 * This is the structure that is exchanged whenever a ProActive node is requested. 
 */
struct	proactive
{
	char *	id;

	char *	name;
	char *	flavor;
	char *	image;
	char *	profile;

	char *	number; // uuid of the request? 
	char *	access;
	char *	rootpass;
	char *	reference;
	char *	publicaddr;
	char *	privateaddr;

	char *	hostname;

	int	when;

	int	status;
};

#endif	


