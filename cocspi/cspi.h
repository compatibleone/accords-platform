#ifndef	_cspi_h
#define	_cspi_h

#include "standard.h"
#include "allocate.h"
#include "rest.h"
#include "cp.h"
#include "cordslang.h"
#include "occiresolver.h"

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

#define	_DISPLAY_FUNCTION	0
#define	_WAIT_FUNCTION		1
#define	_LENGTH_FUNCTION	2
#define	_ROUND_FUNCTION		3
#define	_MEMBER_FUNCTION	4
#define	_DATE_FUNCTION		5
#define	_CUT_FUNCTION		6
#define	_JOIN_FUNCTION		7
#define	_DEBUG_FUNCTION		8
#define	_NEW_FUNCTION		9
#define	_OPEN_FUNCTION		10
#define	_EVAL_FUNCTION		11
#define	_MAX_FUNCTIONS		12

#define	_MAX_KEYWORDS	20
#define	_KEYWORD_HASH	57

struct	cordscript_context;
struct	cordscript_instruction;

struct	cordscript_language_function
{
	char *	keyword;
	int	value;
	int	hashcode;
}; 

struct	cordscript_language	
{
	char	*	keyword;
	struct cordscript_instruction * (*compile)(struct cordscript_context * cptr);
	int		hashcode;
};

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
	int	nature;
	struct	cordscript_instruction * caller;
	struct	cordscript_value * stack;
	struct	cordscript_value * data;
	struct	cordscript_value * code;
	struct	cordscript_value * returnvalue;
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

public struct cordscript_context	* compile_cordscript_string( char * expression, int argc, char * argv[]  );
public struct cordscript_context	* compile_cordscript_file( char * expression, int argc, char * argv[] );
public struct cordscript_value 		* execute_cordscript( struct cordscript_context * cptr );
public char *				  evaluate_cordscript( char * expression, int argc, char * argv[] );

#endif	/* _cspi_h */










