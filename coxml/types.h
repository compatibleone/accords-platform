#ifndef _types_h
#define	_types_h

#define	private	static
#define	public	


struct	coord	{
	float	X;
	float	Y;
	};

struct	flag	{
	int	Value;
	};

struct	boolean	{
	int	Value;
	};

struct	value	{
	int	Value;
	};

struct	uv	{
	int	U;
	int	V;
	};

struct	values	{
	int	Value;
	};

struct	scale	{
	float	X;
	float	Y;
	};

struct	filename {
	char * Value;
	};

struct	strings {
	char *	Value;
	};

struct	timer {
	int	Hours;
	int	Minutes;
	int	Seconds;
	};

struct	type_flag	{
	char *	Type;
	int	Value;
	};

struct	vector	{
	float	X;
	float	Y;
	float	Z;
	};

struct	rotation {
	float	X;
	float	Y;
	float	Z;
	float	W;
	};

struct	colour	{
	int	R;
	int	G;
	int	B;
	};


	/* -------- */
#endif	/* _types_h */
	/* -------- */
















