/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons Paternit‚ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */
#ifndef	_cordscript_parser_c
#define	_cordscript_parser_c

#include "csp.h"

#include "cse.c"
#include "csa.c"

/*	-----------------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ s c r i p t _ e l e m e n t s 	*/
/*	-----------------------------------------------------------	*/
public struct cordscript_element * liberate_cordscript_elements(struct cordscript_element * sptr)
{
	struct	cordscript_element * eptr;
	if ( sptr )
	{
		while (( eptr = sptr ) != (struct cordscript_element *) 0)
		{
			sptr = sptr->next;
			eptr = liberate_cordscript_element( eptr );
		}
	}
	return((struct cordscript_element *) 0);
}

/*	-----------------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ s c r i p t _ e l e m e n t s 	*/
/*	-----------------------------------------------------------	*/
public struct cordscript_action * liberate_cordscript_actions(struct cordscript_action * sptr)
{
	struct	cordscript_action * eptr;
	if ( sptr )
	{
		while (( eptr = sptr ) != (struct cordscript_action *) 0)
		{
			sptr = sptr->next;
			eptr = liberate_cordscript_action( eptr );
		}
	}
	return((struct cordscript_action *) 0);
}

private	struct	cords_token_parser
{
	char *	value;
	int	state;
	int	ungotc;
	int	bytes;
	int	lines;
	int	columns;
	int	echo;
	char 	token[_MAX_CORDSCRIPT_TOKEN+1];
} Csp = { 
	(char *) 0, 
	0, 
	0, 
	0, 0, 0 ,
	0
	};

private	void	initialise_token_parser( char * vptr )
{
	Csp.value = vptr;
	Csp.ungotc = Csp.columns = Csp.bytes = Csp.lines = 0;
	Csp.echo = 0;
	return;
}

private	int	cordscript_getch()
{
	int	c;
	if ((c = Csp.ungotc ))
		Csp.ungotc = 0;
	else if (!( Csp.value ))
		return( 0 );
	else if (!( c = *(Csp.value++) ))
		Csp.value = (char *) 0;
	else if ( c == '\n' )
	{
		Csp.lines++;
		Csp.columns=0;
	}
	else if ( c != '\r' )
		Csp.columns++;
	Csp.bytes++;
	if ( Csp.echo )
		printf("%c",c);
	return( c );
}

private	void	cordscript_ungetch(int c)
{
	Csp.ungotc = c;
	return;
}

private	int	cordscript_is_white( int c )
{
	switch( c )
	{
	case	'\n'	:
	case	'\r'	:
	case	'\t'	:
	case	' '	:
		return(1);
	default		:
		return(0);
	}
}

private	int	cordscript_is_punctuation( int c )
{
	switch( c )
	{
	case	'.'	:
	case	'='	:
	case	'('	:
	case	')'	:
	case	','	:
	case	';'	:
		return(c);
	default		:
		return(0);
	}
}


private	int	cordscript_remove_white()
{
	int	c;
	while (1)
	{
		if ((c = cordscript_getch()) == 0)
			return(0);
		else if ( c == -1 )
			return( 0 );
		else if ( cordscript_is_white( c ) )
			continue;
		else
		{
			cordscript_ungetch(c);
			return( c );
		}
	}
}

private	int	cordscript_punctuation()
{
	int	c;
	if (!(c = cordscript_remove_white()))
		return( 0 );
	else if (!( cordscript_is_punctuation( c ) ))
		return( 0 );
	else	return( cordscript_getch() );
}

private	int	cordscript_token()
{
	int	quoting=0;
	int	c;
	int	i=0;
	if (!( cordscript_remove_white() ))
		return( 0 );
	else
	{
		while (i < _MAX_CORDSCRIPT_TOKEN)
		{
			if (!( c = cordscript_getch() ))
				return( c );
			else if ( quoting )
			{
				if ( c == quoting )
				{
					Csp.token[i++] = c;
					Csp.token[i] = 0;
					return( i );
				}
				else
				{
					Csp.token[i++] = c;
					continue;
				}
			}
			else if (( c == '"' ) 
			     || ( c == 0x0027 ))
			{
				if (!(i))
				{
					Csp.token[i++] = quoting = c;
					continue;
				}
				else
				{
					cordscript_ungetch(c);
					Csp.token[i] = 0;
					return( i );
				}
			}			
			else if ( cordscript_is_white( c ) )
			{
				cordscript_ungetch(c);
				Csp.token[i] = 0;
				return( i );
			}
			else if ( cordscript_is_punctuation( c ) )
			{
				cordscript_ungetch(c);
				Csp.token[i] = 0;
				return( i );
			}
			else
			{
				Csp.token[i++] = c;
				continue;
			}
		}
		/* exceeding token size */
		return(0);
	}
}

/*	---------------------------------------------------------	*/
/*	 	     c o r d s c r i p t _ t e r m 			*/
/*	---------------------------------------------------------	*/
public	struct	cordscript_element *	cordscript_term()
{
	struct	cordscript_element * eptr=(struct cordscript_element *) 0;
	if (!( cordscript_token() ))
		return( eptr );
	else if (!( eptr = allocate_cordscript_element() ))
		return( eptr );
	else if (( Csp.token[0] == '"' )
	     ||  ( Csp.token[0] == 0x0027 ))
	{
		eptr->value = allocate_string( Csp.token );
		eptr->type = _CORDSCRIPT_LITERAL;
	}
	else if (!( eptr->prefix = allocate_string( Csp.token ) ))
		return( liberate_cordscript_element( eptr ) );
	else
	{	
		eptr->type = _CORDSCRIPT_PROPERTY;
		if ( cordscript_punctuation() != '.' )
			return( liberate_cordscript_element( eptr ) );
		else if (!( cordscript_token() ))
			return( liberate_cordscript_element( eptr ) );
		else if (!( eptr->value = allocate_string( Csp.token ) ))
			return( liberate_cordscript_element( eptr ) );
	}
	return( eptr );
}

/*	---------------------------------------------------------	*/
/*	 c o r d s _ s c r i p t _ p a r s e _ s t a t e m e n t	*/
/*	---------------------------------------------------------	*/
public	struct	cordscript_action *	cordscript_parse_statement( char * statement )
{
	int	c;
	struct	cordscript_element * lvalue=(struct cordscript_element *) 0;
	struct	cordscript_element * rvalue=(struct cordscript_element *) 0;
	struct	cordscript_action * aptr=(struct cordscript_action *) 0;
	struct	cordscript_action * root=(struct cordscript_action *) 0;
	struct	cordscript_action * foot=(struct cordscript_action *) 0;
	initialise_token_parser( statement );
	while ( cordscript_token() )
	{
		/* allocate an action controller */
		/* ----------------------------- */
		if (!( aptr = allocate_cordscript_action()))
			return( liberate_cordscript_actions( root ) );
		else if (!( aptr->previous = foot ))
			root = aptr;
		else 	foot->next = aptr;
		foot = aptr;

		/* allocate the lvalue element */
		/* --------------------------- */
		if (!( aptr->lvalue = allocate_cordscript_element() ))
			return( liberate_cordscript_actions( root ) );
		else if ((c = cordscript_punctuation()) != '.')
			return( liberate_cordscript_actions( root ) );
		else if (!( aptr->lvalue->prefix = allocate_string( Csp.token ) ))
			return( liberate_cordscript_actions( root ) );
		else if (!( cordscript_token() ))
			return( liberate_cordscript_actions( root ) );
		else if (!( aptr->lvalue->value = allocate_string( Csp.token ) ))
			return( liberate_cordscript_actions( root ) );

		/* seperate between configuration method and affectation */
		/* ----------------------------------------------------- */
		if (!( strcmp( Csp.token, "configure" )))
		{
			aptr->lvalue->type = _CORDSCRIPT_METHOD;
			aptr->type = _CORDSCRIPT_CONFIGURE;
			if ( cordscript_punctuation() != '(' )
				return( liberate_cordscript_actions( root ) );
			while (1)
			{
				if (!( rvalue = cordscript_term() ))
					return( liberate_cordscript_actions( root ) );
				else if (!( rvalue->previous = lvalue ))
					aptr->rvalue = rvalue;
				else	lvalue->next = rvalue;
				lvalue = rvalue;
				if ((c = cordscript_punctuation()) == ',' )
					continue;
				else
				{
					if ( c ) cordscript_ungetch( c );
					break;
				}
			}
			if ( cordscript_punctuation() != ')' )
				return( liberate_cordscript_actions( root ) );
			if ( cordscript_punctuation() != ';' )
				return( liberate_cordscript_actions( root ) );
			else	continue;
		}
		else
		{
			aptr->lvalue->type = _CORDSCRIPT_PROPERTY;
			aptr->type = _CORDSCRIPT_AFFECT;
			if ( cordscript_punctuation() != '=' )
				return( liberate_cordscript_actions( root ) );

			else if (!( aptr->rvalue = cordscript_term() ))
				return( liberate_cordscript_actions( root ) );

			else if ( cordscript_punctuation() != ';' )
				return( liberate_cordscript_actions( root ) );
			else	continue;
		}
	}
	return( aptr );
}

private	void	cordscript_show_element( struct cordscript_element * eptr )
{
	if ( eptr->type == _CORDSCRIPT_LITERAL )
		printf("%s",eptr->value);
	else if ( eptr->type == _CORDSCRIPT_METHOD )
		printf("%s.%s",eptr->prefix,eptr->value);
	else if ( eptr->type == _CORDSCRIPT_PROPERTY )
		printf("%s.%s",eptr->prefix,eptr->value);
	else	printf("[unknown cordscript element]");
	return;
}

private	void	cordscript_show_configure( struct cordscript_action * aptr )
{
	int	punctuation='(';
	struct	cordscript_element * eptr;
	cordscript_show_element( aptr->lvalue );
	for ( 	eptr=aptr->rvalue;
		eptr != (struct cordscript_element *) 0;
		eptr = eptr->next )
	{
		printf(" %c ",punctuation);
		cordscript_show_element( eptr );
		punctuation = ',';
	}
	printf(" );\n");
	return;
}
	
private	void	cordscript_show_affect( struct cordscript_action * aptr )
{
	cordscript_show_element( aptr->lvalue );
	printf(" = ");
	cordscript_show_element( aptr->rvalue );
	printf(" ;\n");
	return;
}

public	void	cordscript_show( struct cordscript_action * aptr )
{
	struct	cordscript_element * eptr;
	printf("result of cordscript parsing\n{\n");
	for ( 	; 
		aptr != (struct cordscript_action *) 0; 
		aptr = aptr->next )
	{
		if ( aptr->type == _CORDSCRIPT_CONFIGURE )
			cordscript_show_configure( aptr );
		else if ( aptr->type == _CORDSCRIPT_AFFECT )
			cordscript_show_affect( aptr );
		else	printf("unknown cordscript action type\n");
	}
	printf("}\n");
	return;
}

#endif	/* _cordscript_parser_c */
	/* -------------------- */

