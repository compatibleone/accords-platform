/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/
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

/*	---------------------------------------------------------	*/
/*	      i n i t i a l i s e _ t o k e n _ p a r s e r		*/
/*	---------------------------------------------------------	*/
private	void	initialise_token_parser( char * vptr )
{
	Csp.value = vptr;
	Csp.ungotc = Csp.columns = Csp.bytes = Csp.lines = 0;
	Csp.echo = 0;
	return;
}

/*	---------------------------------------------------------	*/
/*		c o r d s c r i p t _ g e t c h				*/
/*	---------------------------------------------------------	*/
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

/*	---------------------------------------------------------	*/
/*		c o r d s c r i p t _ u n g e t c h			*/
/*	---------------------------------------------------------	*/
private	void	cordscript_ungetch(int c)
{
	Csp.ungotc = c;
	return;
}

/*	---------------------------------------------------------	*/
/*		c o r d s c r i p t _ i s _ w h i t e			*/
/*	---------------------------------------------------------	*/
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

/*	---------------------------------------------------------	*/
/*	    c o r d s c r i p t _ i s _ p u n c t u a t i o n		*/
/*	---------------------------------------------------------	*/
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

/*	---------------------------------------------------------	*/
/*	     c o r d s c r i p t _ r e m o v e _ w h i t e		*/
/*	---------------------------------------------------------	*/
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

/*	---------------------------------------------------------	*/
/*		c o r d s c r i p t _ p u n c t u a t i o n		*/
/*	---------------------------------------------------------	*/
private	int	cordscript_punctuation()
{
	int	c;
	if (!(c = cordscript_remove_white()))
		return( 0 );
	else if (!( cordscript_is_punctuation( c ) ))
		return( 0 );
	else	return( cordscript_getch() );
}

/*	---------------------------------------------------------	*/
/*			c o r d s c r i p t _ t o k e n			*/
/*	---------------------------------------------------------	*/
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
	char *	wptr;
	int	c;
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
		while (1)
		{
			if (!( cordscript_token() ))
				return( liberate_cordscript_element( eptr ) );
			else if ((c = cordscript_punctuation()) == '.' )
			{
				if (!( wptr = allocate( strlen( eptr->prefix ) + strlen( Csp.token ) + 2 ) ))
					return( liberate_cordscript_element( eptr ) );
				else
				{
					/* ---------------------------------- */
					/* it is an extended member statement */
					/* ---------------------------------- */
					sprintf(wptr,"%s.%s",eptr->prefix,Csp.token);
					liberate( eptr->prefix );
					eptr->prefix = wptr;
				}
			}
			else
			{
				if ( c ) cordscript_ungetch( c );
				if (!( eptr->value = allocate_string( Csp.token ) ))
					return( liberate_cordscript_element( eptr ) );
				else	break;
			}
		}
	}
	return( eptr );
}

/*	-----------------------		*/
/*	cordscript method names		*/
/*	-----------------------		*/
private	char * cordscript_methodname[] = 
	{
	(char *)	"set",
	(char *)	"configure",
	(char *)	"monitor",
	(char *)	"system",
	(char *)	"fork",
	(char *)	"new",
	(char *)	"delete",
	(char *)	"start",
	(char *)	"stop  ",
	(char *)	"save",
	(char *)	"snapshot",
	(char *)	"build",
	(char *)	"instance",
	(char *)	"resolve",
	(char *)	"invoke",
	(char *) 0
	};


/*	---------------------------------------------------------	*/
/*	     c o r d s c r i p t _ p a r s e _ m e t h o d		*/
/*	---------------------------------------------------------	*/
/*	determine the nature of the cordscript method if any.		*/
/*	---------------------------------------------------------	*/
public	int	cordscript_parse_method( char * token )
{
	int	i=0;
	while ( cordscript_methodname[i] )
	{
		if (!( strcasecmp( Csp.token, cordscript_methodname[i] )))
			return(i);
		else	i++;
	}
	return( _CORDSCRIPT_INVOKE );

}

/*	---------------------------------------------------------	*/
/*	     	c o r d s c r i p t _ m e t h o d			*/
/*	---------------------------------------------------------	*/
/*	returns the method name of a parsed method symbol.		*/
/*	---------------------------------------------------------	*/
public	char *	cordscript_method( int	symbol )
{
	if ( symbol == _CORDSCRIPT_INVOKE )
		return( "invoke" );
	else if ( symbol == _CORDSCRIPT_CONFIGURE )
		return( "configure" );
	else if ( symbol == _CORDSCRIPT_MONITOR )
		return( "monitor" );
	else if ( symbol == _CORDSCRIPT_SYSTEM )
		return( "system" );
	else if ( symbol == _CORDSCRIPT_FORK )
		return( "fork" );
	else if ( symbol > _CORDSCRIPT_INVOKE )
		return( "none" );
	else	return( cordscript_methodname[ symbol ] );
}

/*	---------------------------------------------------------	*/
/*	 c o r d s _ s c r i p t _ p a r s e _ s t a t e m e n t	*/
/*	---------------------------------------------------------	*/
public	struct	cordscript_action * cordscript_parse_statement( char * statement )
{
	int	c;
	char *	wptr;
	struct	cordscript_element * lvalue=(struct cordscript_element *) 0;
	struct	cordscript_element * rvalue=(struct cordscript_element *) 0;
	struct	cordscript_action * aptr=(struct cordscript_action *) 0;
	struct	cordscript_action * root=(struct cordscript_action *) 0;
	struct	cordscript_action * foot=(struct cordscript_action *) 0;

	initialise_token_parser( statement );

	while ( cordscript_token() )
	{
		/* ----------------------------- */
		/* allocate an action controller */
		/* ----------------------------- */
		if (!( aptr = allocate_cordscript_action()))
			return( liberate_cordscript_actions( root ) );
		else if (!( aptr->previous = foot ))
			root = aptr;
		else 	foot->next = aptr;
		foot = aptr;

		/* --------------------------- */
		/* allocate the lvalue element */
		/* --------------------------- */
		if (!( aptr->lvalue = allocate_cordscript_element() ))
			return( liberate_cordscript_actions( root ) );

		/* -------------------------- */
		/* detect the member operator */
		/* -------------------------- */
		else if ((c = cordscript_punctuation()) != '.')
			return( liberate_cordscript_actions( root ) );
		else if (!( aptr->lvalue->prefix = allocate_string( Csp.token ) ))
			return( liberate_cordscript_actions( root ) );

		while (1)
		{
			/* ------------------------- */
			/* retrieve the member token */
			/* ------------------------- */
			if (!( cordscript_token() ))
				return( liberate_cordscript_actions( root ) );

			/* ----------------------------------------------------- */
			/* seperate between method invocation    and affectation */
			/* ----------------------------------------------------- */
			else if (( c = cordscript_punctuation()) == '(' )
			{

				/* ------------------------------------ */
				/* its a method invocation or statement */
				/* ------------------------------------ */
				aptr->type = cordscript_parse_method( Csp.token );
				aptr->lvalue->type = _CORDSCRIPT_METHOD;
				if (!( aptr->lvalue->value = allocate_string( Csp.token ) ))
					return( liberate_cordscript_actions( root ) );
				else if (( aptr->type == _CORDSCRIPT_SYSTEM )
				     ||  ( aptr->type == _CORDSCRIPT_FORK   )) 
				{
					if (!( aptr->rvalue = cordscript_term() ))
						return( liberate_cordscript_actions( root ) );
				}
				else while (1)
				{
					if (!( rvalue = cordscript_term() ))
					{
						if (( aptr->type == _CORDSCRIPT_SYSTEM )
						||  ( aptr->type == _CORDSCRIPT_FORK )
						||  ( aptr->type == _CORDSCRIPT_MONITOR )
						||  ( aptr->type == _CORDSCRIPT_CONFIGURE ))
							return( liberate_cordscript_actions( root ) );
						else if (!( lvalue ))
							break;
						else	return( liberate_cordscript_actions( root ) );
					}
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
				lvalue = (struct cordscript_element *) 0;
				if ( cordscript_punctuation() != ')' )
					return( liberate_cordscript_actions( root ) );
				else if ( cordscript_punctuation() != ';' )
					return( liberate_cordscript_actions( root ) );
				else	break;
			}
			else if (c == '=' )
			{
				/* ------------------------------ */
				/* it is an affectation statement */
				/* ------------------------------ */
				aptr->type = _CORDSCRIPT_AFFECT;
				aptr->lvalue->type = _CORDSCRIPT_PROPERTY;
				if (!( aptr->lvalue->value = allocate_string( Csp.token ) ))
					return( liberate_cordscript_actions( root ) );
				else if (!( aptr->rvalue = cordscript_term() ))
					return( liberate_cordscript_actions( root ) );
				else if ( cordscript_punctuation() != ';' )
					return( liberate_cordscript_actions( root ) );
				else	break;
			}
			else if ( c != '.' )
				return( liberate_cordscript_actions( root ) );
			else if (!( wptr = allocate( strlen( aptr->lvalue->prefix ) + strlen( Csp.token ) + 2 ) ))
				return( liberate_cordscript_actions( root ) );
			else
			{
				/* ---------------------------------- */
				/* it is an extended member statement */
				/* ---------------------------------- */
				sprintf(wptr,"%s.%s",aptr->lvalue->prefix,Csp.token);
				liberate( aptr->lvalue->prefix );
				aptr->lvalue->prefix = wptr;
			}
		}
	}
	return( root );
}

/*	---------------------------------------------------------	*/
/*	     c o r d s c r i p t _ s h o w _ e l e m e n t		*/
/*	---------------------------------------------------------	*/
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

/*	---------------------------------------------------------	*/
/*	    c o r d s c r i p t _ s h o w _ c o n f i g u r e		*/
/*	---------------------------------------------------------	*/
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
	
/*	---------------------------------------------------------	*/
/*	    c o r d s c r i p t _ s h o w _ m o n i t o r 		*/
/*	---------------------------------------------------------	*/
private	void	cordscript_show_monitor( struct cordscript_action * aptr )
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
	
/*	---------------------------------------------------------	*/
/*	    c o r d s c r i p t _ s h o w _ c o m m a n d		*/
/*	---------------------------------------------------------	*/
private	void	cordscript_show_command( struct cordscript_action * aptr )
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
	
/*	---------------------------------------------------------	*/
/*	    c o r d s c r i p t _ s h o w _  m e t h o d		*/
/*	---------------------------------------------------------	*/
private	void	cordscript_show_method( struct cordscript_action * aptr )
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
	
/*	---------------------------------------------------------	*/
/*	    c o r d s c r i p t _ s h o w _ a f f e c t			*/
/*	---------------------------------------------------------	*/
private	void	cordscript_show_affect( struct cordscript_action * aptr )
{
	cordscript_show_element( aptr->lvalue );
	printf(" = ");
	cordscript_show_element( aptr->rvalue );
	printf(" ;\n");
	return;
}

/*	---------------------------------------------------------	*/
/*		     c o r d s c r i p t _ s h o w			*/
/*	---------------------------------------------------------	*/
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
		else if ( aptr->type == _CORDSCRIPT_MONITOR )
			cordscript_show_monitor( aptr );
		else if ( aptr->type == _CORDSCRIPT_SYSTEM )
			cordscript_show_command( aptr );
		else if ( aptr->type == _CORDSCRIPT_FORK )
			cordscript_show_command( aptr );
		else if ( aptr->type == _CORDSCRIPT_AFFECT )
			cordscript_show_affect( aptr );
		else	cordscript_show_method( aptr );
	}
	printf("}\n");
	return;
}

#endif	/* _cordscript_parser_c */
	/* -------------------- */

