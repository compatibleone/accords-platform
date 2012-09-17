#ifndef	_paas_deployable_h
#define	_paas_deployable_h

struct paas_deployable
{
	char* id;
	char* name;
	char* content_type;
	char* location;
	char* multitenancy_level;
	int state;
};

#endif	


