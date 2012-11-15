#ifndef	_paas_application_h
#define	_paas_application_h

struct paas_application 
{
	char* id;
	char* name;
	char* date_created;
	char* description;		
	char* paas_application_version;
	char* paas_application_environment;
	char* paas_application_configuration_template;
	int state;
};

#endif	

