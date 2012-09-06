#ifndef	_paas_version_instance_h
#define	_paas_version_instance_h

struct paas_version_instance
{
	char* id;
	char* name;
	char* date_instantiated;
	char* description;
	char* state;
	char* default_instance;
	int status;
};

#endif	

