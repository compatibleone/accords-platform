
#ifndef	_cords_manifest_h
#define	_cords_manifest_h

struct	cords_manifest
{
	char *				id;
	char *				name;
	char *				plan;
	char *				nodes;
	struct	cords_node	 	* firstapp;
	struct	cords_node	 	* lastapp;
	char *				configuration;
	char *				account;
	char *				security;
	char *				zone;
	char *				description;
	int				state;
};

#endif

