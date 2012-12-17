#ifndef	_cspi_c
#define	_cspi_c

#include "cspi.h"

private	struct	cordscript_label * labelheap=(struct cordscript_label *) 0;
private	struct	cordscript_exception * exceptionheap=(struct cordscript_exception *) 0;

private	int	crop_matrix_element( struct cordscript_value * rptr, struct cordscript_value * sptr );
private	int	crop_structure_element( struct cordscript_value * nptr, struct cordscript_value * rptr, struct cordscript_value * sptr );
private	struct cordscript_instruction * handle_else( struct cordscript_context * cptr,struct cordscript_label * lptr );
private	struct cordscript_instruction * handle_catch( struct cordscript_context * cptr,struct cordscript_label * lptr );
private	int	end_of_instruction=0;
private	int	echo=0;
private	int	abandon_compile=0;

/*   ----------*/
/*   show_value*/
/*   ----------*/
private	void	show_value( struct cordscript_value * vptr )
{
	if ( vptr )
	{
		if ( vptr->body )
		{
			printf("function %s()",vptr->name);
		}
		else
		{
			if ( vptr->name )
				printf("%s = ",vptr->name);
			else	printf("[value] = ");


			if ( vptr->code )
				printf("(*function)()");
			else if ( vptr->value )
				printf("%s",vptr->value);
		}
		printf("\n");
	}
}

/*	-------------------------	*/
/*	liberate_cordscript_label	*/
/*	-------------------------	*/
private	int	labels=0;
public	struct	cordscript_label * liberate_cordscript_label( struct cordscript_label * lptr )
{
	if ( lptr )
	{
		if ( labels ) labels--;
		liberate( lptr );
	}
	return((struct cordscript_label *) 0);
}

/*	-------------------------	*/
/*	allocate_cordscript_label	*/
/*	-------------------------	*/
public	struct	cordscript_label * allocate_cordscript_label( struct cordscript_instruction * iptr, int type )
{
	struct	cordscript_label * lptr;
	labels++;
	if (!( lptr = allocate( sizeof( struct cordscript_label ) ) ))
		return( lptr );
	else
	{
		lptr->value = iptr;
		lptr->type  = type;
		lptr->next  = labelheap;
		labelheap   = lptr;
		return( lptr );
	}
}

/*	-------------------------	*/
/*	liberate_cordscript_exception	*/
/*	-------------------------	*/
private	int	exceptions=0;
public	struct	cordscript_exception * liberate_cordscript_exception( struct cordscript_exception * xptr )
{
	if ( xptr )
	{
		if ( exceptions ) exceptions--;
		liberate( xptr );
	}
	return((struct cordscript_exception *) 0);
}

/*	-------------------------	*/
/*	allocate_cordscript_exception	*/
/*	-------------------------	*/
public	struct	cordscript_exception * allocate_cordscript_exception( struct cordscript_instruction * iptr )
{
	struct	cordscript_exception * xptr;
	exceptions++;
	if (!( xptr = allocate( sizeof( struct cordscript_exception ) ) ))
		return( xptr );
	else
	{
		xptr->handler = iptr;
		xptr->next  = exceptionheap;
		exceptionheap   = xptr;
		return( xptr );
	}
}

/*	----------------------------	*/
/*	liberate_cordscript_function	*/
/*	----------------------------	*/
private	int	values=0;
public struct cordscript_value * liberate_cordscript_function(struct cordscript_value * vptr)
{
	struct	cordscript_instruction * iptr;
	if ( vptr )
	{
		if ( vptr->name )
			liberate( vptr->name );

		if ( vptr->value )
			liberate( vptr->value );

		if ( vptr->body )
			liberate_cordscript_context( vptr->body );

		while ((iptr = vptr->code) != (struct cordscript_instruction *) 0)
		{
			vptr->code = iptr->next;
			liberate_cordscript_instruction( iptr );
		}
		if ( values ) values--;
		liberate( vptr );
	}
	return((struct cordscript_value *) 0);
}

/*	-------------------------	*/
/*	liberate_cordscript_value	*/
/*	-------------------------	*/
public struct cordscript_value * liberate_cordscript_value(struct cordscript_value * vptr)
{
	if ( vptr )
	{
		if ( vptr->name )
			liberate( vptr->name );
		if ( vptr->value )
			liberate( vptr->value );
		if ( values ) values--;
		liberate( vptr );
	}
	return((struct cordscript_value *) 0);
}

/*	-------------------------	*/
/*	allocate_cordscript_value	*/
/*	-------------------------	*/
public struct cordscript_value * allocate_cordscript_value(char * value, char * name)
{
	struct	cordscript_value * vptr;
	values++;
	if (!( vptr = allocate( sizeof( struct cordscript_value ) ) ))
		return( vptr );
	else
	{
		memset( vptr, 0, sizeof( struct cordscript_value ) );
		if ( value )
			if (!( vptr->value = allocate_string( value ) ))
				return( liberate_cordscript_value( vptr ) );
		if ( name )
			if (!( vptr->name = allocate_string( name ) ))
				return( liberate_cordscript_value( vptr ) );
		return( vptr );
	}
}

/*   -----------------*/
/*   instruction_value*/
/*   -----------------*/
private	struct cordscript_value * instruction_value( struct cordscript_instruction * iptr )
{
 	struct	cordscript_value * vptr;
	if (!( vptr = allocate_cordscript_value( (char *) 0, (char *) 0) ))
		return(vptr);
	else
	{
		vptr->code = iptr;
		return( vptr );
	}
}

private	struct cordscript_value * string_value( char * sptr )
{
	return( allocate_cordscript_value( sptr, (char *) 0 ) );
}

private	struct cordscript_value * integer_value( int v )
{
	char	buffer[256];
	sprintf(buffer,"%u",v);
	return( string_value( buffer ) );
}

private	struct cordscript_value * null_value()
{
	return( allocate_cordscript_value( "", (char *) 0 ) );
}

private	void	push_value( struct cordscript_context * cptr, struct cordscript_value * vptr )
{
	vptr->next = cptr->stack;
	cptr->stack = vptr;
	return;
}

/*	----------------------------------	*/
/*		allocate_cordscript_operand	*/
/*	----------------------------------	*/
private	int	operands=0;
private	struct cordscript_operand * allocate_cordscript_operand( struct cordscript_value * vptr )
{
	struct cordscript_operand * optr;
	operands++;
	if (!( optr = allocate( sizeof( struct cordscript_operand ) )))
		return( optr );
	else
	{
		memset( optr, 0, sizeof( struct cordscript_operand ));
		optr->value = vptr;
		return( optr );
	}
}

/*	-----------	*/
/*	add_operand 	*/
/*	-----------	*/
private	void	add_operand( struct cordscript_instruction * iptr, struct cordscript_value * vptr )
{
	struct	cordscript_operand * optr;
	if (!( optr = allocate_cordscript_operand( vptr )))
		return;
	else if (!( iptr->last ))
		iptr->first = optr;
	else
		iptr->last->next = optr;
	iptr->last = optr;
	return;
}

/*	---------------------------		*/
/*	liberate_cordscript_operand		*/
/*	---------------------------		*/
public struct cordscript_operand * liberate_cordscript_operand(struct cordscript_operand * optr)
{
	struct	cordscript_value * vptr;
	if ( optr )
	{
		if ((vptr = optr->value) != (struct cordscript_value *) 0)
		{
			if (!( vptr->name ))
				vptr = liberate_cordscript_value( vptr );
		}
		if ( operands ) operands--;
		liberate( optr );
	}
	return( (struct cordscript_operand *) 0);
}

/*	-------------------------------		*/
/*	liberate_cordscript_instruction		*/
/*	-------------------------------		*/
private	int	instructions=0;
public struct cordscript_instruction * liberate_cordscript_instruction(struct cordscript_instruction * iptr)
{
	struct	cordscript_operand * optr;
	if ( iptr )
	{
		while ((optr = iptr->first) != (struct cordscript_operand *) 0)
		{
			iptr->first = optr->next;
			optr = liberate_cordscript_operand( optr );
		}
		if ( instructions ) instructions--;
		liberate( iptr );
	}
	return( (struct cordscript_instruction *) 0);
}

/*	---------------------------	*/
/*	liberate_cordscript_context	*/
/*	---------------------------	*/
public struct cordscript_context * liberate_cordscript_context( struct cordscript_context * xptr )
{
	struct	cordscript_value * vptr;
	struct	cordscript_instruction * iptr;
	if ( xptr )
	{
		while ((vptr = xptr->stack) != (struct cordscript_value *) 0)
		{
			xptr->stack = vptr->next;
			vptr = liberate_cordscript_value( vptr );
		}
		while ((vptr = xptr->data) != (struct cordscript_value *) 0)
		{
			xptr->data = vptr->next;
			vptr = liberate_cordscript_value( vptr );
		}
		while ((vptr = xptr->code) != (struct cordscript_value *) 0)
		{
			xptr->code = vptr->next;
			vptr = liberate_cordscript_function( vptr );
		}
		while ((iptr = xptr->cs) != (struct cordscript_instruction *) 0)
		{
			xptr->cs = iptr->next;
			iptr = liberate_cordscript_instruction( iptr );
		}
		liberate( xptr );
	}
	return( (struct cordscript_context *) 0 );
}

/*	----------	*/
/*	drop_value	*/
/*	----------	*/
private	void	drop_value( struct cordscript_value * vptr )
{
	if ( vptr )
		if (!( vptr->name ))
			liberate_cordscript_value( vptr );
	return;
}

/*   ---------  */
/*   pop_stack  */
/*   ---------  */
private	struct	cordscript_value * pop_stack( struct cordscript_context * cptr )
{
	struct	cordscript_value * vptr;
	if (!( vptr = cptr->stack ))
		return( vptr );
	else
	{
		cptr->stack = vptr->next;
		return( vptr );
	}
}

/*	-------------------------	*/
/*		no_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * no_operation( struct cordscript_instruction * iptr )
{
	if ( check_debug() )
		printf("no_operation();\n");

	if (!( iptr ))
		return( (struct cordscript_instruction *) 0);
	else	return( iptr->next );
}

/*	-------------------------	*/
/*		pop_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * pop_operation( struct cordscript_instruction * iptr )
{
	struct	cordscript_value * vptr;
	if ( check_debug() )
		printf("pop_operation();\n");


	if (!( iptr ))
		return( (struct cordscript_instruction *) 0);
	else
	{
		if ((vptr = pop_stack(iptr->context)) != (struct cordscript_value*) 0)
			drop_value( vptr );
		return( iptr->next );
	}
}

/*	-------------------------	*/
/*		push_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * push_operation( struct cordscript_instruction * iptr )
{
	if ( check_debug() )
		printf("push_operation();\n");

	/* --------------------------------------- */
	/* simply push any operands onto the stack */
	/* --------------------------------------- */
	if (( iptr->first )
	&&  ( iptr->first->value )
	&&  ( iptr->first->value->value ))
		push_value( iptr->context, string_value( iptr->first->value->value ) );

	if (!( iptr ))
		return( (struct cordscript_instruction *) 0);
	else	return( iptr->next );
}


/*	-------------------------	*/
/*		dup_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * dup_operation( struct cordscript_instruction * iptr )
{
	struct	cordscript_value * vptr;

	if ( check_debug() )
		printf("dup_operation();\n");

	if (!( iptr ))
		return( (struct cordscript_instruction *) 0);
	else
	{
		if ((vptr = pop_stack(iptr->context)) != (struct cordscript_value*) 0)
		{
			push_value( iptr->context, vptr );
			push_value( iptr->context, string_value( vptr->value ) );
		}
		return( iptr->next );
	}
}

/*	-------------------------	*/
/*	 set_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * set_operation( struct cordscript_instruction * iptr )
{
	struct	cordscript_operand * optr;
	struct	cordscript_value * vptr;
	struct	cordscript_value * wptr;

	if ( check_debug() )
		printf("set_operation();\n");
	
	if (((optr = iptr->first) != (struct cordscript_operand *) 0)
	&&  ((vptr = optr->value)  != (struct cordscript_value *)   0))
	{
		if ((wptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
		{
			if ( vptr->value )
				liberate( vptr->value );
			if ( wptr->value )
				vptr->value = allocate_string( wptr->value );
			else	vptr->value = allocate_string( "" );

			drop_value( wptr );
		}
	}

	if (!( iptr ))
		return( (struct cordscript_instruction *) 0);
	else	return( iptr->next );
}

/*	----------------	*/
/*	   add_array		*/
/*	----------------	*/
private	char *	add_array( char * aptr, char * hptr, char * vptr )
{
	char *	rptr;
	if (!( vptr ))
		return( aptr );
	else if (!( aptr ))
	{
		if (!( rptr = allocate( strlen( hptr ) + strlen( vptr ) + 16 ) ))
			return( rptr );
		else
		{
			sprintf(rptr,"[%s%s",hptr,vptr);
			return( rptr );
		}
	}
	else if ( *aptr != '[' )
		return( aptr );
	else
	{
		if (!( rptr = allocate( strlen( hptr ) + strlen(aptr) + strlen( vptr ) + 16 ) ))
			return( rptr );
		else
		{
			sprintf(rptr,"%s,%s%s",aptr,hptr,vptr);
			aptr = liberate( aptr );
			return( rptr );
		}
	}
}

/*	----------------	*/
/*	  close_array		*/
/*	----------------	*/
private	char *	close_array( char * aptr )
{
	if (!( aptr ))
		return( aptr );
	else if ( *aptr != '[' )
		return( aptr );
	else 	return( strcat( aptr, "]" ) );
}

/*	----------------	*/
/*	   add_structure		*/
/*	----------------	*/
private	char *	add_structure( char * aptr, char * hptr, char * vptr )
{
	char *	rptr;
	if (!( vptr ))
		return( aptr );
	else if (!( aptr ))
	{
		if (!( rptr = allocate( strlen( hptr ) + strlen( vptr ) + 16 ) ))
			return( rptr );
		else
		{
			sprintf(rptr,"{%s%s",hptr,vptr);
			return( rptr );
		}
	}
	else if ( *aptr != '{' )
		return( aptr );
	else
	{
		if (!( rptr = allocate( strlen( hptr ) + strlen(aptr) + strlen( vptr ) + 16 ) ))
			return( rptr );
		else
		{
			sprintf(rptr,"%s,%s%s",aptr,hptr,vptr);
			aptr = liberate( aptr );
			return( rptr );
		}
	}
}

/*	----------------	*/
/*	  close_structure		*/
/*	----------------	*/
private	char *	close_structure( char * aptr )
{
	if (!( aptr ))
		return( aptr );
	else if ( *aptr != '[' )
		return( aptr );
	else 	return( strcat( aptr, "}" ) );
}



/*	------------		*/
/*	join_array		*/
/*	------------		*/
private	struct	cordscript_value * join_array( char * lptr, char * rptr )
{
	int	i;
	if (!( rptr ))
	{
		if (!( lptr ))
			return( string_value( "" ) );
		else	return( string_value( lptr ) );
	}
	else if (!( lptr ))
		return( string_value( rptr ) );
	else if (!( lptr = allocate_string( lptr ) ))
		return( string_value("[error]" ) );
	else
	{
		i = strlen(lptr);
		while ( i > 0 )
		{
			--i;
			if ( *(lptr+i) == ']' )
			{
				*(lptr+i) = 0;
				break;
			} 
		}
		if (!( lptr = add_array( lptr, rptr, "" ) ))
			return( string_value( "[error]" ) );
		else	return( string_value( close_array( lptr ) ));
	}
}

/*	--------------		*/
/*	join_structure		*/
/*	--------------		*/
private	struct	cordscript_value * join_structure( char * lptr, char * rptr )
{
	int	i;
	if (!( rptr ))
	{
		if (!( lptr ))
			return( string_value( "" ) );
		else	return( string_value( lptr ) );
	}
	else if (!( lptr ))
		return( string_value( rptr ) );
	else if (!( lptr = allocate_string( lptr ) ))
		return( string_value("[error]" ) );
	else
	{
		i = strlen(lptr);
		while ( i > 0 )
		{
			--i;
			if ( *(lptr+i) == '}' )
			{
				*(lptr+i) = 0;
				break;
			} 
		}
		if (!( lptr = add_structure( lptr, rptr, "" ) ))
			return( string_value( "[error]" ) );
		else	return( string_value( close_structure( lptr ) ));
	}
}

/*	------------		*/
/*	join_strings		*/
/*	------------		*/
private	struct	cordscript_value * join_value_strings( char * lptr, char * rptr )
{
	char *	wptr;
	struct	cordscript_value * vptr;
	if ( lptr )
	{
		if (!( rptr ))
			return( string_value( lptr ) );
		else if (!( wptr = allocate( strlen( lptr ) + strlen( rptr ) +1 ) ))
			return( string_value("[Error]") );
		else
		{
			sprintf(wptr,"%s%s",lptr,rptr);
			vptr = string_value( wptr );
			liberate( wptr );
			return( vptr );
		}
	}
	else if (!( rptr ))
		return( string_value( "" ) );
	else	return( string_value( rptr ) );
}


private	int	check_value_type( char * sptr )
{
	if (!( sptr ))
		return( _INTEGER_VALUE );
	else if ( *sptr == '[' )
		return( _ARRAY_VALUE );
	else if ( *sptr == '{' )
		return( _STRUCTURE_VALUE );
	else if (( *sptr >= '0' ) && ( *sptr <= '9' ))
		return( _INTEGER_VALUE );
	else if ( *sptr == '"' )
		return( _STRING_VALUE );
	else if (( *sptr == '+' ) || ( *sptr == '-' ))
		return( _INTEGER_VALUE );
	else	return( _STRING_VALUE );
}


/*	--------------		*/
/*	 cat_operation		*/
/*	--------------		*/
private	struct	cordscript_instruction * cat_operation( struct cordscript_instruction * iptr )
{
	struct	cordscript_operand * optr;
	struct	cordscript_value * vptr;
	struct	cordscript_value * wptr;
	int	value=0;
	if ( check_debug() )
		printf("cat_operation();\n");
	
	if (((optr = iptr->first) != (struct cordscript_operand *) 0)
	&&  ((vptr = optr->value)  != (struct cordscript_value *)   0))
	{
		if ((wptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
		{
			push_value( iptr->context, join_value_strings( vptr->value, wptr->value ) );
			drop_value( wptr );
		}
	}

	if (!( iptr ))
		return( (struct cordscript_instruction *) 0);
	else	return( iptr->next );
}
/*	--------------		*/
/*	 add_operation		*/
/*	--------------		*/
private	struct	cordscript_instruction * add_operation( struct cordscript_instruction * iptr )
{
	struct	cordscript_operand * optr;
	struct	cordscript_value * vptr;
	struct	cordscript_value * wptr;
	int	value=0;
	if ( check_debug() )
		printf("add_operation();\n");
	
	if (((optr = iptr->first) != (struct cordscript_operand *) 0)
	&&  ((vptr = optr->value)  != (struct cordscript_value *)   0))
	{
		if ((wptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
		{
			switch ( check_value_type( vptr->value ) )
			{
			case	_INTEGER_VALUE	:
				/* numeric addition */
				/* ---------------- */
				if ( vptr->value )
					value = atoi( vptr->value );
				if ( wptr->value )
					value += atoi( wptr->value );
				push_value( iptr->context, integer_value( value ) );
				break;
			case	_STRING_VALUE	:
				/* string concatenation */
				/* -------------------- */
				push_value( iptr->context, join_value_strings( vptr->value, wptr->value ) );
				break;
			case	_ARRAY_VALUE	:
				/* add array elements */
				/* ------------------ */
				push_value( iptr->context, join_array( vptr->value, wptr->value ) );
				break;
			case	_STRUCTURE_VALUE	:
				/* add structure members */
				/* --------------------- */
				push_value( iptr->context, join_structure( vptr->value, wptr->value ) );
				break;
			default		:
				push_value( iptr->context, string_value( "[error]" ) );
				break;

			}
			
			drop_value( wptr );
		}
	}

	if (!( iptr ))
		return( (struct cordscript_instruction *) 0);
	else	return( iptr->next );
}

/*	--------------		*/
/*	 sub_operation		*/
/*	--------------		*/
private	struct	cordscript_instruction * sub_operation( struct cordscript_instruction * iptr )
{
	struct	cordscript_operand * optr;
	struct	cordscript_value * vptr;
	struct	cordscript_value * wptr;
	int	value=0;
	if ( check_debug() )
		printf("sub_operation();\n");
	
	if (((optr = iptr->first) != (struct cordscript_operand *) 0)
	&&  ((vptr = optr->value)  != (struct cordscript_value *)   0))
	{
		if ( vptr->value )
			value = atoi( vptr->value );
		if ((wptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
		{
			if ( wptr->value )
				value -= atoi( wptr->value );
			push_value( iptr->context, integer_value( value ) );
			drop_value( wptr );
		}
	}

	if (!( iptr ))
		return( (struct cordscript_instruction *) 0);
	else	return( iptr->next );
}

/*	--------------		*/
/*	 mul_operation		*/
/*	--------------		*/
private	struct	cordscript_instruction * mul_operation( struct cordscript_instruction * iptr )
{
	struct	cordscript_operand * optr;
	struct	cordscript_value * vptr;
	struct	cordscript_value * wptr;
	int	value=0;
	if ( check_debug() )
		printf("mul_operation();\n");
	
	if (((optr = iptr->first) != (struct cordscript_operand *) 0)
	&&  ((vptr = optr->value)  != (struct cordscript_value *)   0))
	{
		if ( vptr->value )
			value = atoi( vptr->value );
		if ((wptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
		{
			if ( wptr->value )
				value *= atoi( wptr->value );
			push_value( iptr->context, integer_value( value ) );
			drop_value( wptr );
		}
	}

	if (!( iptr ))
		return( (struct cordscript_instruction *) 0);
	else	return( iptr->next );
}

/*	--------------		*/
/*	 div_operation		*/
/*	--------------		*/
private	struct	cordscript_instruction * div_operation( struct cordscript_instruction * iptr )
{
	struct	cordscript_operand * optr;
	struct	cordscript_value * vptr;
	struct	cordscript_value * wptr;
	int	value=0;
	if ( check_debug() )
		printf("div_operation();\n");
	
	if (((optr = iptr->first) != (struct cordscript_operand *) 0)
	&&  ((vptr = optr->value)  != (struct cordscript_value *)   0))
	{
		if ( vptr->value )
			value = atoi( vptr->value );
		if ((wptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
		{
			if ( wptr->value )
				value /= atoi( wptr->value );
			push_value( iptr->context, integer_value( value ) );
			drop_value( wptr );
		}
	}

	if (!( iptr ))
		return( (struct cordscript_instruction *) 0);
	else	return( iptr->next );
}

/*	--------------		*/
/*	 mod_operation		*/
/*	--------------		*/
private	struct	cordscript_instruction * mod_operation( struct cordscript_instruction * iptr )
{
	struct	cordscript_operand * optr;
	struct	cordscript_value * vptr;
	struct	cordscript_value * wptr;
	int	value=0;
	if ( check_debug() )
		printf("mod_operation();\n");
	
	if (((optr = iptr->first) != (struct cordscript_operand *) 0)
	&&  ((vptr = optr->value)  != (struct cordscript_value *)   0))
	{
		if ( vptr->value )
			value = atoi( vptr->value );
		if ((wptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
		{
			if ( wptr->value )
				value %= atoi( wptr->value );
			push_value( iptr->context, integer_value( value ) );
			drop_value( wptr );
		}
	}

	if (!( iptr ))
		return( (struct cordscript_instruction *) 0);
	else	return( iptr->next );
}

/*	------------------------	*/
/*	build_category_reference	*/
/*	------------------------	*/
private	char *	build_category_reference( char * name )
{
	char buffer[1024];
	char *	id;
	if (!( name ))
		return( name );
	else if (!( strcmp(name,"publication") ))
	{
		sprintf(buffer,"%s/%s/",default_publisher(),name);
		return( allocate_string( buffer ) );
	}
	else if (!( id = occi_resolve_category_provider( name, "Cordscripter", default_tls() ) ))
		return( id );
	else
	{
		sprintf(buffer,"%s/%s/",id,name);
		liberate(id);
		return( allocate_string( buffer ) );
	}
}

/*	----------------	*/
/*	evaluation_value	*/
/*	----------------	*/
private	char *	evaluation_value( char * sptr )
{
	char *	rptr;
	if (!( sptr ))
		return( sptr );
	else 	return( allocate_string( sptr ) );
}

/*	------------------------	*/
/*	cordscript_string_filter	*/
/*	------------------------	*/
private	struct occi_element * cordscript_string_filter( char * wptr )
{
	struct	occi_element * dptr=(struct occi_element *) 0;
	char	*	sptr;
	for ( sptr=wptr; *sptr; sptr++ )
		if ( *sptr == '=' )
			break;
	if ( *sptr == '=' ) 
		*(sptr++) = 0;
	if (!( dptr = occi_create_element( wptr, sptr ) ))
		return( dptr );
	else if (!( dptr->name = occi_unquoted_value( dptr->name ) ))
		return( dptr );
	else if (!( dptr->value = occi_unquoted_value( dptr->value ) ))
		return( dptr );
	else	return( dptr );
}

/*	------------------------	*/
/*	cordscript_member_filter	*/
/*	------------------------	*/
private	struct occi_element * cordscript_member_filter( char * wptr )
{
	struct	occi_element * dptr=(struct occi_element *) 0;
	char	*	sptr;
	for ( sptr=wptr; *sptr; sptr++ )
		if ( *sptr == ':' )
			break;
	if ( *sptr == ':' ) 
		*(sptr++) = 0;
	if (!( dptr = occi_create_element( wptr, sptr ) ))
		return( dptr );
	else if (!( dptr->name = occi_unquoted_value( dptr->name ) ))
		return( dptr );
	else if (!( dptr->value = occi_unquoted_value( dptr->value ) ))
		return( dptr );
	else	return( dptr );
}

/*	----------------------	*/
/*	cordscript_occi_filter	*/
/*	----------------------	*/
private	struct occi_element * cordscript_occi_filter( char * vptr )
{
	struct	occi_element * root=(struct occi_element *) 0;
	struct	occi_element * foot=(struct occi_element *) 0;
	struct	occi_element * dptr=(struct occi_element *) 0;
	char *	aptr;
	char *	wptr;
	char *	xptr;
	char *	sptr;
	int	c;
	if (!( vptr ))
		return( root );
	else if (!( wptr = allocate_string( vptr ) ))
		return( root );
	else	aptr = wptr;
	
	if ( *wptr == '[' )
	{
		/* array of values */
		wptr++;
		while (1)
		{
			while ( *wptr )
			{
				if ( *wptr != ' ') 
					break;
				else	wptr++;
			}
			xptr = wptr;
			while ( *wptr )
			{
				if (( *wptr == ',')
				||  ( *wptr == ']'))
					break;
				else	wptr++;
			}
			if ((c = *wptr) != 0)
				*(wptr++) = 0;
			if (!( dptr = cordscript_string_filter( xptr )))
				break;
			else if (!( dptr->previous = foot ))
				root = dptr;
			else	foot->next = dptr;
			foot = dptr;
			if ((!( c )) || ( c == ']' ))
				break;			
		}
	}
	else if ( *wptr == '{' )
	{
		/* structure of name value pairs */
		wptr++;
		while (1)
		{
			while ( *wptr )
			{
				if ( *wptr != ' ') 
					break;
				else	wptr++;
			}
			xptr = wptr;
			while ( *wptr )
			{
				if (( *wptr == ',')
				||  ( *wptr == '}'))
					break;
				else	wptr++;
			}
			if ((c = *wptr) != 0)
				*(wptr++) = 0;
			if (!( dptr = cordscript_member_filter( xptr )))
				break;
			else if (!( dptr->previous = foot ))
				root = dptr;
			else	foot->next = dptr;
			foot = dptr;
			if ((!( c )) || ( c == '}' ))
				break;			
		}
	}
	/* simple string value */
	else if ((dptr = cordscript_string_filter( wptr )) != (struct occi_element *) 0)
	{
		if (!( dptr->previous = foot ))
			root = dptr;
		else	foot->next = dptr;
		foot = dptr;
	}
	if ( aptr )
		liberate( aptr );
	return( root );
}

/*	---------------		*/
/*	 enter_operation		*/
/*	---------------		*/
private	struct	cordscript_instruction * enter_operation( struct cordscript_instruction * iptr )
{
	struct	cordscript_operand * optr;
	struct	cordscript_value * vptr=(struct	cordscript_value *) 0;
	if ( check_debug() )
		printf("enter_operation();\n");

	if (((optr = iptr->first) != (struct cordscript_operand *) 0)
	&&  ((vptr = optr->value) != (struct cordscript_value   *) 0)
        &&  ( vptr->code ))
	{
		allocate_cordscript_exception( vptr->code );
	}
	
	iptr->context->ip = iptr->next;

	return(iptr->context->ip);
}

/*	---------------		*/
/*	 leave_operation		*/
/*	---------------		*/
private	struct	cordscript_instruction * leave_operation( struct cordscript_instruction * iptr )
{
	struct	cordscript_operand * optr;
	struct	cordscript_value * vptr=(struct	cordscript_value *) 0;
	struct	cordscript_exception * xptr;
	if ( check_debug() )
		printf("leave_operation();\n");

	if (( xptr = exceptionheap ) != (struct cordscript_exception *) 0)
	{
		exceptionheap = xptr->next;
		liberate_cordscript_exception( xptr) ;
	}

	if (((optr = iptr->first) != (struct cordscript_operand *) 0)
	&&  ((vptr = optr->value) != (struct cordscript_value   *) 0)
        &&  ( vptr->code ))
		iptr->context->ip = vptr->code;
	else	iptr->context->ip = iptr->next;

	return(iptr->context->ip);
}

/*	----------------	*/
/*	join_operation		*/
/*	----------------	*/
private	void	join_operation( struct cordscript_instruction * iptr,char * source, struct cordscript_value * separator )
{
	return;
}

/*	----------------	*/
/*	cut_operation		*/
/*	----------------	*/
private	void	cut_operation( struct cordscript_instruction * iptr,  char * source, struct cordscript_value * separator )
{
	char *	rptr=(char *) 0;
	char *	sptr;
	char *	wptr;
	char *	xptr;	
	int	cutat = ( separator ? ( separator->value ? *(separator->value) : ' ' ) : ' ' );
	if (!( sptr = allocate_string( source ) ))
		push_value( iptr->context, string_value("" ) );
	else
	{
		for ( 	xptr=wptr=sptr;
			*wptr != 0;
			wptr++ )
		{
			if ( *wptr != cutat )
				continue;
			else 
			{
				*(wptr++) = 0;
				if (!( rptr = add_array( rptr, xptr, "" )))
					break;
				else	xptr = wptr;
			}
		}
		if ( strlen( xptr ) > 0 )
			rptr = add_array( rptr, xptr, "" );
		push_value( iptr->context, string_value( close_array( rptr ) ) );
		return;
	}
}

/*	----------------------	*/
/*	   duplicate_value	*/
/*	----------------------	*/
private	struct	cordscript_value * duplicate_value(struct cordscript_value * source)
{
	return( allocate_cordscript_value( source->value, source->name ) );
}


/*	--------------------------	*/
/*	structure_member_operation	*/
/*	--------------------------	*/
private	int	structure_member_operation( struct cordscript_instruction * iptr,  struct cordscript_value * source, struct cordscript_value * selector  )
{
	char *	item;
	struct	cordscript_value *	result;
	struct	cordscript_value *	token;

	switch ( check_value_type( selector->value ) )
	{
	case	_INTEGER_VALUE	:
		return(0);
	case	_STRING_VALUE	:
		if (!( item = selector->value ))
			return(0);
		else if (!( source = duplicate_value( source ) ))
			return(0);
		else if (!( result = allocate_cordscript_value( (char *) 0, (char *) 0) ))
			return(0);
		else if (!( token = allocate_cordscript_value( (char *) 0, (char *) 0) ))
			return(0);
		else
		{
			while ( crop_structure_element( token, result, source ) )
			{
				if (!( token->value ))
					break;
				else if (!( strcmp( token->value, item ) ))
				{
					push_value( iptr->context, result );
					drop_value( token );
					drop_value( source );
					return(1);
				}					
			}
		}
		drop_value( result );
		drop_value( token );
		drop_value( source );
	default	:
		return(0);
	}
}

/*	----------------------	*/
/*	array_member_operation	*/
/*	----------------------	*/
private	int	array_member_operation( struct cordscript_instruction * iptr,  struct cordscript_value * source, struct cordscript_value * selector )
{
	int	item;
	struct	cordscript_value *	result;

	switch ( check_value_type( selector->value ) )
	{
	case	_INTEGER_VALUE	:
		if (!( source = duplicate_value( source ) ))
			return(0);
		else if (!( result = allocate_cordscript_value( (char *) 0, (char *) 0) ))
			return(0);
		else
		{
			item=(selector->value ? atoi( selector->value ) : 0 );
			while (1)
			{
				if (!( crop_matrix_element( result, source ) ))
				{
					drop_value( result );
					drop_value( source );
					return(0);
				}
				else if (!( item ))
					break;
				else	item--;
			}
			push_value( iptr->context, result );
			drop_value( source );
			return(1);
		}

	case	_STRING_VALUE	:
		if (!( source = duplicate_value( source ) ))
			return(0);
		else if (!( result = allocate_cordscript_value( (char *) 0, (char *) 0) ))
			return(0);
		else
		{
			item=(selector->value ? atoi( selector->value ) : 0 );
			while ( crop_matrix_element( result, source ) )
			{
				if ( check_value_type( result->value ) == _STRUCTURE_VALUE )
				{
					if ( structure_member_operation( iptr, result, selector ) )
					{
						drop_value( source );
						drop_value( result );
						return(1);
					}
				}
			}			
			drop_value( result );
			drop_value( source );
			return(0);
		}
	default		:
		return(0);
	}
}

/*	----------------	*/
/*	member_operation	*/
/*	----------------	*/
private	void	member_operation( struct cordscript_instruction * iptr, struct cordscript_value * source, struct cordscript_value * selector  )
{
	if ((!( source )) || (!( source->value )))
		push_value( iptr->context, string_value( "" ) );

	else if (*source->value == '[' )
	{
		if (!( array_member_operation( iptr, source,selector ) ))
			push_value( iptr->context, string_value( "" ) );
	}
	else if (*source->value == '{' )
	{
		if (!( structure_member_operation( iptr, source, selector ) ))
			push_value( iptr->context, string_value( "" ) );
	}
	else	push_value( iptr->context, string_value( "" ) );
	return;
}


/*	---------------		*/
/*	 eval_operation		*/
/*	---------------		*/
private	struct	cordscript_instruction * eval_operation( struct cordscript_instruction * iptr )
{
	struct	cordscript_value * vptr;
	struct	cordscript_value * nptr;
	struct	cordscript_value * wptr;
	struct	cordscript_operand * optr;
	struct	occi_element * dptr=(struct occi_element *) 0;
	struct	occi_response* zptr=(struct occi_response *) 0;
	struct	cordscript_value * 	argv[10];
	int				argc=0;
	int				argi=0;
	char	vbuffer[_MAX_VALUE];
	char *	ihost;
	char *	evalue=(char *) 0;
	char *	aptr;
	char *	tptr;
	char *	sptr;
	char *	lptr;
	int	status;
	int	count=0;
	int	v;

	if ( check_debug() )
		printf("eval_operation();\n");

	for ( argi=0; argi < 10; argi++ )
		argv[argi] = (struct cordscript_value *) 0;

	/* ----------------------------------- */
	/* detect nature of evaluation operand */
	/* ----------------------------------- */
	if (((optr = iptr->first) != (struct cordscript_operand *) 0)
	&&  ((vptr = optr->value) != (struct cordscript_value   *) 0)
	&&  ((optr = optr->next)  != (struct cordscript_operand *) 0)
	&&  ((wptr = optr->value) != (struct cordscript_value   *) 0))
	{
		/* ------------------------- */
		/* check for pushed operands */
		/* ------------------------- */
		if ((( optr = optr->next ) != (struct cordscript_operand *) 0)
		&&  (( nptr = optr->value) != (struct cordscript_value *) 0))
		{
			if ((argc = (nptr->value ? atoi( nptr->value ) : 0)) != 0 )
			{
				for ( argi=0; argi < argc; argi++ )
				{
					if (!( argv[argi] = pop_stack( iptr->context ) ))
						break;
					else if ( check_verbose() )
						printf("argv[%i] = %s \n",argi,( argv[argi]->value ? argv[argi]->value : "0"));
				}
			}
		}

		if (!( strcmp( wptr->value, "display" ) ))
		{
			if ( vptr->value )
				printf("%s\n",vptr->value);
		}
		else if (!( strcmp( wptr->value, "wait" ) ))
		{
			if ( vptr->value )
				sleep( atoi(vptr->value) );
		}

		else if (!( strcmp( wptr->value, "length" ) ))
		{
			push_value( iptr->context, integer_value( ( vptr->value ? strlen( vptr->value ) : 0 ) ));
		}

		else if (!( strcmp( wptr->value, "member" ) ))
		{
			member_operation( iptr, vptr, argv[0] );
		}

		else if (!( strcmp( wptr->value, "cut" ) ))
		{
			cut_operation( iptr, vptr->value, argv[0] );
		}

		else if (!( strcmp( wptr->value, "join" ) ))
		{
			join_operation( iptr, vptr->value, argv[0] );
		}

		else if (!( strcmp( wptr->value, "debug" ) ))
		{
			v = atoi( vptr->value );
			if ( v & 1 )
				rest_show_request( 1 );
			else	rest_show_request( 0 );
			if ( v & 2 )
				echo = 1;
			else	echo = 0;
		}

		/* --------------------------------------- */
		/* check for a universal unique identifier */
		/* --------------------------------------- */
		else if (!( evalue = evaluation_value( vptr->value ) ))
			push_value( iptr->context, string_value("[error]") );

		else if (!( strncmp( evalue, "http", strlen("http" ) ) ))
		{
			/* rest/crud methods for instances */
			/* ------------------------------- */
			if ((!( strcasecmp( wptr->value, "get"      ) ))
			||  (!( strcmp( wptr->value, "retrieve" ) )))
			{
				if (( zptr = occi_simple_get( evalue, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
				{
					aptr = (char *) 0;
					for (	dptr=zptr->first;
						dptr != (struct occi_element *) 0;
						dptr = dptr->next )
					{
						if (!( tptr = dptr->name ))
							continue;
						{
							if (!( strcmp( dptr->name, "link" ) ))
							{
								if (!( lptr = allocate_string( dptr->value ) ))
									continue;
								else if (!( lptr = occi_unquoted_link( lptr ) ))
									continue;
								else
								{
									sprintf(vbuffer,"{%c%s%c:%c%s%c}",
										0x0022,dptr->name,0x0022,
										0x0022,lptr,0x0022);
									lptr = liberate( lptr );
								}
							}
							else if ( *dptr->name == '"' )
							{
								sprintf(vbuffer,"{%s:%s}",
								(dptr->name ? dptr->name : ""), 
								(dptr->value ? dptr->value : "")); 
							}
							else
							{
								sprintf(vbuffer,"{%c%s%c:%c%s%c}",
									0x0022,(dptr->name ? dptr->name : ""), 0x0022,
									0x0022,(dptr->value ? dptr->value : ""), 0x0022);
							}
							if (!( aptr = add_array( aptr, "", vbuffer ) ))
								break;
							else	continue;
						}
					}
					aptr = close_array( aptr );
					push_value( iptr->context, string_value(aptr) );
					liberate( aptr );
					zptr = occi_remove_response( zptr );
				}
				else	push_value( iptr->context, string_value("") );
			}
			/* -------------------------------- */
			/* rest/crud methods for categories */
			/* -------------------------------- */
			else if ((!( strcmp( wptr->value, "new"      ) ))
			     ||  (!( strcmp( wptr->value, "create"   ) ))
			     ||  (!( strcasecmp( wptr->value, "post" ) )))
			{
				if ( argv[0] != (struct cordscript_value *) 0)
					dptr = cordscript_occi_filter( argv[0]->value );
				if (( zptr = occi_simple_post( evalue, dptr, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
				{
					if (( ihost = occi_extract_location( zptr )) != (char *) 0)
						push_value( iptr->context, string_value(ihost) );
					else	push_value( iptr->context, string_value("[error]") );
					zptr = occi_remove_response( zptr );
				}
				else	push_value( iptr->context, string_value("[error]") );
			}
			else if ((!( strcasecmp( wptr->value, "put") ))
			     ||  (!( strcmp( wptr->value, "update" ) )))
			{
				if ( argv[0] != (struct cordscript_value *) 0)
					dptr = cordscript_occi_filter( argv[0]->value );
				if (( zptr = occi_simple_put( evalue, dptr, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
				{
					aptr = (char *) 0;
					for (	dptr=zptr->first;
						dptr != (struct occi_element *) 0;
						dptr = dptr->next )
					{
						if (!( tptr = dptr->name ))
							continue;
						else if (!( tptr = occi_category_id( dptr->value ) ))
							continue;
						else if (!( aptr = add_array( aptr, evalue, tptr ) ))
							break;
						else	continue;
					}
					aptr = close_array( aptr );
					push_value( iptr->context, string_value(aptr) );
					liberate( aptr );
					zptr = occi_remove_response( zptr );
				}
				else	push_value( iptr->context, string_value("[error]") );
			}
			else if (!( strcmp( wptr->value, "list" ) ))
			{
				if ( argv[0] != (struct cordscript_value *) 0)
					dptr = cordscript_occi_filter( argv[0]->value );
				if (( zptr = occi_simple_list( evalue, dptr, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
				{
					aptr = (char *) 0;
					for (	dptr=zptr->first;
						dptr != (struct occi_element *) 0;
						dptr = dptr->next )
					{
						if (!( tptr = dptr->name ))
							continue;
						else if (!( tptr = occi_category_id( dptr->value ) ))
							continue;
						else if (!( aptr = add_array( aptr, evalue, tptr ) ))
							break;
						else	continue;
					}
					aptr = close_array( aptr );
					push_value( iptr->context, string_value(aptr) );
					liberate( aptr );
					zptr = occi_remove_response( zptr );
				}
				else	push_value( iptr->context, string_value("") );
			}
			else if (!( strcmp( wptr->value, "count" ) ))
			{
				count=0;
				if ( argv[0] != (struct cordscript_value *) 0)
					dptr = cordscript_occi_filter( argv[0]->value );
				if (( zptr = occi_simple_list( evalue, dptr, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
				{
					aptr = (char *) 0;
					for (	dptr=zptr->first;
						dptr != (struct occi_element *) 0;
						dptr = dptr->next )
					{
						if (!( tptr = dptr->name ))
							continue;
						else if (!( tptr = occi_category_id( dptr->value ) ))
							continue;
						else 	count++;
					}
					push_value( iptr->context, integer_value(count) );
					zptr = occi_remove_response( zptr );
				}
				else	push_value( iptr->context, string_value("") );
			}
			else if (!( strcasecmp( wptr->value, "delete" ) ))
			{
				if ( argv[0] != (struct cordscript_value *) 0)
					dptr = cordscript_occi_filter( argv[0]->value );
				if (( zptr = occi_simple_delete( evalue, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
					zptr = occi_remove_response( zptr );
			}
			else
			{
				/* it may be an OCCI action */
				/* ------------------------ */
				if (( zptr = ll_cords_invoke_action( evalue, wptr->value, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
					zptr = occi_remove_response( zptr );
				
			}
		}
		/* --------------------------------------- */
		/* else it should be a category identifier */
		/* --------------------------------------- */
		else if (( sptr = build_category_reference( evalue )) != (char *) 0)
		{
			/* -------------------------------- */
			/* rest/crud methods for categories */
			/* -------------------------------- */
			if ((!( strcmp( wptr->value, "new"      ) ))
			||  (!( strcmp( wptr->value, "create"   ) ))
			||  (!( strcasecmp( wptr->value, "post" ) )))
			{
				if ( argv[0] != (struct cordscript_value *) 0)
					dptr = cordscript_occi_filter( argv[0]->value );
				if (( zptr = occi_simple_post( sptr, dptr, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
				{
					if (( ihost = occi_extract_location( zptr )) != (char *) 0)
						push_value( iptr->context, string_value(ihost) );
					else	push_value( iptr->context, string_value("[error]") );
					zptr = occi_remove_response( zptr );
				}
				else	push_value( iptr->context, string_value("[error]") );
			}
			else if (!( strcmp( wptr->value, "list" ) ))
			{
				if ( argv[0] != (struct cordscript_value *) 0)
					dptr = cordscript_occi_filter( argv[0]->value );
				if (( zptr = occi_simple_list( sptr, dptr, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
				{
					aptr = (char *) 0;
					for (	dptr=zptr->first;
						dptr != (struct occi_element *) 0;
						dptr = dptr->next )
					{
						if (!( tptr = dptr->name ))
							continue;
						else if (!( tptr = occi_category_id( dptr->value ) ))
							continue;
						else if (!( aptr = add_array( aptr, sptr, tptr ) ))
							break;
						else	continue;
					}
					aptr = close_array( aptr );
					push_value( iptr->context, string_value(aptr) );
					liberate( aptr );
					zptr = occi_remove_response( zptr );
				}
				else	push_value( iptr->context, string_value("") );
			}
			else if (!( strcmp( wptr->value, "count" ) ))
			{
				count=0;
				if ( argv[0] != (struct cordscript_value *) 0)
					dptr = cordscript_occi_filter( argv[0]->value );
				if (( zptr = occi_simple_list( sptr, dptr, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
				{
					aptr = (char *) 0;
					for (	dptr=zptr->first;
						dptr != (struct occi_element *) 0;
						dptr = dptr->next )
					{
						if (!( tptr = dptr->name ))
							continue;
						else if (!( tptr = occi_category_id( dptr->value ) ))
							continue;
						else 	count++;
					}
					push_value( iptr->context, integer_value(count) );
					zptr = occi_remove_response( zptr );
				}
				else	push_value( iptr->context, string_value("") );
			}
			else if (!( strcmp( wptr->value, "get"  ) ))
			{
				if (( zptr = occi_simple_get( sptr, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
				{
					aptr = (char *) 0;
					for (	dptr=zptr->first;
						dptr != (struct occi_element *) 0;
						dptr = dptr->next )
					{
						if (!( tptr = dptr->name ))
							continue;
						else if (!( tptr = occi_category_id( dptr->value ) ))
							continue;
						else if (!( aptr = add_array( aptr, sptr, tptr ) ))
							break;
						else	continue;
					}
					aptr = close_array( aptr );
					push_value( iptr->context, string_value(aptr) );
					liberate( aptr );
					zptr = occi_remove_response( zptr );
				}
				else	push_value( iptr->context, string_value("") );
			}
			else if (!( strcasecmp( wptr->value, "delete" ) ))
			{
				if ( argv[0] != (struct cordscript_value *) 0)
					dptr = cordscript_occi_filter( argv[0]->value );
				if (( zptr = occi_simple_delete( sptr, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
					zptr = occi_remove_response( zptr );
			}
		}
		else
		{
			/* it may be an OCCI action */
			/* ------------------------ */
			if (( zptr = ll_cords_invoke_action( sptr, wptr->value, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
				zptr = occi_remove_response( zptr );
				
		}
	}

	if ( evalue )
		evalue = liberate( evalue );
	if (!( iptr ))
		return( (struct cordscript_instruction *) 0);
	else	return( iptr->next );
}

/*	-------------------------	*/
/*		jmp_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * jmp_operation( struct cordscript_instruction * iptr )
{
	struct cordscript_operand * optr;
	struct cordscript_value * vptr;
	if ( check_debug() )
		printf("jmp_operation();\n");
	if (!( iptr ))
		return((struct cordscript_instruction *) 0);
	else if (((optr = iptr->first) != (struct cordscript_operand *) 0 ) 
	     &&  ((vptr = optr->value) != (struct cordscript_value *) 0 ) 
	     &&  ( vptr->code ))
		return((iptr->context->ip = vptr->code));
	else	return((struct cordscript_instruction *) 0);
}

/*	-------------------------	*/
/*		jeq_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * jeq_operation( struct cordscript_instruction * iptr )
{
	struct cordscript_operand * optr;
	struct cordscript_value * vptr;
	struct cordscript_value * lptr;
	struct cordscript_value * wptr;

	if ( check_debug() )
		printf("jeq_operation();\n");

	/* ----------------------------- */
	/* collect and validate operands */
	/* ----------------------------- */
	if (((vptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
	&&  ((optr = iptr->first) != (struct cordscript_operand *) 0 ) 
	&&  ((wptr = optr->value) != (struct cordscript_value *) 0 ) 
	&&  ((optr = optr->next ) != (struct cordscript_operand *) 0 ) 
	&&  ((lptr = optr->value) != (struct cordscript_value *) 0 ) 
	&&  ( lptr->code ))
 	{
		/* ---------------------------------------- */
		/* test for true condition to take the jump */
		/* ---------------------------------------- */
		if ((!( wptr->value )) && (!( vptr->value )))
		{
			iptr->context->ip = lptr->code;
			drop_value( vptr );
			return(iptr->context->ip);
		}
		else if (( wptr->value ) && ( vptr->value ))
		{

			switch( check_value_type( vptr->value ) )
			{
			case	_INTEGER_VALUE	:
				if (atoi(wptr->value) == atoi(vptr->value))
				{
					iptr->context->ip = lptr->code;
					drop_value( vptr );
					return(iptr->context->ip);
				}
				else	break;
			case	_STRING_VALUE	:
				if (!( strcmp( wptr->value, vptr->value ) ))
				{

					iptr->context->ip = lptr->code;
					drop_value( vptr );
					return(iptr->context->ip);
				}
				else	break;
			}
		}
	}
	iptr->context->ip = iptr->next;
	drop_value( vptr );
	return(iptr->context->ip);
}

/*	-------------------------	*/
/*		jne_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * jne_operation( struct cordscript_instruction * iptr )
{
	struct cordscript_operand * optr;
	struct cordscript_value * vptr;
	struct cordscript_value * lptr;
	struct cordscript_value * wptr;

	if ( check_debug() )
		printf("jne_operation();\n");

	/* ----------------------------- */
	/* collect and validate operands */
	/* ----------------------------- */
	if (((vptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
	&&  ((optr = iptr->first) != (struct cordscript_operand *) 0 ) 
	&&  ((wptr = optr->value) != (struct cordscript_value *) 0 ) 
	&&  ((optr = optr->next ) != (struct cordscript_operand *) 0 ) 
	&&  ((lptr = optr->value) != (struct cordscript_value *) 0 ) 
	&&  ( lptr->code ))
 	{
		/* ---------------------------------------- */
		/* test for true condition to take the jump */
		/* ---------------------------------------- */
		if (((!( wptr->value )) && ( vptr->value ))
		||  ((!( vptr->value )) && ( wptr->value )))
		{
			iptr->context->ip = lptr->code;
			drop_value( vptr );
			return(iptr->context->ip);
		}
		else if (( wptr->value ) && ( vptr->value ))
		{
			switch( check_value_type( vptr->value ) )
			{
			case	_INTEGER_VALUE	:
				if (atoi(wptr->value) != atoi(vptr->value))
				{
					iptr->context->ip = lptr->code;
					drop_value( vptr );
					return(iptr->context->ip);
				}
				else	break;
			case	_STRING_VALUE	:
				if ( strcmp( wptr->value, vptr->value ) != 0 )
				{

					iptr->context->ip = lptr->code;
					drop_value( vptr );
					return(iptr->context->ip);
				}
				else	break;

			}
		}
	}
	iptr->context->ip = iptr->next;
	drop_value( vptr );
	return(iptr->context->ip);
}

/*	-------------------------	*/
/*		jle_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * jle_operation( struct cordscript_instruction * iptr )
{
	struct cordscript_operand * optr;
	struct cordscript_value * vptr;
	struct cordscript_value * lptr;
	struct cordscript_value * wptr;

	if ( check_debug() )
		printf("jle_operation();\n");

	/* ----------------------------- */
	/* collect and validate operands */
	/* ----------------------------- */
	if (((vptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
	&&  ((optr = iptr->first) != (struct cordscript_operand *) 0 ) 
	&&  ((wptr = optr->value) != (struct cordscript_value *) 0 ) 
	&&  ((optr = optr->next ) != (struct cordscript_operand *) 0 ) 
	&&  ((lptr = optr->value) != (struct cordscript_value *) 0 ) 
	&&  ( lptr->code ))
 	{
		if  (((!( vptr->value )) &&   ( wptr->value  ))
		||   ((!( vptr->value )) && (!( wptr->value ))))
		{
			iptr->context->ip = lptr->code;
			drop_value( vptr );
			return(iptr->context->ip);
		}
		else if (( wptr->value ) && ( vptr->value ))
		{
			switch( check_value_type( vptr->value ) )
			{
			case	_INTEGER_VALUE	:
				if (atoi(vptr->value) <= atoi(wptr->value))
				{
					iptr->context->ip = lptr->code;
					drop_value( vptr );
					return(iptr->context->ip);
				}
				else	break;
			case	_STRING_VALUE	:
				if ( strcmp( vptr->value, wptr->value ) <= 0 )
				{

					iptr->context->ip = lptr->code;
					drop_value( vptr );
					return(iptr->context->ip);
				}
				else	break;
			}

		}
	}
	iptr->context->ip = iptr->next;
	drop_value( vptr );
	return(iptr->context->ip);
}

/*	-------------------------	*/
/*		jls_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * jls_operation( struct cordscript_instruction * iptr )
{
	struct cordscript_operand * optr;
	struct cordscript_value * vptr;
	struct cordscript_value * lptr;
	struct cordscript_value * wptr;

	if ( check_debug() )
		printf("jls_operation();\n");

	/* ----------------------------- */
	/* collect and validate operands */
	/* ----------------------------- */
	if (((vptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
	&&  ((optr = iptr->first) != (struct cordscript_operand *) 0 ) 
	&&  ((wptr = optr->value) != (struct cordscript_value *) 0 ) 
	&&  ((optr = optr->next ) != (struct cordscript_operand *) 0 ) 
	&&  ((lptr = optr->value) != (struct cordscript_value *) 0 ) 
	&&  ( lptr->code ))
 	{
		if  ((!( vptr->value )) &&   ( wptr->value  ))
		{
			iptr->context->ip = lptr->code;
			drop_value( vptr );
			return(iptr->context->ip);
		}
		else if (( wptr->value ) && ( vptr->value ))
		{
			switch( check_value_type( vptr->value ) )
			{
			case	_INTEGER_VALUE	:
				if (atoi(vptr->value) < atoi(wptr->value))
				{
					iptr->context->ip = lptr->code;
					drop_value( vptr );
					return(iptr->context->ip);
				}
				else	break;
			case	_STRING_VALUE	:
				if ( strcmp( vptr->value, wptr->value ) < 0 )
				{

					iptr->context->ip = lptr->code;
					drop_value( vptr );
					return(iptr->context->ip);
				}
				else	break;
			}
		}
	}
	iptr->context->ip = iptr->next;
	drop_value( vptr );
	return(iptr->context->ip);
}

/*	-------------------------	*/
/*		jgr_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * jgr_operation( struct cordscript_instruction * iptr )
{
	struct cordscript_operand * optr;
	struct cordscript_value * vptr;
	struct cordscript_value * lptr;
	struct cordscript_value * wptr;

	if ( check_debug() )
		printf("jgr_operation();\n");

	/* ----------------------------- */
	/* collect and validate operands */
	/* ----------------------------- */
	if (((vptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
	&&  ((optr = iptr->first) != (struct cordscript_operand *) 0 ) 
	&&  ((wptr = optr->value) != (struct cordscript_value *) 0 ) 
	&&  ((optr = optr->next ) != (struct cordscript_operand *) 0 ) 
	&&  ((lptr = optr->value) != (struct cordscript_value *) 0 ) 
	&&  ( lptr->code ))
 	{
		if  ((!( wptr->value )) &&   ( vptr->value  ))
		{
			iptr->context->ip = lptr->code;
			drop_value( vptr );
			return(iptr->context->ip);
		}
		else if (( wptr->value ) && ( vptr->value ))
		{
			switch( check_value_type( vptr->value ) )
			{
			case	_INTEGER_VALUE	:
				if (atoi(vptr->value) > atoi(wptr->value))
				{
					iptr->context->ip = lptr->code;
					drop_value( vptr );
					return(iptr->context->ip);
				}
				else	break;
			case	_STRING_VALUE	:
				if ( strcmp( vptr->value, wptr->value ) > 0 )
				{

					iptr->context->ip = lptr->code;
					drop_value( vptr );
					return(iptr->context->ip);
				}
				else	break;
			}
		}
	}
	iptr->context->ip = iptr->next;
	drop_value( vptr );
	return(iptr->context->ip);
}

/*	-------------------------	*/
/*		jge_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * jge_operation( struct cordscript_instruction * iptr )
{
	struct cordscript_operand * optr;
	struct cordscript_value * vptr;
	struct cordscript_value * lptr;
	struct cordscript_value * wptr;

	if ( check_debug() )
		printf("jge_operation();\n");

	/* ----------------------------- */
	/* collect and validate operands */
	/* ----------------------------- */
	if (((vptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
	&&  ((optr = iptr->first) != (struct cordscript_operand *) 0 ) 
	&&  ((wptr = optr->value) != (struct cordscript_value *) 0 ) 
	&&  ((optr = optr->next ) != (struct cordscript_operand *) 0 ) 
	&&  ((lptr = optr->value) != (struct cordscript_value *) 0 ) 
	&&  ( lptr->code ))
 	{
		if  (((!( wptr->value )) &&   ( vptr->value  ))
		||   ((!( vptr->value )) && (!( wptr->value ))))
		{
			iptr->context->ip = lptr->code;
			drop_value( vptr );
			return(iptr->context->ip);
		}
		else if (( wptr->value ) && ( vptr->value ))
		{
			switch( check_value_type( vptr->value ) )
			{
			case	_INTEGER_VALUE	:
				if (atoi(vptr->value) >= atoi(wptr->value))
				{
					iptr->context->ip = lptr->code;
					drop_value( vptr );
					return(iptr->context->ip);
				}
				else	break;

			case	_STRING_VALUE	:
				if ( strcmp( vptr->value, wptr->value ) >= 0 )
				{

					iptr->context->ip = lptr->code;
					drop_value( vptr );
					return(iptr->context->ip);
				}
				else	break;
			}
		}
	}
	iptr->context->ip = iptr->next;
	drop_value( vptr );
	return(iptr->context->ip);
}

/*   ---------------- */
/*   push_instruction */
/*   ---------------- */
private	void	push_instruction( struct cordscript_context * cptr, struct cordscript_instruction * iptr )
{
	struct	cordscript_value * vptr;
	if (!( vptr = instruction_value( iptr ) ))
		return;
	else
	{
		vptr->next = cptr->stack;
		cptr->stack = vptr;
		return;
	}
}

/*   ---------------- */
/*   pop_instruction  */
/*   ---------------- */
private	void	pop_instruction( struct cordscript_context * cptr )
{
	struct	cordscript_value * vptr;
	if (!( vptr = cptr->stack ))
		return;
	else 
	{
		cptr->stack = vptr->next;
		if ( vptr->code )
			cptr->ip = vptr->code;
		vptr = liberate_cordscript_value( vptr );
		return;
	}
}

/*	-------------------	*/
/*	crop_matrix_element	*/
/*	-------------------	*/
private	int	crop_matrix_element( struct cordscript_value * rptr, struct cordscript_value * sptr )
{
	int	c;
	char *	mptr;
	char *	vptr;

	/* check valid parameters or fail */
	/* ------------------------------ */
	if (!( rptr )) 
		return(0);
	else if (!( sptr ))
		return(0);
	else if (!(mptr = sptr->value))
		return(0);

	/* detect source matrix or fail */
	/* ---------------------------- */
	while ( *mptr == ' ' ) mptr++;

	if ( *(mptr++) != '[' ) return(0);

	while ( *mptr == ' ' ) mptr++;

	for ( vptr=mptr; *vptr != 0; vptr++ )
	{
		if (( *vptr == ']' )
		||  ( *vptr == ',' ))
			break;
	}
	c = *vptr;
	*vptr = 0;

	if ( rptr->value )
		liberate( rptr->value );

	if (!( rptr->value = allocate_string( mptr ) ))
		return(0);
	else if (!( rptr->value = occi_unquoted_value( rptr->value ) ))
		return( 0 );
	
	if ( c == ']' )
	{
		sptr->value = liberate( sptr->value );
		return(1);
	}
	else if (!( mptr = allocate( strlen( (vptr+1) ) + 4 ) ))
	{
		sptr->value = liberate( sptr->value );
		return(1);
	}
	else
	{
		sprintf(mptr, "[%s",(vptr+1));
		sptr->value = liberate( sptr->value );
		sptr->value = mptr;
		return(1);
	}
}

/*	----------------------	*/
/*	crop_structure_element	*/
/*	----------------------	*/
private	int	crop_structure_element( struct cordscript_value * nptr, struct cordscript_value * rptr, struct cordscript_value * sptr )
{
	int	c;
	char *	mptr;
	char *	vptr;
	char *	wptr;

	/* check valid parameters or fail */
	/* ------------------------------ */
	if (!( rptr )) 
		return(0);
	else if (!( sptr ))
		return(0);
	else if (!(mptr = sptr->value))
		return(0);

	/* detect source matrix or fail */
	/* ---------------------------- */
	while ( *mptr == ' ' ) mptr++;

	if ( *(mptr++) != '{' ) return(0);

	while ( *mptr == ' ' ) mptr++;

	for ( vptr=mptr; *vptr != 0; vptr++ )
	{
		if (( *vptr == '}' )
		||  ( *vptr == ',' ))
			break;
	}
	c = *vptr;
	*vptr = 0;

	if ( nptr->value )
		liberate( nptr->value );

	if ( rptr->value )
		liberate( rptr->value );

	for  ( wptr = mptr; *wptr != 0; wptr++ )
		if ( *wptr == ':' )
			break;

	if ( *wptr == ':' )
		*(wptr++)=0;


	if (!( nptr->value = allocate_string( mptr ) ))
		return(0);
	else if (!( nptr->value = occi_unquoted_value( nptr->value ) ))
		return( 0 );
	
	if (!( rptr->value = allocate_string( wptr ) ))
		return(0);
	else if (!( rptr->value = occi_unquoted_value( rptr->value ) ))
		return( 0 );
	
	if ( c == '}' )
	{
		sptr->value = liberate( sptr->value );
		return(1);
	}
	else if (!( mptr = allocate( strlen( (vptr+1) ) + 4 ) ))
	{
		sptr->value = liberate( sptr->value );
		return(1);
	}
	else
	{
		sprintf(mptr, "{%s",(vptr+1));
		sptr->value = liberate( sptr->value );
		sptr->value = mptr;
		return(1);
	}
}

/*	-------------------------	*/
/*		each_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * each_operation( struct cordscript_instruction * iptr )
{
	struct cordscript_operand * optr;
	struct cordscript_value * vptr;
	struct cordscript_value * tptr;
	struct cordscript_value * sptr;
	
	if ( check_debug() )
		printf("each_operation();\n");
	
	/* ------------------------------------------------- */
	/* collect source and target operands from the stack */
	/* ------------------------------------------------- */
	if (!(optr = iptr->first))
		return( iptr->next );
	else if (!(sptr = optr->value))
		return( iptr->next );
	else if (!(optr = optr->next))
		return( iptr->next );
	else if (!(vptr = optr->value))
		return( iptr->next );
	else if ( crop_matrix_element( vptr, sptr ) )
		return( iptr->next );
	else if (!(optr = optr->next))
		return( iptr->next );
	else if (!(vptr = optr->value))
		return( iptr->next );
	else if (!( vptr->code ))
		return( iptr->next );
	else	return( vptr->code );
}

/*	-------------------------	*/
/*		both_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * both_operation( struct cordscript_instruction * iptr )
{
	struct cordscript_operand * optr;
	struct cordscript_value * nptr;
	struct cordscript_value * vptr;
	struct cordscript_value * tptr;
	struct cordscript_value * sptr;
	
	if ( check_debug() )
		printf("both_operation();\n");
	
	/* ------------------------------------------------- */
	/* collect source and target operands from the stack */
	/* ------------------------------------------------- */
	if (!(optr = iptr->first))
		return( iptr->next );
	else if (!(sptr = optr->value))
		return( iptr->next );
	else if (!(optr = optr->next))
		return( iptr->next );
	else if (!(nptr = optr->value))
		return( iptr->next );
	else if (!(optr = optr->next))
		return( iptr->next );
	else if (!(vptr = optr->value))
		return( iptr->next );
	else if ( crop_structure_element( nptr, vptr, sptr ) )
		return( iptr->next );
	else if (!(optr = optr->next))
		return( iptr->next );
	else if (!(vptr = optr->value))
		return( iptr->next );
	else if (!( vptr->code ))
		return( iptr->next );
	else	return( vptr->code );
}

/*	-------------------------	*/
/*		call_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * call_operation( struct cordscript_instruction * iptr )
{
	struct cordscript_operand * optr;
	struct cordscript_value   * vptr;
	struct cordscript_value   * nptr;
	struct cordscript_context * bptr;
	struct cordscript_value   * pptr;
	struct cordscript_value   * qptr;
	int	parameters=0;

	if ( check_debug() )
		printf("call_operation();\n");
	if (( iptr ) 
	&&  ((optr = iptr->first) != (struct cordscript_operand *) 0 ) 
	&&  ((vptr = optr->value) != (struct cordscript_value *) 0 ) 
	&&  ((bptr = vptr->body ) != (struct cordscript_context *) 0))
	{
		/* --------------------------------------------- */
		/* detect and handle invocation parameter values */
		/* --------------------------------------------- */
		if (((optr = optr->next) != (struct cordscript_operand *) 0)
		&&  ((nptr = optr->value) != (struct cordscript_value *) 0)
		&&  ((parameters = (nptr->value ? atoi(nptr->value) : 0 )) != 0))
		{
			for (	pptr=bptr->data;
				pptr != (struct cordscript_value *) 0;
				pptr = pptr->next )
			{
				if ( pptr->parameter == parameters )
				{
					if (( qptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
					{
						if ( pptr->value )
							pptr->value = liberate( pptr->value );
						if ( qptr->value )
							pptr->value = allocate_string( qptr->value );
						drop_value( qptr );
					}
					parameters--;
				}
			}
		}
		bptr->caller = iptr->next;
		return((bptr->ip = bptr->cs));
	}
	else	return((struct cordscript_instruction *) 0);
}

/*	-------------------------	*/
/*		ret_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * ret_operation( struct cordscript_instruction * iptr )
{
	struct cordscript_context * cptr;
	if ( check_debug() )
		printf("ret_operation();\n");
	if (!( iptr ))
		return((struct cordscript_instruction *) 0);
	else if (!( cptr = iptr->context ))
		return((struct cordscript_instruction *) 0);
	else if (!( iptr = cptr->caller ))
		return((struct cordscript_instruction *) 0);
	else	return( iptr );
}

/*	-------------------------	*/
/*	   retvalue_operation		*/
/*	-------------------------	*/
private	struct	cordscript_instruction * retvalue_operation( struct cordscript_instruction * iptr )
{
	struct 	cordscript_context * cptr;
	struct	cordscript_value * vptr;
	if ( check_debug() )
		printf("retvalue_operation();\n");
	if (!( iptr ))
		return((struct cordscript_instruction *) 0);
	else if (!( vptr = pop_stack( iptr->context ) ))
		return((struct cordscript_instruction *) 0);
	else if (!( cptr = iptr->context ))
		return((struct cordscript_instruction *) 0);
	else if (!( iptr = cptr->caller ))
		return((struct cordscript_instruction *) 0);
	else
	{
		push_value( iptr->context, vptr );
		return(iptr);
	}
}


/*	-------------------------	*/
/*	allocate_cordscript_value	*/
/*	-------------------------	*/
public	struct	cordscript_value * duplicate_cordscript_value( struct cordscript_value * vptr )
{
	if (!( vptr ))
		return( vptr );
	else 	return( allocate_cordscript_value( vptr->value, vptr->name ) );
}

/*	-------------------------------		*/
/*	allocate_cordscript_instruction		*/
/*	-------------------------------		*/
public struct cordscript_instruction	* allocate_cordscript_instruction( struct cordscript_instruction * (*fptr)(struct cordscript_instruction * self) )
{
	struct	cordscript_instruction * iptr;
	if (!( iptr = allocate( sizeof( struct cordscript_instruction ) ) ))
		return( iptr );
	else
	{
		memset( iptr, 0, sizeof( struct cordscript_instruction ) );
		iptr->evaluate = fptr;
		return( iptr );
	}
}

/*	---------------------------	*/
/*	allocate_cordscript_context	*/
/*	---------------------------	*/
public struct cordscript_context * allocate_cordscript_context()
{
	struct	cordscript_context * cptr;
	if (!( cptr = allocate( sizeof( struct cordscript_context ) ) ))
		return( cptr );
	else
	{
		memset( cptr, 0, sizeof( struct cordscript_context ) );
		return( cptr );
	}
}

/*   	---------------		*/
/*	initialise_line		*/
/*   	----------------	*/
private	char *  linebuffer=(char *) 0;
private	int	linecounter=0;
private	int	ungotc=0;
private	char * 	ungot_token=(char *) 0;

private	int	initialise_line( char * sptr )
{
	linebuffer = sptr;
	linecounter++;
	return( strlen( linebuffer ) ); 		
}

/*   	---------------		*/
/*	initialise_file		*/
/*   	----------------	*/
private	FILE * 	infile=(FILE *) 0;
private	FILE *	initialise_file( FILE * h )
{
	FILE * hh=infile;
	infile = h;
	return( hh );
}

/*	-----------	*/
/*	ungot_token	*/
/*	-----------	*/
private	void	unget_token( char * sptr )
{
	ungot_token = allocate_string( sptr );
	return;
}

/*	------------	*/
/*	 unget_byte	*/
/*	------------	*/
private	int	unget_byte(int c)
{
	ungotc = ((ungotc << 8) | ( c & 0x00FF));
	return(0);
}

/*	------------	*/
/*	  get_byte	*/
/*	------------	*/
private	int	get_byte()
{
	int	c;
	if ((c = (ungotc & 0x00FF)) != 0 )
		ungotc >>= 8;
	else if ( linebuffer )
	{
		if (!( c = *(linebuffer++) ))
			linebuffer--;
	}
	else if ( infile )
		if ((c = fgetc(infile)) == -1)
			c = 0;

	if ( echo )
		if ( c )
			printf("%c",c);	
	return( c );
}

/*	------------	*/
/*	remove_line	*/
/*	------------	*/
private	int	remove_line()
{
	int	c;
	while ((c = get_byte()) != 0)
		if ( c == '\n' )
			break;
	return(c);
}

/*	--------------	*/
/*	remove_comment	*/
/*	--------------	*/
private	int	remove_comment()
{
	int	c;
	while ((c = get_byte()) != 0)
	{
		if ( c != '*' )
			continue;
		else if (( c = get_byte()) == '/' )
			break;
		else if (!( c ))
			break;
		else	continue;
	}
	return(c);
}

/*	------------	*/
/*	remove_white	*/
/*	------------	*/
private	int	remove_white()
{
	int	c;

	if ( ungot_token )
		return( 1 );
	else
	{
		while (1) 
		{
			if (!(c = get_byte()))
				break;
			else if ( c== '\r' )
				continue;
			else if ( c == '\n' )
				continue;
			else if ( c == '\t' )
				continue;
			else if ( c == ' ' )
				continue;
			else if ( c == '/' )
			{
				if (( c = get_byte()) == '/' )
				{
					if (!( remove_line() ))
						return(0);
					else	continue;
				}
				else if ( c == '*' )
				{
					if (!( remove_comment() ))
						return(0);
					else	continue;
				}
				else
				{
					unget_byte( c ); 
					unget_byte('/');
					return( 1 );
				}
			}
			else
			{
				unget_byte(c);
				return( 1 );
			}
		}
		return(0);
	}
}

/*	---------	*/
/*	get_array 	*/
/*	---------	*/
private	int	get_array( char * bptr )
{
	int	nb=0;
	int	braces=0;
	int	c;
	*(bptr+nb) = '['; nb++;
	while ((c = get_byte()) != 0)
	{
		/* ------------------------- */
		/* white space normalisation */
		/* ------------------------- */
		if ( c == '\t' ) 
			c = ' ';
		else if ( c == '\r' )
			continue;
		else if ( c == '\n' )
			continue;

		if ( c == '[' )
			braces++;

		else if ( c == ']' )
		{
			if ( braces )
				braces--;
			else	break;
		}

		*(bptr+nb) = c;
		nb++;
	}
	*(bptr+nb) = ']';
	nb++;
	*(bptr+nb) = 0;
	return( nb );
}

/*	-------------	*/
/*	get_structure 	*/
/*	-------------	*/
private	int	get_structure( char * bptr )
{
	int	nb=0;
	int	braces=0;
	int	c;
	*(bptr+nb) = '{'; nb++;
	while ((c = get_byte()) != 0)
	{
		/* ------------------------- */
		/* white space normalisation */
		/* ------------------------- */
		if ( c == '\t' ) 
			c = ' ';
		else if ( c == '\r' )
			continue;
		else if ( c == '\n' )
			continue;

		if ( c == '{' )
			braces++;
		else if ( c == '}' )
		{
			if ( braces )
				braces--;
			else	break;
		}
		*(bptr+nb) = c;
		nb++;
	}
	*(bptr+nb) = '}';
	nb++;
	*(bptr+nb) = 0;
	return( nb );
}

/*   ---------------- */
/*  token_legal   */
/*   ---------------- */
private	int	token_legal( int c )
{
	if (( c >= '0' ) && ( c <= '9' ))
		return(1);
	else if (( c >= 'a' ) && ( c <= 'z' ))
		return(1);
	else if (( c >= 'A' ) && ( c <= 'Z' ))
		return(1);
	else if ( c == '$' )
		return( 1 );
	else if ( c == '_' )
		return( 1 );
	else if ( c == '"' )
		return( 1 );
	else if ( c == 0x0027 )
		return( 1 );
	else	return( 0 );
}

/*   ---------	*/
/*   get_token	*/
/*   ---------	*/
private	int	get_token( char * result )
{
	int	quote=0;
	int	c;
	int	cc;
	int	nb=0;

	if ( ungot_token )
	{
		strcpy( result, ungot_token );
		liberate( ungot_token );
		ungot_token = (char *) 0;
		return( strlen( result ) );
	}

	if (!( remove_white() ))
		return( 0 );
	while (1)
	{
		if (!( c = get_byte()))
			break;
		else if ( quote )
		{
			if ( c == quote )
				break;
			{
				*(result+nb) = c;
				nb++;
			}
		}
		else if ((!( nb )) && ( c == '[' ))
			return( get_array( result ) );
		else if ((!( nb )) && ( c == '{' ))
			return( get_structure( result ) );
		else if (( c == '"' ) || ( c == 0x0027 ))
		{
			quote = c;
			continue;
		}
		else if (((!( nb )) && ( c == '+' ))
		     || ((!( nb )) && ( c == '-' )))
		{
			if (((c = get_byte()) >= '0' )
			&&  (c <= '9'))
			{
				if ( c == '-' )
				{
					*(result+nb) = c; nb++;
				}
				*(result+nb) = cc; nb++;
				continue;
			}
			else	
			{
				unget_byte( cc );
				unget_byte( c );
				break;
			}
		}
		else if (!( token_legal(c) ))
		{
			unget_byte(c);
			break;
		}
		else
		{
			*(result+nb) = c; nb++;
		}
	}
	*(result+nb) = 0;
	return(nb);
}

/*   ---------------- */
/*   resolve_variable */
/*   ---------------- */
private	struct	cordscript_value *	resolve_variable( char * token, struct cordscript_context * cptr )
{
	struct	cordscript_value * vptr;
	if ((vptr = cptr->data) != (struct cordscript_value *) 0)
	{
		while ( vptr )
		{
			if (( vptr->name )
			&&  (!( strcmp( token, vptr->name ) )))
				return( vptr );
			else	vptr = vptr->next;
		}
	}
	if (!( vptr = allocate_cordscript_value( "", token ) ))
		return( vptr );
	else
	{
		vptr->next = cptr->data;
		cptr->data = vptr;
		return( vptr );
	}
}
/*   ---------------- */
/*   resolve_function */
/*   ---------------- */
private	struct	cordscript_value *	resolve_function( char * token, struct cordscript_context * cptr )
{
	struct	cordscript_value * vptr;
	if ((vptr = cptr->code) != (struct cordscript_value *) 0)
	{
		while ( vptr )
		{
			if (( vptr->name )
			&&  (!( strcmp( token, vptr->name ) )))
				return( vptr );
			else	vptr = vptr->next;
		}
	}
	return( (struct cordscript_value *) 0 );
}

/*   -------------------	*/
/*	resolve_value  		*/
/*   -------------------	*/
private	struct	cordscript_value * 	resolve_value( char * token, struct cordscript_context * cptr )
{
	struct	cordscript_value * fptr;
	if ( *token == '$' )
		return( resolve_variable( token, cptr ) );
	else	return( allocate_cordscript_value( token , (char *) 0 ) );
}

/*   ---------------- */
/*    get_punctuation */
/*   ---------------- */
private	int	get_punctuation()
{
	int	c;
	if ( ungot_token )
		return( 0 );
	else if (!( remove_white( linebuffer ) ))
		return( 0 );
	else if (!( c = get_byte()))
		return( c );
	else if (!( token_legal( c ) ))
		return( c );
	else
	{
		unget_byte(c);
		return( 0 );
	}
}

/*   ---------------*/
/*   add_instruction*/
/*   ---------------*/
private	void	add_instruction( struct cordscript_context * cptr, struct cordscript_instruction * iptr )
{
	if (!( cptr->cs ))
		cptr->cs = iptr;
	else	cptr->ip->next = iptr;
	cptr->ip = iptr;
	iptr->context = cptr;
	return;
}

private	struct	cordscript_instruction	* compile_failure( int type, char * operand )
{
	abandon_compile=1;
	switch ( type )
	{
	case	1 :	
		printf("\nexpected punctuation : %s\n",operand);
		return((struct cordscript_instruction *) 0);
	case	2 :	
		printf("\nallocation failure : %s\n",operand);
		return((struct cordscript_instruction *) 0);
	case	3 :	
		printf("\nexpected operand : %s\n",operand);
		return((struct cordscript_instruction *) 0);
	default	:
		printf("\nunexpected failure \n");
		return((struct cordscript_instruction *) 0);
	}
}
	
/*   -----------------------	*/
/*   compile_cordscript_call 	*/
/*   -----------------------	*/
private	struct	cordscript_instruction * compile_cordscript_call(struct cordscript_context * cptr, struct cordscript_value * fptr )
{
	struct	cordscript_instruction * iptr;
	struct	cordscript_instruction * jptr;
	int	c;
	int	operands=0;
	if ((c = get_punctuation()) != '(' )
	{
		if ( c ) unget_byte( c );
		return( compile_failure(1,")") );
	}
	else if (!( iptr = allocate_cordscript_instruction( call_operation ) ))
		return( compile_failure(2,"i") );
	else
	{
		while (1)
		{
			if (( c = get_punctuation() ) != 0)
			{
				if ( c == ')' )
					break;
				else if ( c == ',' )
					continue;
				else if ((c == '[' ) || ( c == '{' ))
				{
					unget_byte( c ); 
					if (!( jptr = compile_cordscript_instruction(cptr, 1 ) ))
						break;
					else
					{
						operands++;
						continue;
					}
				}
				else 	break;
			}
			else if (!( jptr = compile_cordscript_instruction(cptr, 1 ) ))
				break;
			else	operands++;
		}
		add_operand( iptr, fptr );
		add_operand( iptr, integer_value( operands ) );
		add_instruction( cptr, iptr );
		return( iptr );
	}
}

/*   ------------------------	*/
/*   compile_cordscript_block	*/
/*   ------------------------	*/
private	struct	cordscript_instruction * compile_cordscript_block(struct cordscript_context * cptr)
{
	int	c;

	while ((c = get_punctuation()) != '}' )
	{
		if ( c )
			unget_byte( c );
			
		compile_cordscript_instruction( cptr, 0 );
	}	   	  	 
	return((struct cordscript_instruction *) 0);
}

/*   ------------	*/
/*   sytnax_error	*/
/*   ------------	*/
private	struct cordscript_instruction * syntax_error( struct cordscript_instruction * iptr )
{
	compile_failure(4,"syntax error");
	return( liberate_cordscript_instruction( iptr ) );
}

/*	------------------	*/
/*	cordscript_compare	*/
/*	------------------	*/
private	int	cordscript_compare()
{
	int	c;
	if (!(c = get_punctuation()))
		return(c);
	else if ( c == '=' )
		return(_CSP_EQ);
	else if ( c == '<' )
	{
		if ((c = get_punctuation()) == '=' )
			return(_CSP_LE );
		else
		{
			if ( c ) 
				unget_byte( c );
			return( _CSP_LS );
		}
	}
	else if ( c == '>' )
	{
		if ((c = get_punctuation()) == '=' )
			return(_CSP_GE );
		else
		{
			if ( c ) 
				unget_byte( c );
			return( _CSP_GR );
		}
	}
	else if ( c == '!' )
	{
		if ((c = get_punctuation()) == '=' )
			return(_CSP_NE );
		else
		{
			if ( c ) 
				unget_byte( c );
			return( 0 );
		}
	}
	else
	{
		unget_byte(c);
		return( 0 );
	}
}

/*   ---------------------	*/
/*   compile_cordscript_if	*/
/*   ---------------------	*/
private	struct cordscript_instruction * compile_cordscript_if( struct cordscript_context * cptr )
{
	int	c;
	struct	cordscript_instruction * lptr;
	struct	cordscript_instruction * rptr;
	struct	cordscript_instruction * iptr;
	int	operator=0;

	if ( get_punctuation() != '(' )
		return( compile_failure(1,"(") );
	else if (!( lptr = compile_cordscript_instruction( cptr, 0 ) ))
		return((struct cordscript_instruction *) 0);

	else if (!( operator = cordscript_compare() ))
		return( compile_failure(4,"operator") );
	else if (!( rptr = compile_cordscript_instruction( cptr, 0 ) ))
		return( compile_failure(3,"i") );

	if ( get_punctuation() != ')' )
		return( compile_failure(1,")") );

	switch ( operator )
	{
	case	_CSP_EQ	:
		rptr->evaluate = jne_operation;
		break;
	case	_CSP_LS	:
		rptr->evaluate = jge_operation;
		break;
	case	_CSP_LE	:
		rptr->evaluate = jgr_operation;
		break;
	case	_CSP_GR	:
		rptr->evaluate = jle_operation;
		break;
	case	_CSP_GE	:
		rptr->evaluate = jls_operation;
		break;
	case	_CSP_NE	:
		rptr->evaluate = jeq_operation;
		break;
	}

	if (!( iptr = allocate_cordscript_instruction( no_operation ) ))
		return( iptr );
	else
	{
		iptr->context = cptr;
		add_operand( rptr, instruction_value( iptr ) );
		if (( c = get_punctuation()) == '{' )
			allocate_cordscript_label( iptr, _IF_LABEL );
		else if (!( c ))
			add_instruction( cptr, iptr );
		else
		{
			unget_byte(c);
			add_instruction( cptr, iptr );
		}
		return((struct cordscript_instruction *) 0);
	}
}

/*   -----------------------	*/
/*   compile_cordscript_else	*/
/*   -----------------------	*/
private	struct cordscript_instruction * compile_cordscript_else( struct cordscript_context * cptr )
{
	return((struct cordscript_instruction *) 0);
}

/*   -----------------------	*/
/*   compile_cordscript_else	*/
/*   -----------------------	*/
private	struct cordscript_instruction * compile_cordscript_function( struct cordscript_context * kptr )
{
	struct	cordscript_value * vptr;
	struct	cordscript_context * cptr;
	struct	cordscript_value * fptr;
	char	buffer[_MAX_NAME];
	int	parameters=0;
	int	c;
	if (!( get_token( buffer ) ))
		return((struct cordscript_instruction *) 0);
	else if (!( fptr = allocate_cordscript_value( (char *) 0, buffer ) ))
		return( compile_failure(2,"v") );
	else if ((c = get_punctuation()) != '(' )
		return( compile_failure(1,"(") );
	else if (!( cptr = allocate_cordscript_context() ))
		return( compile_failure(2,"") );
	{
		while (1)
		{

			if ( get_token( buffer ) != 0)
			{
				if (!( vptr = allocate_cordscript_value( (char *) 0, buffer ) ))
				{
					liberate_cordscript_context( cptr );
					return( compile_failure(2,"v") );
				}
				else
				{
					vptr->parameter = ++parameters;
					vptr->next = cptr->data;
					cptr->data = vptr;
				}
			}
			if ((c = get_punctuation()) == ')' )
				break;
			else if ( c == ',' )
				continue;
			else			
			{
				liberate_cordscript_context( cptr );
				return( compile_failure(1,",") );
			}
		}		
		fptr->next = kptr->code;
		kptr->code = fptr;
		fptr->body = cptr;
		if (( c = get_punctuation()) != '{' )
		{
			liberate_cordscript_context( cptr );
			return( compile_failure(1,"{") );
		}
		else
		{
			compile_cordscript_block( cptr );
			return((struct cordscript_instruction *) 0);
		}
	}
}

/*   --------------------------		*/
/*   compile_cordscript_foreach		*/
/*   --------------------------		*/
private	struct cordscript_instruction * compile_cordscript_foreach( struct cordscript_context * cptr )
{
	int	c;
	struct	cordscript_instruction * lptr;
	struct	cordscript_instruction * jptr;
	struct	cordscript_instruction * xptr;
	struct	cordscript_instruction * iptr;
	struct	cordscript_value *	 sptr;
	struct	cordscript_value *	 tptr;
	char	buffer[_MAX_NAME];

	if ( get_punctuation() != '(' )
		return((struct cordscript_instruction *) 0);

	else if (!( lptr = allocate_cordscript_instruction( no_operation ) ))
		return( lptr );
	else	add_instruction(cptr, lptr );

	if (!( jptr = allocate_cordscript_instruction( jmp_operation ) ))
		return( xptr );
	else	add_operand( jptr, instruction_value( lptr ) );

	if (!( xptr = allocate_cordscript_instruction( no_operation ) ))
		return( xptr );
	else	jptr->next = xptr;

	if (!( get_token(buffer)))
		return((struct cordscript_instruction *) 0);

	else if (!( sptr = resolve_variable( buffer, cptr ) ))
		return((struct cordscript_instruction *) 0);

	else if (!( get_token(buffer)))
		return((struct cordscript_instruction *) 0);

	else if ( strcmp( buffer,"as" ) )
		return((struct cordscript_instruction *) 0);

	else if (!( get_token(buffer)))
		return((struct cordscript_instruction *) 0);

	else if (!( tptr = resolve_variable( buffer, cptr ) ))
		return((struct cordscript_instruction *) 0);

	else if ( get_punctuation() != ')' )
		return((struct cordscript_instruction *) 0);

	if (!( iptr = allocate_cordscript_instruction( each_operation ) ))
		return( iptr );
	else
	{
		add_operand( iptr, sptr );
		add_operand( iptr, tptr );
		add_operand( iptr, instruction_value( xptr ) );
		add_instruction( cptr, iptr );
	}

	/* ----------------------------------- */
	/* detect and handle conditional block */
	/* ----------------------------------- */
	if (( c = get_punctuation()) == '{' )
	{
		allocate_cordscript_label( jptr, _FOREACH_LABEL );
		return((struct cordscript_instruction *) 0);
	}
	else	return( compile_failure(1,"{") );

}


/*   --------------------------		*/
/*   compile_cordscript_forboth		*/
/*   --------------------------		*/
private	struct cordscript_instruction * compile_cordscript_forboth( struct cordscript_context * cptr )
{
	int	c;
	struct	cordscript_instruction * lptr;
	struct	cordscript_instruction * jptr;
	struct	cordscript_instruction * xptr;
	struct	cordscript_instruction * iptr;
	struct	cordscript_value *	 sptr;
	struct	cordscript_value *	 tptr;
	struct	cordscript_value *	 vptr;
	char	buffer[_MAX_NAME];

	if ( get_punctuation() != '(' )
		return((struct cordscript_instruction *) 0);

	else if (!( lptr = allocate_cordscript_instruction( no_operation ) ))
		return( lptr );
	else	add_instruction(cptr, lptr );

	if (!( jptr = allocate_cordscript_instruction( jmp_operation ) ))
		return( xptr );
	else	add_operand( jptr, instruction_value( lptr ) );

	if (!( xptr = allocate_cordscript_instruction( no_operation ) ))
		return( xptr );
	else	jptr->next = xptr;

	if (!( get_token(buffer)))
		return((struct cordscript_instruction *) 0);

	else if (!( sptr = resolve_variable( buffer, cptr ) ))
		return((struct cordscript_instruction *) 0);

	else if (!( get_token(buffer)))
		return((struct cordscript_instruction *) 0);

	else if ( strcmp( buffer,"as" ) )
		return((struct cordscript_instruction *) 0);

	else if (!( get_token(buffer)))
		return((struct cordscript_instruction *) 0);

	else if (!( tptr = resolve_variable( buffer, cptr ) ))
		return((struct cordscript_instruction *) 0);

	else if (!( get_token(buffer)))
		return((struct cordscript_instruction *) 0);

	else if ( strcmp( buffer,"and" ) )
		return((struct cordscript_instruction *) 0);

	else if (!( get_token(buffer)))
		return((struct cordscript_instruction *) 0);

	else if (!( vptr = resolve_variable( buffer, cptr ) ))
		return((struct cordscript_instruction *) 0);

	else if ( get_punctuation() != ')' )
		return((struct cordscript_instruction *) 0);

	if (!( iptr = allocate_cordscript_instruction( both_operation ) ))
		return( iptr );
	else
	{
		add_operand( iptr, sptr );
		add_operand( iptr, tptr );
		add_operand( iptr, vptr );
		add_operand( iptr, instruction_value( xptr ) );
		add_instruction( cptr, iptr );
	}

	/* ----------------------------------- */
	/* detect and handle conditional block */
	/* ----------------------------------- */
	if (( c = get_punctuation()) == '{' )
	{
		allocate_cordscript_label( jptr, _FORBOTH_LABEL );
		return((struct cordscript_instruction *) 0);
	}
	else	return( compile_failure(1,"{") );
}

/*	------------------------	*/
/*	flush_label_instructions 	*/
/*	------------------------	*/
private	void	flush_label_instructions( struct cordscript_context * cptr, struct cordscript_instruction * xptr )
{
	while ( xptr  )
	{
		add_instruction( cptr, xptr );
		xptr = xptr->next;
	}
	return;
}

/*   ----------------------	*/
/*   compile_cordscript_for	*/
/*   ----------------------	*/
private	struct cordscript_instruction * compile_cordscript_for( struct cordscript_context * cptr )
{
	struct	cordscript_instruction * iptr;
	struct	cordscript_instruction * lptr;
	struct	cordscript_instruction * rptr;
	struct	cordscript_instruction * bptr;
	struct	cordscript_instruction * xptr;
	struct	cordscript_instruction * jptr;
	int	c;
	int	operator;

	/* --------------------- */
	/* for instruction entry */
	/* --------------------- */
	if ( get_punctuation() != '(' )
		return((struct cordscript_instruction *) 0);

	/* ------------------------- */
	/* initialisation expression */
	/* ------------------------- */
	else if (!( iptr = compile_cordscript_instruction( cptr, 0 ) ))
		return((struct cordscript_instruction *) 0);

	/* --------------------------- */
	/* iteration label instruction */
	/* --------------------------- */
	else if (!( xptr = allocate_cordscript_instruction( no_operation ) ))
		return( xptr );
	else	add_instruction(cptr, xptr );
	
	/* ------------------------------------ */
	/* comparison lvalue operand and rvalue */
	/* ------------------------------------ */
	if (!( iptr = compile_cordscript_instruction( cptr, 0 ) ))
		return((struct cordscript_instruction *) 0);
	else if (!( operator = cordscript_compare() ))
		return((struct cordscript_instruction *) 0);
	else if (!( rptr = compile_cordscript_instruction( cptr, 0 ) ))
		return((struct cordscript_instruction *) 0);
	else
	{
		switch ( operator )
		{
		case	_CSP_EQ	:
			rptr->evaluate = jne_operation;
			break;
		case	_CSP_LS	:
			rptr->evaluate = jge_operation;
			break;
		case	_CSP_LE	:
			rptr->evaluate = jgr_operation;
			break;
		case	_CSP_GR	:
			rptr->evaluate = jle_operation;
			break;
		case	_CSP_GE	:
			rptr->evaluate = jls_operation;
			break;
		case	_CSP_NE	:
			rptr->evaluate = jeq_operation;
			break;
		}
	}

	/* -------------------------- */
	/* jump over iterator to body */
	/* -------------------------- */
	if (!( jptr = allocate_cordscript_instruction( jmp_operation ) ))
		return( jptr );
	else	add_instruction(cptr, jptr );

	/* --------------------------- */
	/* iteration label instruction */
	/* --------------------------- */
	if (!( bptr = allocate_cordscript_instruction( no_operation ) ))
		return( bptr );
	else	add_instruction(cptr, bptr );
	
	/* ---------------------- */
	/* iteration instructions */
	/* ---------------------- */
	if (!( iptr = compile_cordscript_instruction( cptr, 0 ) ))
		return((struct cordscript_instruction *) 0);

	/* -------------------------- */
	/* jump over iterator to body */
	/* -------------------------- */
	if (!( iptr = allocate_cordscript_instruction( jmp_operation ) ))
		return( iptr );
	else
	{
		/* --------------------------------- */
		/* jump back to label before compare */
		/* --------------------------------- */
		add_operand( iptr, instruction_value( xptr ) );
		add_instruction(cptr, iptr );
	}

	/* ------------------------------------------ */
	/* entry point to conditional statement block */
	/* ------------------------------------------ */
	if (!( 	iptr = allocate_cordscript_instruction( no_operation ) ))
		return( iptr );
	else
	{
		add_instruction(cptr, iptr );
		/* ---------------------------------------- */
		/* connect jump after compare to this label */
		/* ---------------------------------------- */
		add_operand( jptr, instruction_value( iptr ) );
	}
	
	/* --------------------------- */
	/* close braces and enter body */
	/* --------------------------- */
	if ( get_punctuation() != ')' )
		return((struct cordscript_instruction *) 0);



	/* ------------------------------------ */
	/* end of conditional instruction block */
	/* ------------------------------------ */
	if (!( jptr = allocate_cordscript_instruction( jmp_operation ) ))
		return( jptr );
	else 	add_operand( jptr, instruction_value( bptr ) );

	/* ---------------- */
	/* final exit label */
	/* ---------------- */
	if (!( 	iptr = allocate_cordscript_instruction( no_operation ) ))
		return( iptr );
	else
	{
		/* ------------------------------- */
		/* link compare true to exit label */
		/* ------------------------------- */
		add_operand( rptr, instruction_value( iptr ) );
		jptr->next = iptr;
	}

	/* ----------------------------------- */
	/* detect and handle conditional block */
	/* ----------------------------------- */
	if (( c = get_punctuation()) == '{' )
	{
		allocate_cordscript_label( jptr, _FOR_LABEL );
		return((struct cordscript_instruction *) 0);
	}
	else	return( compile_failure(1,"{") );
}

/*   ------------------------	*/
/*   compile_cordscript_while	*/
/*   ------------------------	*/
private	struct cordscript_instruction * compile_cordscript_while( struct cordscript_context * cptr )
{
	int	c;
	struct	cordscript_instruction * lptr;
	struct	cordscript_instruction * rptr;
	struct	cordscript_instruction * eptr;
	struct	cordscript_instruction * jptr;
	struct	cordscript_instruction * fptr;
	int	operator=0;

	if (!( eptr = allocate_cordscript_instruction( no_operation ) ))
		return( eptr );
	else	add_instruction(cptr, eptr );
	
	if ( get_punctuation() != '(' )
		return((struct cordscript_instruction *) 0);
	else if (!( lptr = compile_cordscript_instruction( cptr, 0 ) ))
		return((struct cordscript_instruction *) 0);

	if (!( operator = cordscript_compare() ))
		return((struct cordscript_instruction *) 0);
	else if (!( rptr = compile_cordscript_instruction( cptr, 0 ) ))
		return((struct cordscript_instruction *) 0);

	if ( get_punctuation() != ')' )
		return((struct cordscript_instruction *) 0);

	switch ( operator )
	{
	case	_CSP_EQ	:
		rptr->evaluate = jne_operation;
		break;
	case	_CSP_LS	:
		rptr->evaluate = jge_operation;
		break;
	case	_CSP_LE	:
		rptr->evaluate = jgr_operation;
		break;
	case	_CSP_GR	:
		rptr->evaluate = jle_operation;
		break;
	case	_CSP_GE	:
		rptr->evaluate = jls_operation;
		break;
	case	_CSP_NE	:
		rptr->evaluate = jeq_operation;
		break;
	}

	/* --------------------- */
	/* allocate the exit NOP */
	/* --------------------- */
	if (!( fptr = allocate_cordscript_instruction( no_operation ) ))
		return( fptr );

	/* ----------------------- */
	/* allocate the JUMP start */
	/* ----------------------- */
	else if (!( jptr = allocate_cordscript_instruction( jmp_operation ) ))
		return( jptr );
	else
	{
		add_operand( jptr, instruction_value( eptr ) );
		jptr->next = fptr;
		jptr->context = cptr;
		fptr->context = cptr;
		add_operand( rptr, instruction_value( fptr ) );

		if (( c = get_punctuation()) == '{' )
		{
			allocate_cordscript_label( jptr, _WHILE_LABEL );
			return((struct cordscript_instruction *) 0);
		}
		else	return( compile_failure(1,"{") );
	}
}

/*	--------------------	*/
/*	switch_default_fixup	*/
/*	--------------------	*/
private	void	switch_default_fixup( struct cordscript_context * cptr, struct cordscript_label * lptr )
{
	struct	cordscript_instruction * optr;

	flush_label_instructions( cptr, (optr = lptr->other) );
	lptr->other = (struct cordscript_instruction *) 0;

	/* -------------------- */
	/* fixup the DUP as NOP */
	/* -------------------- */
	if ( optr->next )
	{
		optr->next->evaluate = no_operation;
		add_operand( optr, instruction_value( optr->next ) );
	}
	return;
}

/*	--------------		*/
/*	check_line_end		*/
/*	--------------		*/
private	void	check_line_end(struct cordscript_context * cptr, int level, int c)
{
	struct	cordscript_label * lptr;
	struct	cordscript_instruction * xptr;
	char	buffer[_MAX_NAME];
	if ((lptr = labelheap) != (struct cordscript_label *) 0)
	{

		if ( c == ';' )
			end_of_instruction=1;

		if ((!( level ))
		&&  ( end_of_instruction ))
		{
			while ((lptr = labelheap) != (struct cordscript_label *) 0)
			{
				if (( c = get_punctuation()) == '}' )
				{
					if ( lptr->type == _SWITCH_LABEL )
					{
						/* ------------------- */
						/* check for CASE exit */
						/* ------------------- */
						if ( lptr->other )
							switch_default_fixup( cptr, lptr );
					}
					else if ( lptr->type == _TRY_LABEL )
					{
						/* --------------------------- */
						/* it could be a TRY and CATCH */
						/* --------------------------- */
						if ( get_token( buffer ) )
						{
							if (!( strcmp( buffer, "catch" ) ))
							{
								end_of_instruction=0;
								handle_catch( cptr, lptr );
								return;
							}
							else	
							{
								unget_token( buffer );

							}
						}
					}
					else if ( lptr->type == _IF_LABEL )
					{
						/* -------------------------------------- */
						/* it could be an IF and may have an ELSE */
						/* -------------------------------------- */
						if ( get_token( buffer ) )
						{
							if (!( strcmp( buffer, "else" ) ))
							{
								end_of_instruction=0;
								handle_else( cptr, lptr );
								return;
							}
							else	unget_token( buffer );
						}
					}
					labelheap = lptr->next;
					flush_label_instructions( cptr, lptr->value );
					lptr = liberate_cordscript_label( lptr );
				}
				else 
				{	
					if ( c != 0 ) unget_byte( c );	
					break;
				}
			}
			end_of_instruction=0;
		}
	}
	return;
}


/*   ---------------------------	*/
/*   compile_cordscript_continue	*/
/*   ---------------------------	*/
private	struct cordscript_instruction * compile_cordscript_continue( struct cordscript_context * cptr )
{
	struct	cordscript_label       * lptr;
	struct	cordscript_instruction * optr;

	/* ----------------------------------- */
	/* BREAK must be inside a construction */
	/* ----------------------------------- */
	if (!(lptr = labelheap))
		return((struct cordscript_instruction *) 0);

	while ( lptr )
	{
		switch( lptr->type )
		{
		case	_FOR_LABEL	:
		case	_FOREACH_LABEL	:
		case	_FORBOTH_LABEL	:
		case	_WHILE_LABEL	:
			if (!( optr = allocate_cordscript_instruction( jmp_operation ) ))
				break;
			else
			{
			 	add_instruction( cptr, optr );
				add_operand( optr, instruction_value( lptr->value ) );
				break;
			}

		case	_IF_LABEL	:
		case	_SWITCH_LABEL	:
		case	_TRY_LABEL	:
		default			:
			lptr = lptr->next;
			continue;
		}
		break;
	}
	if ( get_punctuation() == ';' )
		check_line_end(cptr,0,';');

	return((struct cordscript_instruction *) 0);
}			

/*   -------------------------	*/
/*   compile_cordscript_break	*/
/*   -------------------------	*/
private	struct cordscript_instruction * compile_cordscript_break( struct cordscript_context * cptr )
{
	struct	cordscript_label       * lptr;
	struct	cordscript_instruction * optr;

	/* ----------------------------------- */
	/* BREAK must be inside a construction */
	/* ----------------------------------- */
	if (!(lptr = labelheap))
		return((struct cordscript_instruction *) 0);
	else if (!( optr = allocate_cordscript_instruction( jmp_operation ) ))
		return( optr );
	else
	{
	 	add_instruction( cptr, optr );
		add_operand( optr, instruction_value( lptr->value ) );
	
		if ( get_punctuation() == ';' )
			check_line_end(cptr,0,';');

		return((struct cordscript_instruction *) 0);
	}
}			

/*   -------------------------	*/
/*   compile_cordscript_default	*/
/*   -------------------------	*/
private	struct cordscript_instruction * compile_cordscript_default( struct cordscript_context * cptr )
{
	struct	cordscript_label       * lptr;
	struct	cordscript_instruction * optr;
	/* ------------------------------------------- */
	/* detect CASE inside legal SWITCH instruction */
	/* ------------------------------------------- */
	if (!(lptr = labelheap))
		return((struct cordscript_instruction *) 0);
	else if ( lptr->type != _SWITCH_LABEL )
		return((struct cordscript_instruction *) 0);

	else if ( get_punctuation() != ':' )
		return((struct cordscript_instruction *) 0);

	/* ------------------------------------------- */
	/* detect a subsequent CASE after initial CASE */
	/* ------------------------------------------- */
	else if ( lptr->other )
	{
		switch_default_fixup( cptr, lptr );
	}

	return((struct cordscript_instruction *) 0);
}			

/*   -------------------------	*/
/*   compile_cordscript_case	*/
/*   -------------------------	*/
private	struct cordscript_instruction * compile_cordscript_case( struct cordscript_context * cptr )
{
	int	c;
	struct	cordscript_label       * lptr;
	struct	cordscript_instruction * rptr;
	struct	cordscript_instruction * fptr;
	struct	cordscript_instruction * optr;
	struct	cordscript_instruction * nptr;

	/* ------------------------------------------- */
	/* detect CASE inside legal SWITCH instruction */
	/* ------------------------------------------- */
	if (!(lptr = labelheap))
		return((struct cordscript_instruction *) 0);
	else if ( lptr->type != _SWITCH_LABEL )
		return((struct cordscript_instruction *) 0);

	/* ------------------------------------------- */
	/* detect a subsequent CASE after initial CASE */
	/* ------------------------------------------- */
	else if ( lptr->other )
	{
		flush_label_instructions( cptr, (optr = lptr->other) );
		lptr->other = (struct cordscript_instruction *) 0;
	}
	else
	{
		/* ----------------------------------------- */
		/* otherwise its the first case expression : */
		/* ----------------------------------------- */
		if (!( fptr = allocate_cordscript_instruction( dup_operation ) ))
			return( fptr );
		else 	add_instruction( cptr, fptr );
	}

	/* compile the conditional expression */
	/* ---------------------------------- */
	if (!( rptr = compile_cordscript_instruction( cptr, 0 ) ))
		return((struct cordscript_instruction *) 0);

	else if ( get_punctuation() != ':' )
		return((struct cordscript_instruction *) 0);

	else 	rptr->evaluate = jne_operation;

	if (!( fptr = allocate_cordscript_instruction( dup_operation ) ))
		return( fptr );
	else
	{	fptr->context = cptr;
		add_operand( rptr, instruction_value( fptr ) );
	}

	/* -------------------------------------------------------------- */
	/* if this is a secondary case then spill over JMP linkage needed */
	/* -------------------------------------------------------------- */
	if ( optr )
	{
		if (!( nptr = allocate_cordscript_instruction( no_operation ) ))
			return( nptr );
		else 	add_instruction( cptr, nptr );

		add_operand( optr, instruction_value( nptr ) );
	}


	/* ------------------------------------ */
	/* add the spill over jmp for next case */
	/* ------------------------------------ */
	if (!( nptr = allocate_cordscript_instruction( jmp_operation ) ))
		return( nptr );
	else
	{
		nptr->next = fptr;
		lptr->other = nptr;
	}

	return((struct cordscript_instruction *) 0);
}			

/*   -------------------------	*/
/*   compile_cordscript_switch	*/
/*   -------------------------	*/
private	struct cordscript_instruction * compile_cordscript_switch( struct cordscript_context * cptr )
{
	int	c;
	struct	cordscript_instruction * lptr;
	struct	cordscript_instruction * fptr;
	char	buffer[_MAX_NAME];

	/* ----------------------- */
	/* switch ( expression ) { */
	/* ----------------------- */
	/* push expression	   */
	/* ----------------------- */
	if ( get_punctuation() != '(' )
		return((struct cordscript_instruction *) 0);
	else if (!( lptr = compile_cordscript_instruction( cptr, 0 ) ))
		return((struct cordscript_instruction *) 0);
	else if ( get_punctuation() != ')' )
		return((struct cordscript_instruction *) 0);

	else if ( get_punctuation() != '{' )
		return((struct cordscript_instruction *) 0);

	/* --------------------- */
	/* allocate the exit POP */
	/* --------------------- */
	if (!( fptr = allocate_cordscript_instruction( pop_operation ) ))
		return( fptr );
	else 	allocate_cordscript_label( fptr, _SWITCH_LABEL );

	if (!( get_token( buffer ) ))
		return((struct cordscript_instruction *) 0);

	else if (!( strcmp( buffer, "case" ) ))
		return( compile_cordscript_case( cptr ) );
	else
	{
		unget_token( buffer );
		return((struct cordscript_instruction *) 0);
	}
}			

/*   --------------------------	*/
/*   compile_cordscript_include */
/*   --------------------------	*/
private	struct cordscript_instruction * compile_cordscript_include( struct cordscript_context * cptr )
{
	struct	cordscript_instruction * iptr;
	FILE 	*	h;
	FILE 	*	hh;
	char	buffer[8192];

	if (!( get_token( buffer ) ))
		return((struct cordscript_instruction *) 0);
	else if (!( h = fopen( buffer, "r" ) ))
		return((struct cordscript_instruction *) 0);
	else
	{
		if ( check_verbose() ) { printf("include file: %s\n",buffer); }

		hh = initialise_file( h );

		while ( remove_white() )
		{
			iptr = compile_cordscript_instruction( cptr, 0 );
			if ( get_punctuation() )
				break;
			else if ( abandon_compile )
				break;
		}
		h = initialise_file( hh );
		fclose(h);
		return((struct cordscript_instruction *) 0);
	}
}			

/*   -------------------------	*/
/*   compile_cordscript_try	*/
/*   -------------------------	*/
private	struct cordscript_instruction * compile_cordscript_try( struct cordscript_context * cptr )
{
	struct	cordscript_instruction * iptr;
	struct	cordscript_instruction * jptr;
	struct	cordscript_instruction * kptr;
	struct	cordscript_instruction * xptr;
	struct	cordscript_instruction * aptr;
	struct	cordscript_label * lptr;
	if ( get_punctuation() != '{' )
		return((struct cordscript_instruction *) 0);
	else if (!( aptr = allocate_cordscript_instruction( enter_operation ) ))
		return( aptr );
	else	add_instruction( cptr, aptr );

	if (!( xptr = allocate_cordscript_instruction( no_operation ) ))
		return( xptr );
	else if (!( jptr = allocate_cordscript_instruction( leave_operation ) ))
		return( jptr );
	else if (!( kptr = allocate_cordscript_instruction( leave_operation ) ))
		return( kptr );
	else if (!( iptr = allocate_cordscript_instruction( set_operation ) ))
		return( iptr );
	else 	
	{
		add_operand( aptr, instruction_value( iptr ) );

		/* ----------------------- */
		/* connect JMP to EXIT NOP */
		/* ----------------------- */
		add_operand( jptr, instruction_value( xptr ) );
		jptr->next = iptr;
		iptr->next = kptr;

		/* --------------- */
		/* Set EXIT to JMP */
		/* --------------- */
		if (( lptr = allocate_cordscript_label( xptr, _TRY_LABEL )) != (struct cordscript_label *)0)
		{
			/* ------------------------ */
			/* and other to SET / CATCH */
			/* ------------------------ */
			lptr->other = jptr;
		}
		return((struct cordscript_instruction *) 0);
	}
}			

/*   -------------------------	*/
/*   compile_cordscript_catch	*/
/*   -------------------------	*/
private	struct cordscript_instruction * compile_cordscript_catch( struct cordscript_context * cptr )
{
	return((struct cordscript_instruction *) 0);
}			

/*   -------------------------	*/
/*   compile_cordscript_throw	*/
/*   -------------------------	*/
private	struct cordscript_instruction * compile_cordscript_throw( struct cordscript_context * cptr )
{
	struct	cordscript_instruction * iptr;
	struct	cordscript_label * lptr;

	if (!( lptr = labelheap ))
		return((struct cordscript_instruction *) 0);

	while ( lptr )
	{
		switch( lptr->type )
		{
		case	_TRY_LABEL	:
			if (!( lptr->other ))
				break;
			else if (!( lptr->other->next ))
				break;
			else if (!( iptr = allocate_cordscript_instruction( jmp_operation ) ))
				break;
			else
			{
				add_operand( iptr, instruction_value( lptr->other->next ) );
				compile_cordscript_instruction( cptr, 1 );
				add_instruction( cptr, iptr );
				check_line_end(cptr,0,';');
				break;
			}
		default			:
			lptr = lptr->next;
			continue;
		}
		break;
	}
	return((struct cordscript_instruction *) 0);
}			

/*   -------------------------	*/
/*   compile_cordscript_return	*/
/*   -------------------------	*/
private	struct cordscript_instruction * compile_cordscript_return( struct cordscript_context * cptr )
{
	int	c;
	struct	cordscript_instruction * iptr;
	if ((c = get_punctuation()) == ';' )
	{
		if (!( iptr = allocate_cordscript_instruction( ret_operation ) ))
			return( iptr );
		else
		{
			add_instruction( cptr, iptr );
			return((struct cordscript_instruction *) 0);
		}			
	}
	else if ( c == '(' )
	{
		compile_cordscript_instruction( cptr, 0 );
		if (( c = get_punctuation()) != ')') 
			return((struct cordscript_instruction *) 0);
		if ((c = get_punctuation()) == ';' )
		{
			if (!( iptr = allocate_cordscript_instruction( retvalue_operation ) ))
				return( iptr );
			else
			{
				add_instruction( cptr, iptr );
				return((struct cordscript_instruction *) 0);
			}			
		}
		else	return((struct cordscript_instruction *) 0);
	}
	else	return((struct cordscript_instruction *) 0);
}

/*   -----------------	*/
/*   start_instruction	*/
/*   -----------------	*/
private	struct cordscript_instruction * start_instruction( struct cordscript_context * cptr, char * token )
{
	struct	cordscript_instruction * iptr;
	struct	cordscript_value * fptr;
	if (!( strcmp( token, "if" ) ))
		return( compile_cordscript_if( cptr ) );
	else if (!( strcmp( token, "else" ) ))
		return( compile_cordscript_else( cptr ) );
	else if (!( strcmp( token, "while" ) ))
		return( compile_cordscript_while( cptr ) );
	else if (!( strcmp( token, "for" ) ))
		return( compile_cordscript_for( cptr ) );
	else if (!( strcmp( token, "foreach" ) ))
		return( compile_cordscript_foreach( cptr ) );
	else if (!( strcmp( token, "forboth" ) ))
		return( compile_cordscript_forboth( cptr ) );
	else if (!( strcmp( token, "try" ) ))
		return( compile_cordscript_try( cptr ) );
	else if (!( strcmp( token, "catch" ) ))
		return( compile_cordscript_catch( cptr ) );
	else if (!( strcmp( token, "throw" ) ))
		return( compile_cordscript_throw( cptr ) );
	else if (!( strcmp( token, "switch" ) ))
		return( compile_cordscript_switch( cptr ) );
	else if (!( strcmp( token, "case" ) ))
		return( compile_cordscript_case( cptr ) );
	else if (!( strcmp( token, "default" ) ))
		return( compile_cordscript_default( cptr ) );
	else if (!( strcmp( token, "break" ) ))
		return( compile_cordscript_break( cptr ) );
	else if (!( strcmp( token, "continue" ) ))
		return( compile_cordscript_continue( cptr ) );
	else if (!( strcmp( token, "return" ) ))
		return( compile_cordscript_return( cptr ) );
	else if (!( strcmp( token, "function" ) ))
		return( compile_cordscript_function( cptr ) );
	else if (!( strcmp( token, "include" ) ))
		return( compile_cordscript_include( cptr ) );
	else if (( fptr = resolve_function( token, cptr )) !=(struct cordscript_value *) 0)
		return( compile_cordscript_call( cptr, fptr ) );
	else if (!( iptr = allocate_cordscript_instruction( push_operation )))
		return( iptr );
	else if (!( iptr->first = iptr->last = allocate_cordscript_operand( resolve_value( token, cptr ) )))
		return( liberate_cordscript_instruction( iptr ) );
	else	return( iptr );
}

/*	-----------	*/
/*	handle_else	*/
/*	-----------	*/
private	struct cordscript_instruction * handle_else( struct cordscript_context * cptr,struct cordscript_label * lptr )
{
	struct	cordscript_instruction * jptr;
	struct	cordscript_instruction * xptr;
	struct	cordscript_instruction * yptr;

	if ( get_punctuation() != '{' )
	{
		lptr = liberate_cordscript_label( lptr );
		return((struct cordscript_instruction *) 0);
	}

	else if (!( xptr = allocate_cordscript_instruction( no_operation ) ))
	{
		lptr = liberate_cordscript_label( lptr );
		return( xptr );
	}
	else if (!( jptr = allocate_cordscript_instruction( jmp_operation ) ))
	{
		xptr = liberate_cordscript_instruction( xptr );
		lptr = liberate_cordscript_label( lptr );
		return( jptr );
	}
	else
	{
		add_operand( jptr, instruction_value( xptr ) );
		add_instruction( cptr, jptr );
	}

	for (	yptr=lptr->value;
		yptr != (struct cordscript_instruction *) 0;
		yptr = yptr->next )
		add_instruction( cptr, yptr );

	lptr->value = xptr;
	lptr->type = 0;
	return((struct cordscript_instruction *) 0);
}

/*	------------	*/
/*	handle_catch	*/
/*	------------	*/
private	struct cordscript_instruction * handle_catch( struct cordscript_context * cptr,struct cordscript_label * lptr )
{
	struct	cordscript_value * vptr;
	struct	cordscript_instruction * iptr;
	char	buffer[_MAX_NAME];
	
	if ( lptr->type != _TRY_LABEL )
		return((struct cordscript_instruction *) 0);
	else if (!( iptr = lptr->other ))
		return((struct cordscript_instruction *) 0);
	else if (!( iptr = iptr->next ))
		return((struct cordscript_instruction *) 0);
	else if ( get_punctuation() != '(' )
		return((struct cordscript_instruction *) 0);
	else if (!( get_token( buffer ) ))
		return((struct cordscript_instruction *) 0);
	else if (!( vptr = resolve_value( buffer, cptr ) ))
		return((struct cordscript_instruction *) 0);
	else if ( get_punctuation() != ')' )
		return((struct cordscript_instruction *) 0);
	else if ( get_punctuation() != '{' )
		return((struct cordscript_instruction *) 0);
	else
	{
		add_operand( iptr, vptr );
		flush_label_instructions( cptr, lptr->other );
		lptr->other = (struct cordscript_instruction *) 0;
		lptr->type  = _CATCH_LABEL;
		return((struct cordscript_instruction *) 0);
	}
}

/*   ------------------------------ */
/*   compile_cordscript_instruction */
/*   ------------------------------ */
private	struct	cordscript_instruction * compile_cordscript_instruction( struct cordscript_context * cptr, int level )
{
	char	token[_MAX_VALUE];
	int	c;
	int	bracing=0;
	int	operands=0;
	struct	cordscript_value * vptr;
	struct	cordscript_instruction * iptr;
	struct	cordscript_instruction * jptr;
	struct	cordscript_instruction * xptr;

	/* -------------------------------------- */
	/* collect token and start an instruction */
	/* -------------------------------------- */
 	if (!( get_token( token ) ))
	{
	 	return( (struct cordscript_instruction *) 0 );
   	}
	else if (!( iptr = start_instruction( cptr, token )))
	{
		 return( iptr );
   	}

   	while (1)
	{
		if (!( c = get_punctuation() ))
			break;
		/* resolve object method member */
		else if ( c == '.' )
		{
			if (!( get_token( token ) ))
				break;
			else if (!( vptr = resolve_value( token, cptr ) ))
				break;
			else
				add_operand( iptr, vptr );
		}
		/* resolve structure or array member */
		else if ( c == '[' )
		{
			if (!( compile_cordscript_instruction( cptr, (level+1) ) ))
				return( liberate_cordscript_instruction( iptr ) );
			else if ( get_punctuation() != ']' )
				return( liberate_cordscript_instruction( iptr ) );
			else
			{
				iptr->evaluate = eval_operation;
				add_operand( iptr, string_value( "member" ));
				add_operand( iptr, integer_value( 1 ));
				continue;
			}			
		}
		else if ( c == '=' )
		{
			if ((c = get_punctuation()) == '=' )
			{
				unget_byte(c);
				break;
			}
			else if ( c )
				unget_byte(c);

			iptr->evaluate = set_operation;

			/* ------------------------------- */
			/* compile the affectation operand */
			/* the result will be on the stack */
			/* ------------------------------- */
			if (!( compile_cordscript_instruction( cptr, (level+1) ) ))
			{
				return( liberate_cordscript_instruction( iptr ) );
			}
			else	break;
			
		}
		else if ( c == '+' )
		{	
			iptr->evaluate = add_operation;
			/* ------------------------------- */
			/* compile the affectation operand */
			/* the result will be on the stack */
			/* ------------------------------- */
			if (!( compile_cordscript_instruction( cptr, (level+1) ) ))
			{
				return( liberate_cordscript_instruction( iptr ) );
			}
			else	break;
			
		}
		else if ( c == '#' )
		{	
			iptr->evaluate = cat_operation;
			/* ------------------------------- */
			/* compile the affectation operand */
			/* the result will be on the stack */
			/* ------------------------------- */
			if (!( compile_cordscript_instruction( cptr, (level+1) ) ))
			{
				return( liberate_cordscript_instruction( iptr ) );
			}
			else	break;
			
		}
		else if ( c == '-' )
		{	
			iptr->evaluate = sub_operation;
			/* ------------------------------- */
			/* compile the affectation operand */
			/* the result will be on the stack */
			/* ------------------------------- */
			if (!( compile_cordscript_instruction( cptr, (level+1) ) ))
			{
				return( liberate_cordscript_instruction( iptr ) );
			}
			else	break;
			
		}
		else if ( c == '*' )
		{	
			iptr->evaluate = mul_operation;
			/* ------------------------------- */
			/* compile the affectation operand */
			/* the result will be on the stack */
			/* ------------------------------- */
			if (!( compile_cordscript_instruction( cptr, (level+1) ) ))
			{
				return( liberate_cordscript_instruction( iptr ) );
			}
			else	break;
			
		}
		else if ( c == '/' )
		{	
			iptr->evaluate = div_operation;
			/* ------------------------------- */
			/* compile the affectation operand */
			/* the result will be on the stack */
			/* ------------------------------- */
			if (!( compile_cordscript_instruction( cptr, (level+1) ) ))
			{
				return( liberate_cordscript_instruction( iptr ) );
			}
			else	break;
		}			
		else if ( c == '%' )
		{	
			iptr->evaluate = mod_operation;
			/* ------------------------------- */
			/* compile the affectation operand */
			/* the result will be on the stack */
			/* ------------------------------- */
			if (!( compile_cordscript_instruction( cptr, (level+1) ) ))
			{
				return( liberate_cordscript_instruction( iptr ) );
			}
			else	break;
			
		}
		else if ( c == '(' )
		{
			if ( operands )
				break;
			else	iptr->evaluate = eval_operation;
			/* --------------------------------- */
			/* parameters to be stacked for eval */
			/* --------------------------------- */
			while (1)
			{
				if (( c = get_punctuation() ) != 0)
				{
					if ( c == ')' )
						break;
					else if ( c == ',' )
						continue;
					else if ((c == '[' ) || ( c == '{' ))
					{
						unget_byte( c ); 
						if (!( jptr = compile_cordscript_instruction(cptr, (level+1) ) ))
							break;
						else
						{
							operands++;
							continue;
						}
					}
					else 	break;
				}
				else if (!( jptr = compile_cordscript_instruction(cptr, (level+1) ) ))
					break;
				else	operands++;
			}
			if (( operands ) && ( c == ')' ))
			{
				add_operand( iptr, integer_value( operands ) );
			}
		}
		else if ( c == ';' )
		{
			break;
		}
		else if (( c == '<' )
		     ||  ( c == '>' )
		     ||  ( c == '!' )
		     ||  ( c == ':' )
		     ||  ( c == ',' )
		     ||  ( c == ']' )
		     ||  ( c == '}' )
		     ||  ( c == ')' ))
		{
			unget_byte( c );
			break;
		}
	}
	/* -------------------------------- */
	/* add this instruction to the list */
	/* -------------------------------- */
	add_instruction(cptr, iptr );

	check_line_end(cptr, level,c);

	return(iptr);
}

/*   ------------------------------- */
/*   compile_cordscript_instructions */
/*   ------------------------------- */
/*   this function analyses the string   */
/*   parameter and builds cordscript */
/*   instructions that are appended to   */
/*   the current execution context.  */
/*   ------------------------------- */
private	struct	cordscript_context * compile_cordscript_instructions( struct cordscript_context * cptr, char * buffer )
{
	struct	cordscript_instruction * iptr;

	if (!( initialise_line( buffer ) ))
	   return( cptr );

	while ( remove_white() )
		  iptr = compile_cordscript_instruction( cptr, 0 );

	return( cptr );
}

/*   ------------------------- */
/*   compile_cordscript_string */
/*   ------------------------- */
public struct cordscript_context * compile_cordscript_string( char * expression )
{
	struct	cordscript_context * cptr;
	if (!( cptr = allocate_cordscript_context() ))
		return( cptr );
	else
	{
		return( compile_cordscript_instructions( cptr, expression ) );
	}
}

/*   ----------------------- */
/*   compile_cordscript_file */
/*   ----------------------- */
public struct cordscript_context	* compile_cordscript_file( char * expression, int argc, char * argv[] )
{
	struct	cordscript_instruction * iptr;
	struct	cordscript_context * cptr;
	struct	cordscript_value * vptr;
	FILE 	*	h;
	char 	*	lptr;
	int		argi;
	char	buffer[8192];
	if (!( h = fopen( expression , "r" ) ))
		return((struct cordscript_context *) 0);
	else if (!( cptr = allocate_cordscript_context() ))
		return( cptr );
	else
	{
		for ( argi=0; argi < argc; argi++ )
		{
			sprintf(buffer,"$%u",argi+1);
			if (!( argv[argi] ))
				break;
			else if (!( vptr = resolve_variable( buffer, cptr )))
				break;
			else if (!( vptr->value = allocate_string( argv[argi] )))
				break;
			else	continue;
		}
	 	linecounter=0;
		abandon_compile=0;
		initialise_file( h );
		while ( remove_white() )
		{	
			iptr = compile_cordscript_instruction( cptr, 0 );
			if ( get_punctuation() != 0 )
				break;
			else if ( abandon_compile )
				break;
		}
		fclose(h);
		return( cptr );
	}

}

/*   ----------- */
/*   stack_trace */
/*   ----------- */
public void	stack_trace( struct  cordscript_context * cptr )
{
	struct	cordscript_value * vptr;
	printf("Stack Trace:\n");
	for ( 	vptr=cptr->stack; 
		vptr != (struct cordscript_value*) 0;
		vptr = vptr->next )
		show_value( vptr );
	return;
}

/*   ------------- */
/*   variable_heap */
/*   ------------- */
public void	variable_heap( struct  cordscript_context * cptr )
{
	struct	cordscript_value * vptr;
	printf("Variable Heap:\n");
	for ( 	vptr=cptr->data; 
		vptr != (struct cordscript_value*) 0;
		vptr = vptr->next )
		show_value( vptr );
	return;
}

/*   ------------- */
/*   function_heap */
/*   ------------- */
public void	function_heap( struct  cordscript_context * cptr )
{
	struct	cordscript_value * vptr;
	printf("Function Heap:\n");
	for ( 	vptr=cptr->code; 
		vptr != (struct cordscript_value*) 0;
		vptr = vptr->next )
		show_value( vptr );
	return;
}

/*   ------------------ */
/*   execute_cordscript */
/*   ------------------ */
public struct cordscript_value 	* execute_cordscript( struct  cordscript_context * cptr )
{
	struct	cordscript_value * vptr;
	struct	cordscript_instruction * iptr;
	if (!( cptr ))
		return((struct cordscript_value *) 0);
	else 
	{
		for (	iptr = cptr->ip = cptr->cs;
			iptr != (struct cordscript_instruction *) 0;
			iptr = (*iptr->evaluate)( iptr  ) );

		if (!( cptr->stack ))
			return((struct cordscript_value *) 0);
		else	return( pop_stack( cptr ) );
	}
}

/*   ----------- */
/*   interpreter */
/*   ----------- */
public	int	cordscript_interpreter( char * sptr, int argc, char * argv[] )
{
	struct	cordscript_context * cptr;
	struct	cordscript_value * vptr;
	if ( check_verbose() )
	{
		printf("Cordscript Interpreter\n");
		printf("Compiling: %s \n",sptr);
	}
	if (!( cptr = compile_cordscript_file( sptr, argc, argv ) ))
		return( failure(30, "error compiling", sptr) );
	else
	{
		if ( check_verbose() )
			printf("Execution:\n");
		if (( vptr = execute_cordscript( cptr ))!= (struct cordscript_value *) 0)
		{
			show_value( vptr );
			drop_value( vptr );
		}
		if ( check_verbose() )
		{
			stack_trace( cptr );
			variable_heap( cptr );
			function_heap( cptr );
			printf("Labels: %u\nValues: %u\nInstructions: %u\nOperands: %u\n",
				labels,values,instructions,operands);
			printf("End\n");
		}
		cptr = liberate_cordscript_context( cptr );
		return( 0 );
	}
}

#endif	/* _cspi_c */

