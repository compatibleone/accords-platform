#ifndef	_paas_application_version_instance_h
#define	_paas_application_version_instance_h

struct paas_application_version_instance
{
	char* id;
	char* name;
	char* date_instantiated;
	char* description;
	char* default_instance;
	char* uri;
	int state;
};

#endif	


