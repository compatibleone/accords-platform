#ifndef	_paas_node
#define	_paas_node

struct paas_node
{
	char* id;
	char* content_type;
	char* name;
	char* version;
	char* provider;
	int status;
};

#endif	

