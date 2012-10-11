#ifndef	_paas_configuration_option_h
#define	_paas_configuration_option_h

struct paas_configuration_option
{
	char* id;
	char* name;
	char* change_severity;
	char* default_value;
	char* max_length;
	char* max_value;
	char* min_value;
	char* namespace; 
	char* option_name;
	char* regex;
	char* user_defined;
	char* value;
	char* value_options;
	char* value_type;
	int state;
};

#endif	

