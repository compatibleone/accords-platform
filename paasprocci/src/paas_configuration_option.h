#ifndef	_paas_configuration_option_h
#define	_paas_configuration_option_h

struct paas_configuration_option
{
	char* id;
	char* change_severity;
	char* max_length;
	char* max_value;
	char* min_value;
	char* regex;
	char* user_defined;
	char* value_options;
	char* namespace; 
	char* option_name;
	char* value;
	int status;
};

#endif	

