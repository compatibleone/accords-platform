#ifndef	_paas_node
#define	_paas_node

struct paas_node
{
	char* id;
	char* name;
	char* content_type;
	char* version;
	char* provider;
	int state;
};

#endif	

