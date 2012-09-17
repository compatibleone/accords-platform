#ifndef	_paas_application_h
#define	_paas_application_h

struct paas_application 
{
	char* id;
	char* name;
	char* date_created;
	char* description;
	char* environment;
	int state;
};

#endif	

