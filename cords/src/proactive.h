
#ifndef	_proactive_h
#define	_proactive_h

/*!
 * This is the structure that is exchanged whenever a ProActive node is requested. 
 */
struct	proactive
{
	char *	id;         // Internal id? 

	char *	name;       // Name given to the ProActive node. 
	char *	flavor;
	char *	image;
	char *	profile;

	char *	number;     // uuid of the ProActive node request? 
	char *	access;
	char *	rootpass;   // administrator pass to access the ProActive node once it is locked. 
	char *	reference;  // host id. 
	char *	publicaddr;
	char *	privateaddr;

	char *	hostname;

	int	when;

	int	status;

};

#endif	


