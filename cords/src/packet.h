#ifndef	_packet_h
#define	_packet_h

struct	cords_packet
{
	char *	id;
	char *	name;
	char *	connection;
	char *	probe;
	char *	start;
	char *	finish;
	char *	metric;
	char *	data;
	int	sequence;
	int	samples;
	int	state;
};

#endif

