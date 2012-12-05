#ifndef	_cspi_h
#define	_cspi_h

#ifndef	public
#define	public
#endif

#ifndef	private
#define	private static
#endif

#define	_CORDSCRIPT_AGENT "cordscript/1.0a"

struct	cordscript_context;
struct	cordscript_instruction;

struct	cordscript_value
{
	struct	cordscript_value * next;
	char *	name;
	struct	cordscript_instruction * code;
	char *	value;
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
	struct	cordscript_value * (*evaluate)(struct cordscript_instruction * self);
};

struct	cordscript_context
{
	struct	cordscript_value * stack;
	struct	cordscript_value * data;
	struct	cordscript_value * code;
	struct	cordscript_instruction * cs;
	struct	cordscript_instruction * ip;
};

	
private struct cordscript_value 	* allocate_cordscript_value(char * value, char * name);
public struct cordscript_value 		* duplicate_cordscript_value( struct cordscript_value * vptr );
public struct cordscript_instruction	* allocate_cordscript_instruction( struct cordscript_value * (*fptr)(struct cordscript_instruction * self) );
public struct cordscript_context	* allocate_cordscript_context();

public struct cordscript_value 		* liberate_cordscript_value(struct cordscript_value * vptr);
public struct cordscript_instruction	* liberate_cordscript_instruction(struct cordscript_instruction * iptr); 
public struct cordscript_context	* liberate_cordscript_context( struct cordscript_context * xptr );

public struct cordscript_context	* compile_cordscript_string( char * expression );
public struct cordscript_context	* compile_cordscript_file( char * expression );
public struct cordscript_value 		* execute_cordscript( struct cordscript_context * cptr );

#endif	/* _cspi_h */










