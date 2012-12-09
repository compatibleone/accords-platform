#ifndef	_cspi_c
#define	_cspi_c

#include "cspi.h"

private	struct	cordscript_label * labelheap=(struct cordscript_label *) 0;
private	int	end_of_instruction=0;

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
		if ( vptr->value )
			value = atoi( vptr->value );
	{
		if ((wptr = pop_stack( iptr->context )) != (struct cordscript_value *) 0)
		{
			if ( wptr->value )
				value += atoi( wptr->value );
			push_value( iptr->context, integer_value( value ) );
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
		if ( vptr->value )
			value = atoi( vptr->value );
	{
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
		if ( vptr->value )
			value = atoi( vptr->value );
	{
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
		if ( vptr->value )
			value = atoi( vptr->value );
	{
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
		if ( vptr->value )
			value = atoi( vptr->value );
	{
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
	if (!( id = occi_resolve_category_provider( name, "Cordscripter", default_tls() ) ))
		return( id );
	else
	{
		sprintf(buffer,"%s/%s/",id,name);
		liberate(id);
		return( allocate_string( buffer ) );
	}
}

/*	---------------		*/
/*	 eval_operation		*/
/*	---------------		*/
private	struct	cordscript_instruction * eval_operation( struct cordscript_instruction * iptr )
{
	struct	cordscript_value * vptr;
	struct	cordscript_value * wptr;
	struct	cordscript_operand * optr;
	struct	occi_element * dptr=(struct occi_element *) 0;
	struct	occi_response* zptr=(struct occi_response *) 0;
	char *	ihost;
	char *	sptr;
	int	status;

	if ( check_debug() )
		printf("eval_operation();\n");

	/* ----------------------------------- */
	/* detect nature of evaluation operand */
	/* ----------------------------------- */
	if (((optr = iptr->first) != (struct cordscript_operand *) 0)
	&&  ((vptr = optr->value) != (struct cordscript_value   *) 0)
	&&  ((optr = optr->next)  != (struct cordscript_operand *) 0)
	&&  ((wptr = optr->value) != (struct cordscript_value   *) 0))
	{
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

		/* --------------------------------------- */
		/* check for a universal unique identifier */
		/* --------------------------------------- */
		else if (!( strncmp( vptr->value, "http", strlen("http" ) ) ))
		{
			/* rest/crud methods for instances */
			/* ------------------------------- */
			if ((!( strcasecmp( wptr->value, "get"      ) ))
			||  (!( strcmp( wptr->value, "retrieve" ) )))
			{
				push_value( iptr->context, string_value(vptr->value) );
			}
			else if ((!( strcasecmp( wptr->value, "put") ))
			     ||  (!( strcmp( wptr->value, "update" ) )))
			{
				status = 0;
			}
			else if (!( strcasecmp( wptr->value, "delete" ) ))
			{
				if (( zptr = occi_simple_delete( vptr->value, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
					zptr = occi_remove_response( zptr );
			}
		}
		/* --------------------------------------- */
		/* else it should be a category identifier */
		/* --------------------------------------- */
		else if (( sptr = build_category_reference( vptr->value )) != (char *) 0)
		{
			/* -------------------------------- */
			/* rest/crud methods for categories */
			/* -------------------------------- */
			if ((!( strcmp( wptr->value, "new"      ) ))
			||  (!( strcmp( wptr->value, "create"   ) ))
			||  (!( strcasecmp( wptr->value, "post" ) )))
			{
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
				push_value( iptr->context, string_value(sptr) );
			}
			else if (!( strcasecmp( wptr->value, "delete" ) ))
			{
				if (( zptr = occi_simple_delete( sptr, _CORDSCRIPT_AGENT, default_tls() )) != (struct occi_response *) 0)
					zptr = occi_remove_response( zptr );
			}
		}
		else
		{
			/* ------------------ */
			/* push a null result */
			/* ------------------ */
			push_value( iptr->context, null_value() );
		}
	}

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
			if (atoi(wptr->value) == atoi(vptr->value))
			{
				iptr->context->ip = lptr->code;
				drop_value( vptr );
				return(iptr->context->ip);
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
			if (atoi(wptr->value) != atoi(vptr->value))
			{
				iptr->context->ip = lptr->code;
				drop_value( vptr );
				return(iptr->context->ip);
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
			if (atoi(vptr->value) <= atoi(wptr->value))
			{
				iptr->context->ip = lptr->code;
				drop_value( vptr );
				return(iptr->context->ip);
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
			if (atoi(vptr->value) < atoi(wptr->value))
			{
				iptr->context->ip = lptr->code;
				drop_value( vptr );
				return(iptr->context->ip);
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
			if (atoi(vptr->value) > atoi(wptr->value))
			{
				iptr->context->ip = lptr->code;
				drop_value( vptr );
				return(iptr->context->ip);
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
			if (atoi(vptr->value) >= atoi(wptr->value))
			{
				iptr->context->ip = lptr->code;
				drop_value( vptr );
				return(iptr->context->ip);
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
	if (!( sptr = pop_stack( iptr->context ) ))
		return( iptr->next );
	else if (!( tptr = pop_stack( iptr->context ) ))
		return( iptr->next );
	
	/* ------------------------------ */
	/* TODI : evaluate the ForEach As */
	/* ------------------------------ */
	else if (!(optr = iptr->first))
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
	struct cordscript_value * vptr;
	struct cordscript_context * bptr;

	if ( check_debug() )
		printf("call_operation();\n");
	if (( iptr ) 
	&&  ((optr = iptr->first) != (struct cordscript_operand *) 0 ) 
	&&  ((vptr = optr->value) != (struct cordscript_value *) 0 ) 
	&&  ((bptr = vptr->body ) != (struct cordscript_context *) 0))
	{
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
		push_value( cptr, vptr );
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
private	int	initialise_file( FILE * h )
{
	infile = h;
	return( 1 );
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
	ungotc = c;
	return(0);
}

/*	------------	*/
/*	  get_byte	*/
/*	------------	*/
private	int	echo=0;
private	int	get_byte()
{
	int	c;
	if ((c = ungotc) != 0 )
		ungotc = 0;
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
/*	remove_white	*/
/*	------------	*/
private	int	remove_white()
{
	int	c;
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
		else
		{
			unget_byte(c);
			return( 1 );
		}
	}
	return(0);
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
	int	nb=0;

	if ( ungot_token )
	{
		strcpy( result, ungot_token );
		liberate( ungot_token );
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
				quote=0;
			*(result+nb) = c;
			nb++;
		}
		else if ((!( nb )) && ( c == '[' ))
			return( get_array( result ) );
		else if ((!( nb )) && ( c == '{' ))
			return( get_structure( result ) );
		else if (( c == '"' ) || ( c == 0x0027 ))
		{
			quote = c;
			*(result+nb) = c;
			nb++;
		}
		else if (!( token_legal(c) ))
		{
			unget_byte(c);
			break;
		}
		else
		{
			*(result+nb) = c;
			nb++;
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
	if (!( remove_white( linebuffer ) ))
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

/*   ----------------------------	*/
/*   compile_cordscript_statement	*/
/*   ----------------------------	*/
private	struct	cordscript_instruction * compile_cordscript_call(struct cordscript_context * cptr, struct cordscript_value * fptr )
{
	struct	cordscript_instruction * iptr;
	struct	cordscript_instruction * jptr;
	int	c;
	if ((c = get_punctuation()) != '(' )
	{
		if ( c ) unget_byte( c );
		return((struct cordscript_instruction *) 0);
	}
	else if (!( iptr = allocate_cordscript_instruction( call_operation ) ))
		return((struct cordscript_instruction *) 0);
	else
	{
		while (1)
		{
			jptr = compile_cordscript_instruction( cptr, 0 );
			if ((c = get_punctuation()) == ')' )
				break;
			else if ( c == ',' )
				continue;
			else	return((struct cordscript_instruction *) 0);
		}
		add_operand( iptr, fptr );
		add_instruction( cptr, iptr );
		if ((c = get_punctuation()) == ';' )
			return((struct cordscript_instruction *) 0);
		else 	return((struct cordscript_instruction *) 0);
	}
}

/*   ----------------------------	*/
/*   compile_cordscript_statement	*/
/*   ----------------------------	*/
private	struct	cordscript_instruction * compile_cordscript_statement()
{
	return((struct cordscript_instruction *) 0);
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
	failure(30,"syntax error","");
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
		return((struct cordscript_instruction *) 0);
	else if (!( lptr = compile_cordscript_instruction( cptr, 0 ) ))
		return((struct cordscript_instruction *) 0);

	else if (!( operator = cordscript_compare() ))
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

	if (!( iptr = allocate_cordscript_instruction( no_operation ) ))
		return( iptr );
	else
	{
		iptr->context = cptr;
		add_operand( rptr, instruction_value( iptr ) );
		if (( c = get_punctuation()) == '{' )
			allocate_cordscript_label( iptr, 1 );
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
	int	c;
	if (!( get_token( buffer ) ))
		return((struct cordscript_instruction *) 0);
	else if (!( fptr = allocate_cordscript_value( (char *) 0, buffer ) ))
		return((struct cordscript_instruction *) 0);
	else if ((c = get_punctuation()) != '(' )
		return((struct cordscript_instruction *) 0);
	else if (!( cptr = allocate_cordscript_context() ))
		return((struct cordscript_instruction *) 0);
	{
		while (1)
		{

			if ( get_token( buffer ) != 0)
			{
				if (!( vptr = allocate_cordscript_value( (char *) 0, buffer ) ))
				{
					liberate_cordscript_context( cptr );
					return((struct cordscript_instruction *) 0);
				}
				else
				{
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
				return((struct cordscript_instruction *) 0);
			}
		}		
		fptr->next = kptr->code;
		kptr->code = fptr;
		fptr->body = cptr;
		if (( c = get_punctuation()) != '{' )
		{
			liberate_cordscript_context( cptr );
			return((struct cordscript_instruction *) 0);
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

	if (!( iptr = compile_cordscript_instruction( cptr, 0 ) ))
		return((struct cordscript_instruction *) 0);

	else if (!( get_token(buffer)))
		return((struct cordscript_instruction *) 0);

	else if ( strcmp( buffer,"as" ) )
		return((struct cordscript_instruction *) 0);

	else if (!( iptr = compile_cordscript_instruction( cptr, 0 ) ))
		return((struct cordscript_instruction *) 0);

	else if ( get_punctuation() != ')' )
		return((struct cordscript_instruction *) 0);

	if (!( iptr = allocate_cordscript_instruction( each_operation ) ))
		return( iptr );
	else
	{
		add_operand( iptr, instruction_value( xptr ) );
		add_instruction( cptr, iptr );
	}

	/* ----------------------------------- */
	/* detect and handle conditional block */
	/* ----------------------------------- */
	if (( c = get_punctuation()) == '{' )
		allocate_cordscript_label( jptr, 0 );

	else if (!( c ))
	{
		add_instruction( cptr, jptr );
		add_instruction( cptr, xptr );
	}
	else
	{
		unget_byte(c);
		add_instruction( cptr, jptr );
		add_instruction( cptr, xptr );
	}
	return((struct cordscript_instruction *) 0);

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
		allocate_cordscript_label( jptr, 0 );
	else if (!( c ))
	{
		add_instruction( cptr, jptr );
		add_instruction( cptr, iptr );
	}
	else
	{
		unget_byte(c);
		add_instruction( cptr, jptr );
		add_instruction( cptr, iptr );
	}
	return((struct cordscript_instruction *) 0);
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
			allocate_cordscript_label( jptr, 0 );
		else if (!( c ))
		{
			add_instruction( cptr, jptr );
			add_instruction( cptr, fptr );
		}
		else
		{
			unget_byte(c);
			add_instruction( cptr, jptr );
			add_instruction( cptr, fptr );
		}
		return((struct cordscript_instruction *) 0);
	}
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
			if (!( iptr = allocate_cordscript_instruction( ret_operation ) ))
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
	else if (!( strcmp( token, "return" ) ))
		return( compile_cordscript_return( cptr ) );
	else if (!( strcmp( token, "function" ) ))
		return( compile_cordscript_function( cptr ) );
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
					if ( lptr->type )
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
					for (	xptr=lptr->value;
						xptr != (struct cordscript_instruction *) 0;
						xptr = xptr->next )
						add_instruction( cptr, xptr );
					lptr = liberate_cordscript_label( lptr );
				}
				else 
				{	
					if ( c != 0 ) unget_byte( c );	
					break;
				}
			}
		}
		end_of_instruction=0;
	}
	return;
}

/*   ------------------------------ */
/*   compile_cordscript_instruction */
/*   ------------------------------ */
private	struct	cordscript_instruction * compile_cordscript_instruction( struct cordscript_context * cptr, int level )
{
	char	token[_MAX_VALUE];
	int	c;
	int	bracing=0;
	struct	cordscript_value * vptr;
	struct	cordscript_instruction * iptr;
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
		else if ( c == '.' )
		{
			if (!( get_token( token ) ))
				break;
			else if (!( vptr = resolve_value( token, cptr ) ))
				break;
			else
				add_operand( iptr, vptr );
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
			iptr->evaluate = eval_operation;
			if (!( c = get_punctuation() ))
				break;
			else if ( c != ')' )
				break;
		}
		else if ( c == ';' )
		{
			break;
		}
		else if (( c == '<' )
		     ||  ( c == '>' )
		     ||  ( c == '!' )
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
public struct cordscript_context	* compile_cordscript_file( char * expression )
{
	struct	cordscript_instruction * iptr;
	struct	cordscript_context * cptr;
	FILE 	*	h;
	char 	*	lptr;
	char	buffer[8192];
	if (!( h = fopen( expression , "r" ) ))
		return((struct cordscript_context *) 0);
	else if (!( cptr = allocate_cordscript_context() ))
		return( cptr );
	else
	{
	 	linecounter=0;
		initialise_file( h );
		while ( remove_white() )
			  iptr = compile_cordscript_instruction( cptr, 0 );
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
public	int	cordscript_interpreter( char * sptr )
{
	struct	cordscript_context * cptr;
	struct	cordscript_value * vptr;
	if ( check_verbose() )
	{
		printf("Cordscript Interpreter\n");
		printf("Compiling: %s \n",sptr);
	}
	if (!( cptr = compile_cordscript_file( sptr ) ))
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

