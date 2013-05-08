/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */

#ifndef	_strukt_sql_c
#define	_strukt_sql_c

#include <stdio.h>
#include <string.h>
#include "item.h"
#include "strukt.h"
extern	struct	strukt_context C;

void	generate_occi_sql_on_field( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	fprintf(h,"\nprivate int %s_sql_on_field( char * nptr, char * vptr, int vlen )\n{\n",fullname);
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( strcmp( iptr->name, "previous" ) ))
			continue;
		else if (!( strcmp( iptr->name, "next" ) ))
			continue;
		else if (!( strcmp( iptr->name, "parent" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			if (items++)
				fprintf(h,"\telse if (!( strcmp(nptr,%c%s%c) ))\n",0x0022,iptr->name,0x0022);
			else	fprintf(h,"\tif (!( strcmp(nptr,%c%s%c) ))\n",0x0022,iptr->name,0x0022);
			fprintf(h,"\t\tvlen=0;\n");
		}
	}
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}

void	generate_occi_sql_on_create( FILE * h, char * nptr, char * fullname )
{
	char	buffer[1024];
	char *	xptr=(char *) 0;
	char *	wptr;
	struct	item * iptr;
	int	items=0;
	fprintf(h,"\nprivate int %s_sql_on_create()\n{\n",fullname);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, (void *) 0};\n");
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( strcmp( iptr->name, "previous" ) ))
			continue;
		else if (!( strcmp( iptr->name, "next" ) ))
			continue;
		else if (!( strcmp( iptr->name, "parent" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			if (!( strcmp( iptr->basic, "int" ) ))
				sprintf(buffer, "%s %s INTEGER",(xptr ? "," : " ("),iptr->name);
			else 	sprintf(buffer, "%s %s CHAR(128)",(xptr ? "," : " ("),iptr->name);
			if ( xptr )
			{
				if (!( wptr = allocate( strlen( xptr ) + strlen( buffer ) + 3 ) ))
					break;
				else	sprintf(wptr,"%s%s",xptr,buffer);
				liberate( xptr );
				xptr = wptr;
			}
			else	xptr = allocate_string( buffer );
			if (!( strcmp( iptr->name, "id" ) ))
				xptr = join_string( xptr, " PRIMARY KEY");							
		}
	}
	if ( xptr )
	{
		xptr = join_string( xptr, " ) ");
		fprintf(h,"\tif (!( expression.value = allocate_string(%c%s%c) ))\n",0x0022,xptr,0x0022);
		fprintf(h,"\t\treturn( 27 );\n");
	}
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}

void	generate_occi_sql_on_remove( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	fprintf(h,"\nprivate int %s_sql_on_remove()\n{\n",fullname);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, (void *) 0};\n");
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( strcmp( iptr->name, "previous" ) ))
			continue;
		else if (!( strcmp( iptr->name, "next" ) ))
			continue;
		else if (!( strcmp( iptr->name, "parent" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			items++;
		}
	}
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}
void	generate_occi_sql_on_insert( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	fprintf(h,"\nprivate int %s_sql_on_insert()\n{\n",fullname);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, (void *) 0};\n");
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( strcmp( iptr->name, "previous" ) ))
			continue;
		else if (!( strcmp( iptr->name, "next" ) ))
			continue;
		else if (!( strcmp( iptr->name, "parent" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			items++;
		}
	}
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}
void	generate_occi_sql_on_search( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	fprintf(h,"\nprivate int %s_sql_on_search()\n{\n",fullname);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, %s_sql_on_field };\n",fullname);
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( strcmp( iptr->name, "previous" ) ))
			continue;
		else if (!( strcmp( iptr->name, "next" ) ))
			continue;
		else if (!( strcmp( iptr->name, "parent" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			items++;
		}
	}
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}

void	generate_occi_sql_on_update( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	fprintf(h,"\nprivate int %s_sql_on_update()\n{\n",fullname);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, (void *) 0};\n");
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( strcmp( iptr->name, "previous" ) ))
			continue;
		else if (!( strcmp( iptr->name, "next" ) ))
			continue;
		else if (!( strcmp( iptr->name, "parent" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			items++;
		}
	}
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}
void	generate_occi_sql_on_delete( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	fprintf(h,"\nprivate int %s_sql_on_delete()\n{\n",fullname);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, (void *) 0};\n");
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( strcmp( iptr->name, "previous" ) ))
			continue;
		else if (!( strcmp( iptr->name, "next" ) ))
			continue;
		else if (!( strcmp( iptr->name, "parent" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			items++;
		}
	}
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}

void	generate_occi_sql_on_first( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	fprintf(h,"\nprivate int %s_sql_on_first()\n{\n",fullname);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, %s_sql_on_field };\n",fullname);
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( strcmp( iptr->name, "previous" ) ))
			continue;
		else if (!( strcmp( iptr->name, "next" ) ))
			continue;
		else if (!( strcmp( iptr->name, "parent" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			items++;
		}
	}
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}

void	generate_occi_sql_on_previous( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	fprintf(h,"\nprivate int %s_sql_on_previous()\n{\n",fullname);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, %s_sql_on_field };\n",fullname);
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( strcmp( iptr->name, "previous" ) ))
			continue;
		else if (!( strcmp( iptr->name, "next" ) ))
			continue;
		else if (!( strcmp( iptr->name, "parent" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			items++;
		}
	}
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}

void	generate_occi_sql_on_next( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	fprintf(h,"\nprivate int %s_sql_on_next()\n{\n",fullname);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, %s_sql_on_field };\n",fullname);
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( strcmp( iptr->name, "previous" ) ))
			continue;
		else if (!( strcmp( iptr->name, "next" ) ))
			continue;
		else if (!( strcmp( iptr->name, "parent" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			items++;
		}
	}
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}

void	generate_occi_sql_on_last( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	fprintf(h,"\nprivate int %s_sql_on_last()\n{\n",fullname);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, %s_sql_on_field };\n",fullname);
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( strcmp( iptr->name, "previous" ) ))
			continue;
		else if (!( strcmp( iptr->name, "next" ) ))
			continue;
		else if (!( strcmp( iptr->name, "parent" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			items++;
		}
	}
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}

void	generate_occi_sql_builder( FILE * h, char * nptr )
{
	char	* prefix;
	int	item=0;
	char	buffer[512];
	struct	item * iptr;


	title(h,"OCCI MYSQL BUILDER START");

	fprintf(h,"\n#include <%s>\n","mysql.h");
	fprintf(h,"#include %c%s%c\n",0x0022,"occisql.h",0x0022);


	sprintf(buffer,"%s_%s",nptr,C.name);

	generate_occi_sql_on_field(h, nptr, buffer );
	generate_occi_sql_on_create(h, nptr, buffer );
	generate_occi_sql_on_remove(h, nptr, buffer );
	generate_occi_sql_on_insert(h, nptr, buffer );
	generate_occi_sql_on_search(h, nptr, buffer );
	generate_occi_sql_on_update(h, nptr, buffer );
	generate_occi_sql_on_delete(h, nptr, buffer );
	generate_occi_sql_on_first(h, nptr, buffer );
	generate_occi_sql_on_last(h, nptr, buffer );
	generate_occi_sql_on_previous(h, nptr, buffer );
	generate_occi_sql_on_next(h, nptr, buffer );

	title(h,buffer );

	fprintf(h,"public struct  occi_%s_node * occi_first_%s_node() { return( %s_first ); }\n",
		C.klass,C.klass,C.name);
	fprintf(h,"public struct  occi_%s_node * occi_last_%s_node() { return( %s_last ); }\n",
		C.klass,C.klass,C.name);
	fprintf(h,"public struct  occi_%s_node * occi_first_%s_node() { return( %s_first ); }\n",
		C.klass,C.klass,C.name);
	fprintf(h,"public struct  occi_%s_node * occi_last_%s_node() { return( %s_last ); }\n",
		C.klass,C.klass,C.name);
	fprintf(h,"public  void set_autosave_%s_name(char * fn) {\nreturn;\n",C.name);

	fprintf(h,"public  void autosave_%s_nodes() {\nreturn;\n}",C.name);

	/* ------------ */
	/* generate get */
	/* ------------ */
	title(h,"occi category rest interface method get");
	fprintf(h,"private struct rest_response * %s_get(",buffer);
	fprintf(h,"void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_response * aptr;\n");
	fprintf(h,"\tstruct rest_header   * hptr;\n");
	fprintf(h,"\tstruct occi_category * optr;\n");
	fprintf(h,"\tchar * ctptr;\n");
	fprintf(h,"\tchar * mptr;\n");

	fprintf(h,"\tif (!( hptr = rest_resolve_header( rptr->first, %cContent-Type%c ) ))\n",
		0x0022,0x0022);
	fprintf(h,"\t\tctptr = %ctext/occi%c;\n",0x0022,0x0022);
	fprintf(h,"\telse\tctptr = hptr->value;\n");

	fprintf(h,"\tif (!( optr = vptr )) \n");
		fprintf(h,"\t\treturn( rest_bad_request(vptr,cptr,rptr) );\n");

	fprintf(h,"\tif(!(aptr = rest_allocate_response( cptr )))\n");
		fprintf(h,"\t\treturn( aptr );\n");

	fprintf(h,"\telse if (!(strcmp( rptr->object, optr->location ) ))\n");
		fprintf(h,"\t\treturn( %s_get_list( optr, cptr, rptr, aptr ) );\n",C.name);

	fprintf(h,"\telse if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))\n");
		fprintf(h,"\t\treturn( %s_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",C.name);

	fprintf(h,"\telse\treturn( rest_html_response( aptr, 400, %cBad Request%c) );\n",0x0022,0x0022);
	fprintf(h,"}\n");

	/* ------------- */
	/* generate head */
	/* ------------- */
	title(h,"occi category rest interface method head");
	fprintf(h,"private struct rest_response * %s_head(",buffer);
	fprintf(h,"void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_response * aptr;\n");
	fprintf(h,"\tstruct rest_header   * hptr;\n");
	fprintf(h,"\tstruct occi_category * optr;\n");
	fprintf(h,"\tchar * ctptr;\n");
	fprintf(h,"\tchar * mptr;\n");
	fprintf(h,"\tif (!( hptr = rest_resolve_header( rptr->first, %cContent-Type%c ) ))\n",
		0x0022,0x0022);
	fprintf(h,"\t\tctptr = %ctext/occi%c;\n",0x0022,0x0022);
	fprintf(h,"\telse\tctptr = hptr->value;\n");

	fprintf(h,"\tif (!( optr = vptr )) \n");
		fprintf(h,"\t\treturn( rest_bad_request(vptr,cptr,rptr) );\n");

	fprintf(h,"\tif(!(aptr = rest_allocate_response( cptr )))\n");
		fprintf(h,"\t\treturn( aptr );\n");

	fprintf(h,"\telse if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))\n");
	fprintf(h,"\t\treturn( %s_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",C.name);

	fprintf(h,"\telse\treturn( rest_html_response( aptr, 400, %cBad Request%c) );\n",0x0022,0x0022);
	fprintf(h,"}\n");

	/* ------------- */
	/* generate post */
	/* ------------- */
	title(h,"occi category rest interface method post");
	fprintf(h,"private struct rest_response * %s_post(",buffer);
	fprintf(h,"void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_response * aptr;\n");
	fprintf(h,"\tstruct rest_header   * hptr;\n");
	fprintf(h,"\tstruct occi_category * optr;\n");
	fprintf(h,"\tchar * ctptr;\n");

	fprintf(h,"\tchar * mptr;\n");
	fprintf(h,"\tif (!( hptr = rest_resolve_header( rptr->first, %cContent-Type%c ) ))\n",
		0x0022,0x0022);
	fprintf(h,"\t\tctptr = %ctext/occi%c;\n",0x0022,0x0022);
	fprintf(h,"\telse\tctptr = hptr->value;\n");

	fprintf(h,"\tif (!( optr = vptr )) \n");
		fprintf(h,"\t\treturn( rest_bad_request(vptr,cptr,rptr) );\n");

	fprintf(h,"\tif(!(aptr = rest_allocate_response( cptr )))\n");
		fprintf(h,"\t\treturn( aptr );\n");

	fprintf(h,"\telse if (!( strcmp( rptr->object, optr->location ) ))\n");
	fprintf(h,"\t\treturn( %s_post_item( optr, cptr, rptr, aptr ) );\n",C.name);
	fprintf(h,"\telse if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)\n");
	fprintf(h,"\t\treturn( rest_html_response( aptr, 400, %cBad Request%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse if (!( rptr->parameters ))\n");
	fprintf(h,"\t\treturn( rest_html_response( aptr, 400, %cBad Request%c) );\n",0x0022,0x0022);
	switch( C.genrest )
	{
	case	_OCCI_KIND	:
		fprintf(h,"\telse if (!( strncmp( rptr->parameters, %caction=%c, strlen(%caction=%c)) ))\n",
			0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"\t\treturn( %s_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",C.name);
		fprintf(h,"\telse if (!( strncmp( rptr->parameters, %cmixin=%c, strlen(%cmixin=%c)) ))\n",
			0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"\t\treturn( %s_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",C.name);
		fprintf(h,"\telse if (!( strncmp( rptr->parameters, %clink=%c, strlen(%clink=%c)) ))\n",
			0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"\t\treturn( %s_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",C.name);
		fprintf(h,"\telse\treturn( rest_html_response( aptr, 400, %cBad Request%c) );\n",0x0022,0x0022);
		break;
	default	:
		fprintf(h,"\telse\treturn( rest_html_response( aptr, 400, %cBad Request%c) );\n",0x0022,0x0022);
	}

	fprintf(h,"}\n");

	/* ------------ */
	/* generate put */
	/* ------------ */
	title(h,"occi category rest interface method put");
	fprintf(h,"private struct rest_response * %s_put(",buffer);
	fprintf(h,"void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_response * aptr;\n");
	fprintf(h,"\tstruct rest_header   * hptr;\n");
	fprintf(h,"\tstruct occi_category * optr;\n");
	fprintf(h,"\tchar * ctptr;\n");

	fprintf(h,"\tchar * mptr;\n");
	fprintf(h,"\tif (!( hptr = rest_resolve_header( rptr->first, %cContent-Type%c ) ))\n",
		0x0022,0x0022);
	fprintf(h,"\t\tctptr = %ctext/occi%c;\n",0x0022,0x0022);
	fprintf(h,"\telse\tctptr = hptr->value;\n");

	fprintf(h,"\tif (!( optr = vptr )) \n");
		fprintf(h,"\t\treturn( rest_bad_request(vptr,cptr,rptr) );\n");

	fprintf(h,"\tif(!(aptr = rest_allocate_response( cptr )))\n");
		fprintf(h,"\t\treturn( aptr );\n");
	fprintf(h,"\telse if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))\n");
	fprintf(h,"\t\treturn( %s_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",C.name);

	fprintf(h,"\telse\treturn( rest_html_response( aptr, 400, %cBad Request%c) );\n",0x0022,0x0022);
	fprintf(h,"}\n");

	/* --------------- */
	/* generate delete */
	/* --------------- */
	title(h,"occi category rest interface method delete");
	fprintf(h,"private struct rest_response * %s_delete(",buffer);
	fprintf(h,"void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_response * aptr;\n");
	fprintf(h,"\tstruct rest_header   * hptr;\n");
	fprintf(h,"\tstruct occi_category * optr;\n");
	fprintf(h,"\tchar * ctptr;\n");
	fprintf(h,"\tchar * mptr;\n");
	fprintf(h,"\tif (!( hptr = rest_resolve_header( rptr->first, %cContent-Type%c ) ))\n",
		0x0022,0x0022);
	fprintf(h,"\t\tctptr = %ctext/occi%c;\n",0x0022,0x0022);
	fprintf(h,"\telse\tctptr = hptr->value;\n");

	fprintf(h,"\tif (!( optr = vptr )) \n");
		fprintf(h,"\t\treturn( rest_bad_request(vptr,cptr,rptr) );\n");

	fprintf(h,"\tif(!(aptr = rest_allocate_response( cptr )))\n");
		fprintf(h,"\t\treturn( aptr );\n");

	fprintf(h,"\telse if (!(strcmp( rptr->object, optr->location ) ))\n");
		fprintf(h,"\t\treturn( %s_delete_all( optr, cptr, rptr, aptr ) );\n",C.name);

	fprintf(h,"\telse if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))\n");
		fprintf(h,"\t\treturn( %s_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",C.name);

	fprintf(h,"\telse\treturn( rest_html_response( aptr, 400, %cBad Request%c) );\n",0x0022,0x0022);
	fprintf(h,"}\n");

	/* ------------------ */
	/* generate interface */
	/* ------------------ */
	title(h,"occi category rest interface redirection");

	fprintf(h,"private void	redirect_%s_mt( struct rest_interface * iptr )\n{\n",buffer);
		fprintf(h,"\tiptr->get = %s_get;\n",buffer);
		fprintf(h,"\tiptr->post = %s_post;\n",buffer);
		fprintf(h,"\tiptr->put = %s_put;\n",buffer);
		fprintf(h,"\tiptr->delete = %s_delete;\n",buffer);
		fprintf(h,"\tiptr->head = %s_head;\n",buffer);
	fprintf(h,"\treturn;\n}\n");

	/* ---------------- */
	/* generate builder */
	/* ---------------- */
	title(h,"occi category builder");
	fprintf(h,"/* occi category rest instance builder for : %s */\n",buffer);
	fprintf(h,"public struct occi_category * %s_builder(char * a,char * b)\n{\n",buffer);
	fprintf(h,"\tchar * c=%c%s%c;\n",0x0022,C.scheme,0x0022);
	fprintf(h,"\tchar * d=%c%s%c;\n",0x0022,C.klass,0x0022);
	fprintf(h,"\tchar * e=%c%s%c;\n",0x0022,C.rel,0x0022);
	fprintf(h,"\tchar * f=%cCompatibleOne OCCI resource %s%c;\n",0x0022,C.name,0x0022);
	fprintf(h,"\tstruct occi_category * optr;\n");
	fprintf(h,"\tif (!( optr = occi_create_category(a,b,c,d,e,f) ))\n\t\treturn(optr);\n");
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( strcmp( iptr->name, "previous" ) ))
			continue;
		else if (!( strcmp( iptr->name, "next" ) ))
			continue;
		else if (!( strcmp( iptr->name, "id" ) ))
			continue;
		else if (!( strcmp( iptr->name, "parent" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			fprintf(h,"\telse if (!( optr = occi_add_attribute(optr, %c%s%c,0,0) ))\n",
				0x0022,iptr->name,0x0022);
			fprintf(h,"\t\treturn(liberate_occi_category(optr));\n");
		}
	}
	/* ----------------------------------------------------------- */
	/* start the sql database connection for the required category */
	/* ----------------------------------------------------------- */
	fprintf(h,"\telse if (!( initialise_occi_sql_table(%c%s%c) ))\n",0x0022,C.name,0X0022);
	fprintf(h,"\t\treturn(liberate_occi_category(optr));\n");

	/* -------------------------------------- */
	/* process the sql crud interface methods */
	/* -------------------------------------- */
	fprintf(h,"\telse\n\t{\n\t\tredirect_%s_mt(optr->interface);\n",buffer);

	/* ------------------------------------------------- */
	/* return the resulting category manageent structure */
	/* ------------------------------------------------- */
	fprintf(h,"\t\treturn(optr);\n\t}\n}\n");

	title(h,"OCCI MYSQL BUILDER END");

	return;

}

	// fprintf(h,"private pthread_mutex_t list_%s_control=PTHREAD_MUTEX_INITIALIZER;\n",C.name);
	// fprintf(h,"private struct occi_%s_node * %s_first = (struct occi_%s_node *) 0;\n",C.klass,C.name,C.klass);
	// fprintf(h,"private struct occi_%s_node * %s_last  = (struct occi_%s_node *) 0;\n",C.klass,C.name,C.klass);
	// fprintf(h,"private struct %s_node * liberate_%s_node(struct %s_node * nptr) {\n",
	// fprintf(h,"private struct occi_%s_node * ll_drop_%s_node(struct occi_%s_node * nptr) {\n",
	// fprintf(h,"private struct occi_%s_node * drop_%s_node(struct occi_%s_node * nptr) {\n",
	// fprintf(h,"private struct occi_%s_node * ll_locate_%s_node(char * id) {\n",C.klass,C.name);
	// fprintf(h,"private struct occi_%s_node * locate_%s_node(char * id) {\n",C.klass,C.name);
	// fprintf(h,"private struct occi_%s_node * ll_add_%s_node(int mode) {\n",C.klass,C.name);
	// fprintf(h,"private struct occi_%s_node * add_%s_node(int mode) {\n",
	// fprintf(h,"private char*autosave_%s_name=%c%s.xml%c;\n",C.name,0x0022,C.name,0x0022);
	// fprintf(h,"private void autoload_%s_nodes() {\n",C.name,C.name);
	// fprintf(h,"private void set_%s_field(\n",C.name);
	// fprintf(h,"private struct %s * filter_%s_info(\n",C.name,C.name);
	// fprintf(h,"private int pass_%s_filter(\n",C.name);
	// fprintf(h,"private struct rest_response * %s_occi_response(\n",C.name);
	// fprintf(h,"private struct rest_response * %s_get_item(\n",C.name);
	// fprintf(h,"private struct rest_response * %s_post_link(\n",C.name);
	// fprintf(h,"private struct rest_response * %s_post_mixin(\n",C.name);
	// fprintf(h,"private struct rest_response * %s_post_action(\n",C.name);
	// fprintf(h,"private struct rest_response * %s_post_item(\n",C.name);
	// fprintf(h,"private struct rest_response * %s_put_item(\n",C.name);
	// fprintf(h,"private struct rest_response * %s_head_item(\n",C.name);
	// fprintf(h,"private struct rest_response * %s_delete_item(\n",C.name);
	// fprintf(h,"private struct rest_response * %s_get_list(\n",C.name);
	// fprintf(h,"private struct rest_response * %s_delete_all(\n",C.name);
	// fprintf(h,"private struct rest_response * %s_get(",buffer);
	// fprintf(h,"private struct rest_response * %s_head(",buffer);
	// fprintf(h,"private struct rest_response * %s_post(",buffer);
	// fprintf(h,"private struct rest_response * %s_put(",buffer);
	// fprintf(h,"private struct rest_response * %s_delete(",buffer);
	// fprintf(h,"private void	redirect_%s_mt( struct rest_interface * iptr )\n{\n",buffer);
	// fprintf(h,"private struct rest_response * post_action_%s(",C.name);
	// fprintf(h,"private struct rest_response * get_action_%s(",C.name);
	// fprintf(h,"private struct rest_response * delete_action_%s(",C.name);
	// fprintf(h,"private struct rest_response * put_action_%s(",C.name);

	/* ------------- */
#endif	/* _strukt_sql_c */
	/* ------------- */

