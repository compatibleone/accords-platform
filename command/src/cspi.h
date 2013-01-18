#ifndef	_cspi_h
#define	_cspi_h

#ifndef	public
#define	public
#endif

#ifndef	private
#define	private static
#endif

#define	_CORDSCRIPT_AGENT "cordscript/1.0a"

#define	_INTEGER_VALUE		0
#define	_STRING_VALUE		1
#define	_ARRAY_VALUE		2
#define	_STRUCTURE_VALUE	3
#define	_FLOAT_VALUE		4

#define	_CSP_EQ	1
#define	_CSP_LS	2
#define	_CSP_LE	3
#define	_CSP_GR	4
#define	_CSP_GE	5
#define	_CSP_NE	6

#define	_MAX_NAME	2048
#define	_MAX_VALUE	16000

#define	_IF_LABEL	1
#define	_SWITCH_LABEL	2
#define	_FOR_LABEL	3
#define	_FOREACH_LABEL	4
#define	_FORBOTH_LABEL	5
#define	_WHILE_LABEL	6
#define	_TRY_LABEL	7
#define	_CATCH_LABEL	8

struct	cordscript_context;
struct	cordscript_instruction;

struct	cordscript_label
{
	struct	cordscript_label 	* next;
	struct	cordscript_instruction 	* value;
	struct	cordscript_instruction 	* other;
	int	type;
};

struct	cordscript_value
{
	struct	cordscript_value * next;
	char *	name;
	int	parameter;
	struct	cordscript_instruction * code;
	struct	cordscript_context * body;
	struct	cordscript_context * object;
	char *	value;
};

struct	cordscript_exception
{
	struct	cordscript_exception *	next;
	struct	cordscript_instruction * handler;
};


struct	cordscript_operand
{
	struct	cordscript_operand 	* next;
	struct	cordscript_value	* value;
};

struct	cordscript_instruction
{
	struct	cordscript_context * context;
	struct	cordscript_instruction * next;
	struct	cordscript_operand * first;
	struct	cordscript_operand * last;
	struct	cordscript_instruction * (*evaluate)(struct cordscript_instruction * self);
};

struct	cordscript_context
{
	char *	name;
	int	instance;
	struct	cordscript_instruction * caller;
	struct	cordscript_value * stack;
	struct	cordscript_value * data;
	struct	cordscript_value * code;
	struct	cordscript_context     * env;
	struct	cordscript_context     * next;
	struct	cordscript_instruction * cs;
	struct	cordscript_instruction * ip;
};

	
private struct cordscript_value 	* allocate_cordscript_value(char * value, char * name);
public struct cordscript_value 		* duplicate_cordscript_value( struct cordscript_value * vptr );
public struct cordscript_instruction	* allocate_cordscript_instruction( struct cordscript_instruction * (*fptr)(struct cordscript_instruction * self) );
public struct cordscript_context	* allocate_cordscript_context();
private	struct	cordscript_instruction 	* compile_cordscript_instruction( struct cordscript_context * cptr, int level );

public struct cordscript_value 		* liberate_cordscript_value(struct cordscript_value * vptr);
public struct cordscript_instruction	* liberate_cordscript_instruction(struct cordscript_instruction * iptr); 
public struct cordscript_context	* liberate_cordscript_context( struct cordscript_context * xptr );

public struct cordscript_context	* compile_cordscript_string( char * expression );
public struct cordscript_context	* compile_cordscript_file( char * expression, int argc, char * argv[] );
public struct cordscript_value 		* execute_cordscript( struct cordscript_context * cptr );

#endif	/* _cspi_h */










