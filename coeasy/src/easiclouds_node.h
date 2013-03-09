	#ifndef	_easiclouds_node_h
	#define	_easiclouds_node_h
	struct	easiclouds_node
	{
		char	*	id;
		char 	*	name;
		char 	*	description;
		char 	*	tenant;
		int		state;
		int		servers;
		int		extras;
	};
	#endif

