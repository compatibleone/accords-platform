
#ifndef	_cords_manifest_h
#define	_cords_manifest_h

struct	cords_manifest
{
	char *				id;
	char *				name;
	char *				plan;
	char *				cores;
	struct	cords_core	 	* firstapp;
	struct	cords_core	 	* lastapp;
	char *				configuration;
	char *				account;
	char *				security;
	char *				zone;
	int				state;
};

#endif

