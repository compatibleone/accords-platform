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

private	void	generate_occi_response( FILE * h )
{
	struct	item * iptr;
	/* ---------------------- */
	/* generate occi response */
	/* ---------------------- */
	title(h,"occi category rest interface method response");
	fprintf(h,"private struct rest_response * %s_occi_response(\n",C.name);
	fprintf(h,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
	fprintf(h,"\tstruct rest_request * rptr, struct rest_response * aptr,\n");
	fprintf(h,"\tstruct %s * pptr)\n",C.name);
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_header * hptr;\n");

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
			if (!( strcmp( iptr->name, "id" ) ))
				fprintf(h,"\tsprintf(cptr->buffer,%cocci.core.id=%cc%cs%cc%c,0x0022,pptr->%s,0x0022);\n",
				0x0022,0x0025,0x0025, 0x0025, 0x0022,iptr->name);
			else
			{
				if ( iptr->indirection )
				{
				  fprintf(h,"\tsprintf(cptr->buffer,%c%cs.%cs.%s=%cc%cs%cc%c,optr->domain,optr->id",
					0x0022, 0x0025,0x0025,iptr->name,0x0025,0x0025,0x0025, 0x0022);
				}
				else
				{
				  fprintf(h,"\tsprintf(cptr->buffer,%c%cs.%cs.%s=%cc%cu%cc%c,optr->domain,optr->id",
					0x0022, 0x0025,0x0025,iptr->name,0x0025,0x0025,0x0025, 0x0022);
				}
				fprintf(h,",0x0022,pptr->%s,0x0022);\n",iptr->name);
			}
			fprintf(h,"\tif (!( hptr = rest_response_header( aptr, %cX-OCCI-Attribute%c,cptr->buffer) ))\n",
				0x0022,0x0022);
			fprintf(h,"\t\treturn( rest_html_response( aptr, 500, %cServer Failure%c ) );\n",
				0x0022,0x0022);
		}
	}

	fprintf(h,"\tif ( occi_render_links( aptr, pptr->id ) != 0)\n");
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500, %cServer Link Failure%c ) );\n",
			0x0022,0x0022);
	fprintf(h,"\telse\tif (!( occi_success( aptr ) ))\n");
	fprintf(h,"\t\treturn( rest_response_status( aptr, 500, %cServer Failure%c ) );\n",
		0x0022,0x0022);
	fprintf(h,"\telse\treturn( rest_response_status( aptr, 200, %cOK%c ) );\n",
		0x0022,0x0022);
	fprintf(h,"}\n");
	return;
}

/*	-------------------------------------------------	*/
/*	  g e n e r a t e _ s e t _ o c c i _ f i e l d 	*/
/*	-------------------------------------------------	*/
private	void	generate_set_occi_field( FILE * h )
{
	struct	item * iptr;
	/* ------------------------ */
	/* generate activate filter */
	/* ------------------------ */
	title(h,"occi category rest interface method activate filter");
	fprintf(h,"private void activate_%s_filter_on_field(\n",C.name);
	fprintf(h,"\tstruct occi_category * cptr,void * target_void, char * field_name)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct %s_occi_filter * target;\n",C.name);
	fprintf(h,"\tchar prefix[1024];\n");
	fprintf(h,"\tif (!( target = target_void )) return;\n");
	fprintf(h,"\tsprintf(prefix,%c%cs.%cs.%c,cptr->domain,cptr->id);\n",0x0022,0x0025,0x0025,0x0022);
	fprintf(h,"\tif (!( strncmp( field_name, prefix, strlen(prefix) ) )) {\n");
	fprintf(h,"\t\tfield_name += strlen(prefix);\n");
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
		fprintf(h,"\t\tif (!( strcmp( field_name, %c%s%c ) ))\n",0x0022,iptr->name,0x0022);
		fprintf(h,"\t\t\ttarget->%s = 1;\n",iptr->name);
	}
	fprintf(h,"\t\t}\n");
	fprintf(h,"\treturn;\n");
	fprintf(h,"}\n");

	/* ------------------ */
	/* generate set field */
	/* ------------------ */
	title(h,"occi category rest interface method set field");
	fprintf(h,"private void set_%s_field(\n",C.name);
	fprintf(h,"\tstruct occi_category * cptr,void * target_void, char * field_name, char * value)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tchar prefix[1024];\n");
	fprintf(h,"\tif (!( pptr = target_void )) return;\n");
	fprintf(h,"\tsprintf(prefix,%c%cs.%cs.%c,cptr->domain,cptr->id);\n",0x0022,0x0025,0x0025,0x0022);
	fprintf(h,"\tif (!( strncmp( field_name, prefix, strlen(prefix) ) ))\n\t{\n");
	fprintf(h,"\t\tfield_name += strlen(prefix);\n");
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
		fprintf(h,"\t\tif (!( strcmp( field_name, %c%s%c ) ))\n\t\t{\n",0x0022,iptr->name,0x0022);
		if ( iptr->indirection )
		{
			fprintf(h,"\t\t\tif ( pptr->%s )\n\t\t\t\tpptr->%s = liberate(pptr->%s );\n",
				iptr->name, iptr->name, iptr->name),
			fprintf(h,"\t\t\tpptr->%s = allocate_string(value);\n",iptr->name);
		}
		else	fprintf(h,"\t\t\tpptr->%s = atoi(value);\n",iptr->name);
		fprintf(h,"\t\t}\n");
	}
	fprintf(h,"\t}\n");
	fprintf(h,"\treturn;\n");
	fprintf(h,"}\n");

	/* ------------------ */
	/* generate set filter on field */
	/* ------------------ */
	title(h,"occi category rest interface method set filter on field");
	fprintf(h,"private void set_%s_filter_on_field(\n",C.name);
	fprintf(h,"\tstruct occi_category * category, void * target_void, char * field_name, char * value)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct %s_occi_filter *filter = target_void;\n",C.name);
	fprintf(h, "\tset_%s_field(category, filter->attributes, field_name, value);\n", C.name);
	fprintf(h, "\tactivate_%s_filter_on_field(category, filter, field_name);\n", C.name);
	fprintf(h,"}\n");

	/* -------------------- */
	/* generate filter info */
	/* -------------------- */
	title(h,"occi category filter info");
	fprintf(h,"private int filter_%s_info(\n",C.name,C.name);
	fprintf(h,"\tstruct %s_occi_filter *filter,\n", C.name);
	fprintf(h,"\tstruct occi_category * optr,\n");
	fprintf(h,"\tstruct rest_request  * rptr,\n");
	fprintf(h,"\tstruct rest_response * aptr) {\n");
	fprintf(h,"\t*filter = (const struct %s_occi_filter) {0};\n",C.name);
	fprintf(h,"\tif (!( filter->attributes = allocate_%s()))\n",C.name);
	fprintf(h,"\t\treturn 0;\n");
	fprintf(h,"\telse if (!( occi_process_atributs(optr, rptr, aptr, filter, set_%s_filter_on_field) )) {\n",C.name);
	fprintf(h,"\t\tliberate_%s(filter->attributes);\n", C.name);
	fprintf(h,"\t\treturn 0;\n");
	fprintf(h,"\t}\n");
	fprintf(h,"\telse\treturn( 1 );\n");
	fprintf(h,"}\n");
	/* -------------------- */
	/* generate filter pass */
	/* -------------------- */
	title(h,"occi category filter pass");
	fprintf(h,"private int pass_%s_filter(\n",C.name);
	fprintf(h,"\tstruct %s * pptr, struct %s_occi_filter *filter) {\n",C.name,C.name);
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
		else if ( iptr->indirection )
		{
			fprintf(h,"\tif ( filter->%s ) {\n",iptr->name);
			fprintf(h,"\t\tif (!( pptr->%s ))\n",iptr->name);
			fprintf(h,"\t\t\treturn(0);\n");
			fprintf(h,"\t\telse if ( strcmp(pptr->%s,filter->attributes->%s) != 0)\n",iptr->name,iptr->name);
			fprintf(h,"\t\t\treturn(0);\n\t\t}\n");
		}
		else
		{
			fprintf(h,"\tif (filter->%s && (pptr->%s != filter->attributes->%s)) return(0);\n",
				iptr->name,iptr->name,iptr->name);
		}
	}
	fprintf(h,"\treturn(1);\n");
	fprintf(h,"}\n");


}

private	void	generate_prepare_response( FILE * h )
{
	/* -------------------- */
	/* response preparation */
	/* -------------------- */
	title(h,"prepare response");
	fprintf(h,"\nprivate int prepare_%s_response(struct rest_response * aptr, char * id, struct %s * pptr)\n",C.name,C.name);
	fprintf(h,"{\n");
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}

private	void	generate_prepare_location( FILE * h )
{
	/* -------------------- */
	/* location preparation */
	/* -------------------- */
	title(h,"prepare location");
	fprintf(h,"\nprivate void prepare_%s_location(char *r,char *h,int port,char *l,char *i)\n",C.name);
	fprintf(h,"{\n");
	fprintf(h,"\tchar * sptr=h;\n");
	fprintf(h,"\twhile ( *sptr ) { if (( *sptr == ':' ) && ( *(sptr+1) != '/' ))  break; else sptr++; }\n");
	fprintf(h,"\tif ( *sptr != ':' )\n");
	fprintf(h,"\t\tsprintf(r,%c%cs:%cu%cs%cs%c,h,port,l,i);\n",0x0022,0x0025,0x0025,0x0025,0x0025,0x0022);
	fprintf(h,"\telse\tsprintf(r,%c%cs%cs%cs%c,h,l,i);\n",0x0022,0x0025,0x0025,0x0025,0x0022);
	fprintf(h,"}\n");
	return;
}

private	void	generate_occi_sql_on_field( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	title(h,"sql on set field");
	fprintf(h,"\nprivate int %s_sql_on_field( void * context, char * nptr, char * vptr, int vlen )\n{\n",fullname);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tchar buffer[2048];\n");
	fprintf(h,"\tmemcpy(buffer,vptr,vlen);\n");
	fprintf(h,"\tbuffer[vlen] = 0;\n");
	fprintf(h,"\tvptr=buffer;\n");
	fprintf(h,"\tif (!( pptr = context ))\n\t\treturn(0);\n");
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
			if (!( strcmp( iptr->name, "id" ) ))
				fprintf(h,"\telse if (!( strcmp(nptr,%c%s%c) ))\n\t{\n",0x0022,iptr->name,0x0022);
			else	fprintf(h,"\telse if (!( strcmp(nptr,%c_%s%c) ))\n\t{\n",0x0022,iptr->name,0x0022);
			if (!( strcmp( iptr->basic, "int" ) ))
			{
				fprintf(h,"\t\tpptr->%s = atoi(vptr);\n",iptr->name);
				fprintf(h,"\t\treturn(0);\n");
			}
			else
			{
				fprintf(h,"\t\tif (!(pptr->%s = sql_unescaped_value(vptr)))\n\t\t\treturn(0);\n",iptr->name);
				fprintf(h,"\t\telse\treturn(0);\n");
			}
			fprintf(h,"\t}\n");
		}
	}
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}

private	void	generate_occi_sql_on_collection( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	title(h,"sql on set collection");
	fprintf(h,"\nprivate int %s_sql_on_collection( void * context, char * nptr, char * vptr, int vlen )\n{\n",fullname);
	fprintf(h,"\tstruct response_context * cptr=(struct response_context *) 0;\n");
	fprintf(h,"\tstruct rest_response * rptr=(struct rest_response *) 0;\n");
	fprintf(h,"\tstruct rest_header * hptr=(struct rest_header *) 0;\n");
	fprintf(h,"\tchar * host=(char *) 0;\n");
	fprintf(h,"\tchar * location=(char *) 0;\n");
	fprintf(h,"\tint  port=0;\n");
	fprintf(h,"\tchar buffer[2048];\n");
	fprintf(h,"\tchar value[2048];\n");
	fprintf(h,"\tif (!( cptr = context )) return(0);\n");
	fprintf(h,"\telse if (!( rptr = cptr->response )) return(0);\n");
	fprintf(h,"\telse if (!( host = cptr->host )) return(0);\n");
	fprintf(h,"\telse if (!( port = cptr->port )) return(0);\n");
	fprintf(h,"\telse if (!( location = cptr->location )) return(0);\n");
	fprintf(h,"\tmemcpy(value,vptr,vlen);\n");
	fprintf(h,"\tvalue[vlen] = 0;\n");
	fprintf(h,"\tvptr=value;\n");
	fprintf(h,"\tif (!( strcmp(nptr,%c%s%c) ))\n\t{\n",0x0022,"id",0x0022);
	fprintf(h,"\t\tprepare_%s_location(buffer,host,port,location,vptr);\n",C.name);
	fprintf(h,"\t\tif (!( hptr = rest_response_header( rptr, %cX-OCCI-Location%c,buffer) ))\n\t\treturn(0);\n",0x0022,0x0022);
	fprintf(h,"\t\telse\treturn(0);\n");
	fprintf(h,"\t}\n");
	fprintf(h,"\telse\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}

private	void	generate_file_description( FILE * h )
{
	char	buffer[1024];
	char *	xptr=(char *) 0;
	char *	wptr=(char *) 0;
	struct	item * iptr;

	if (!( 	xptr = allocate_string( "(id CHAR(80) PRIMARY KEY") ))
		return;

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
		else if (!( strcmp( iptr->name, "id" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			if (!( strcmp( iptr->basic, "int" ) ))
				sprintf(buffer, "%s _%s INTEGER",(xptr ? "," : " ("),iptr->name);
			else 	sprintf(buffer, "%s _%s CHAR(128)",(xptr ? "," : " ("),iptr->name);
			if (!( wptr = allocate( strlen( xptr ) + strlen( buffer ) + 3 ) ))
				break;
			else	sprintf(wptr,"%s%s",xptr,buffer);
			liberate( xptr );
			xptr = wptr;
		}
	}
	if ( xptr )
	{
		xptr = join_string( xptr, " ) ");
		fprintf(h,"\tchar * description=%c%s%c;\n",0x0022,xptr,0x0022);
		liberate( xptr );
	}
	return;
}

private	void	generate_occi_sql_on_create( FILE * h, char * nptr, char * fullname )
{
	title(h,"sql table handle");
	fprintf(h,"\nprivate struct occi_table * %s_table=(struct occi_table *) 0;\n",C.name);

	title(h,"sql on create table");
	fprintf(h,"\nprivate int %s_sql_on_create()\n{\n",fullname);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, (void *) 0, (void *) 0};\n");

	generate_file_description( h );

	fprintf(h,"\tif (!( expression.value = allocate_string(description) ))\n");
	fprintf(h,"\t\treturn( 27 );\n");
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}

private	void	generate_occi_sql_on_remove( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	title(h,"sql on remove table");
	fprintf(h,"\nprivate int %s_sql_on_remove()\n{\n",fullname);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, (void *) 0, (void *) 0};\n");
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}


private	void	generate_occi_sql_on_insert( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	char *	xptr=(char *) 0;
	char *	wptr=(char *) 0;
	title(h,"sql on insert record");
	fprintf(h,"\nprivate int %s_sql_on_insert(char * id, struct %s *pptr)\n{\n",fullname,C.name);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, (void *) 0, (void *) 0};\n");
	fprintf(h,"\tchar * xptr=(char *) 0;\n");
	fprintf(h,"\tchar * wptr=(char *) 0;\n");
	fprintf(h,"\tchar buffer[1024];\n");
	fprintf(h,"\tint separator='(';\n");
	if (!( xptr = allocate_string("(id")))
		return;

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
		else if (!( strcmp( iptr->name, "id" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			if (!( wptr = allocate( strlen( xptr ) + strlen( iptr->name ) + 8 ) ))
				break;
			else	sprintf(wptr,"%s, _%s",xptr,iptr->name);
			liberate( xptr );
			xptr = wptr;
		}
	}

	if (!( wptr = allocate( strlen( xptr ) + strlen( ") VALUES(" ) + 8 ) ))
		return;
	else	sprintf(wptr,"%s%s",xptr,") VALUES(");
	liberate( xptr );
	xptr = wptr;

	fprintf(h,"\tchar * prefix=%c%s%c;\n",0x0022,xptr,0x0022);
	liberate( xptr );

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
			if (!( strcmp( iptr->name, "id" ) ))
				fprintf(h,"\tsprintf(buffer,%c'%cs'%c,id);\n",0x0022,0x0025,0x0022);
			else if (!( strcmp( iptr->basic, "int" ) ))
			 	fprintf(h,"\tsprintf(buffer,%c'%cu'%c,pptr->%s);\n",0x0022,0x0025,0x0022,iptr->name);
			else 	fprintf(h,"\tsprintf(buffer,%c'%cs'%c,(rest_valid_string(pptr->%s)?sql_escaped_value(pptr->%s):%c%c));\n",0x0022,0x0025,0x0022,iptr->name,iptr->name,0x0022,0x0022);
			fprintf(h,"\tif (!( xptr )) { if (!( xptr = allocate_string(buffer) )) return(27); else separator =','; }\n");
			fprintf(h,"\telse if (!( wptr = allocate( strlen( xptr ) + strlen( buffer ) + 8 ) )) return( 27 );\n");
			fprintf(h,"\telse { sprintf(wptr,%c%cs%cc%cs%c,xptr,separator,buffer); liberate( xptr ); xptr = wptr; separator = ','; }\n",0x0022,0x0025,0x0025,0x0025,0x0022);
			items++;
		}
	}
	fprintf(h,"\tif (!( wptr = allocate( strlen( xptr ) + 8 ) )) return( 27 );\n");
	fprintf(h,"\telse { sprintf(wptr,%c%cs)%c,xptr); liberate( xptr ); xptr = wptr; }\n",0x0022,0x0025,0x0022);

	fprintf(h,"\tif (!( expression.value = allocate( strlen( xptr ) + strlen( prefix ) + 8 ) )) return( 27 );\n");
	fprintf(h,"\telse { sprintf(expression.value,%c%cs%cs%c,prefix,xptr); liberate( xptr ); }\n",0x0022,0x0025,0x0025,0x0022);

	fprintf(h,"\treturn(insert_occi_sql_record(%c%s%c,&expression));\n",0x0022,C.name,0x0022);
	fprintf(h,"}\n");
	return;
}

private	void	generate_occi_sql_on_search( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	title(h,"sql on search record");
	fprintf(h,"\nprivate int %s_sql_on_search(char * id, struct %s *pptr)\n{\n",fullname,C.name);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, %s_sql_on_field, (void *) pptr };\n",fullname);
	fprintf(h,"\tchar buffer[2048];\n");
	fprintf(h,"\tsprintf(buffer,%cWHERE id = '%cs'%c,id);\n",0x0022,0x0025,0x0022);
	fprintf(h,"\tif (!( expression.value = allocate_string(buffer))) return( 27 );\n");
	fprintf(h,"\tpptr->id = allocate_string( id );\n");
	fprintf(h,"\treturn(search_occi_sql_record(%c%s%c,&expression));\n",0x0022,C.name,0x0022);
	fprintf(h,"}\n");
	return;
}

private	void	generate_occi_sql_on_collect( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;

	title(h,"sql on collect record");
	fprintf(h,"\nprivate int %s_sql_on_collect(struct %s_occi_filter *fptr,struct response_context * cptr)\n{\n",fullname,C.name);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, %s_sql_on_collection, (void *) cptr };\n",fullname);
	fprintf(h,"\tchar buffer[2048];\n");
	fprintf(h,"\tstruct %s * pptr=(struct %s*) 0;\n",C.name,C.name);
	fprintf(h,"\tchar * xptr=(char *) 0;\n");
	fprintf(h,"\tchar * wptr=(char *) 0;\n");
	fprintf(h,"\tchar * separator=\"WHERE\";\n");

	fprintf(h,"\tif (!( pptr = fptr->attributes )) return( 118 );\n");

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
		else if (!( strcmp( iptr->name, "id" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			fprintf(h,"\tif (( fptr->%s != 0 ))\n\t{\n",iptr->name);
			if (!( strcmp( iptr->basic, "int" ) ))
			 	fprintf(h,"\t\tsprintf(buffer,%c_%s = '%cu'%c,pptr->%s);\n",0x0022,iptr->name,0x0025,0x0022,iptr->name);
			else 	fprintf(h,"\t\tsprintf(buffer,%c_%s = '%cs'%c,(rest_valid_string(pptr->%s)?sql_escaped_filter(pptr->%s):%c%c));\n",0x0022,iptr->name,0x0025,0x0022,iptr->name,iptr->name,0x0022,0x0022);
			fprintf(h,"\t\tif (!( xptr ))\n\t\t{\n");
			fprintf(h,"\t\t\tif (!( wptr = allocate( strlen(separator) + strlen(buffer) + 8 ) )) { return(27); }\n");
			fprintf(h,"\t\t\telse\n\t\t\t{\n");
			fprintf(h,"\t\t\t\tsprintf(wptr,\"%cs \%cs\",separator,buffer);\n",0x0025,0x0025);
			fprintf(h,"\t\t\t\tliberate( xptr ); xptr = wptr; separator = \" AND \";\n");
			fprintf(h,"\t\t\t}\n\t\t}\n");
			fprintf(h,"\t\telse if (!( wptr = allocate( strlen( xptr ) + strlen( buffer ) + 8 ) )) return( 27 );\n");
			fprintf(h,"\t\telse { sprintf(wptr,%c%cs%cs%cs%c,xptr,separator,buffer); liberate( xptr ); xptr = wptr; separator = \" AND \"; }\n",0x0022,0x0025,0x0025,0x0025,0x0022);
			fprintf(h,"\t}\n");
			items++;
		}
	}
	
	fprintf(h,"\tif ((xptr != (char *) 0) && (!( expression.value = xptr ))) { return( 27 ); }\n");
	fprintf(h,"\treturn(collect_occi_sql_records(%c%s%c,&expression));\n",0x0022,C.name,0x0022);
	fprintf(h,"}\n");
	return;
}

private	void	generate_occi_sql_on_delete_list( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	title(h,"sql on delete record");
	fprintf(h,"\nprivate int %s_sql_on_delete_list(struct %s_occi_filter * fptr)\n{\n",fullname,C.name);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, (void *) 0, (void *) 0};\n");
	fprintf(h,"\tchar buffer[2048];\n");
	fprintf(h,"\tstruct %s * pptr=(struct %s*) 0;\n",C.name,C.name);
	fprintf(h,"\tchar * xptr=(char *) 0;\n");
	fprintf(h,"\tchar * wptr=(char *) 0;\n");
	fprintf(h,"\tchar * separator=\"WHERE\";\n");

	fprintf(h,"\tif (!( pptr = fptr->attributes )) return( 118 );\n");

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
		else if (!( strcmp( iptr->name, "id" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			fprintf(h,"\tif (( fptr->%s != 0 ))\n\t{\n",iptr->name);
			if (!( strcmp( iptr->basic, "int" ) ))
			 	fprintf(h,"\t\tsprintf(buffer,%c_%s = '%cu'%c,pptr->%s);\n",0x0022,iptr->name,0x0025,0x0022,iptr->name);
			else 	fprintf(h,"\t\tsprintf(buffer,%c_%s = '%cs'%c,(rest_valid_string(pptr->%s)?sql_escaped_filter(pptr->%s):%c%c));\n",0x0022,iptr->name,0x0025,0x0022,iptr->name,iptr->name,0x0022,0x0022);
			fprintf(h,"\t\tif (!( xptr ))\n\t\t{\n");
			fprintf(h,"\t\t\tif (!( wptr = allocate( strlen(separator) + strlen(buffer) + 8 ) )) { return(27); }\n");
			fprintf(h,"\t\t\telse\n\t\t\t{\n");
			fprintf(h,"\t\t\t\tsprintf(wptr,\"%cs \%cs\",separator,buffer);\n",0x0025,0x0025);
			fprintf(h,"\t\t\t\tliberate( xptr ); xptr = wptr; separator = \" AND \";\n");
			fprintf(h,"\t\t\t}\n\t\t}\n");
			fprintf(h,"\t\telse if (!( wptr = allocate( strlen( xptr ) + strlen( buffer ) + 8 ) )) return( 27 );\n");
			fprintf(h,"\t\telse { sprintf(wptr,%c%cs%cs%cs%c,xptr,separator,buffer); liberate( xptr ); xptr = wptr; separator = \" AND \"; }\n",0x0022,0x0025,0x0025,0x0025,0x0022);
			fprintf(h,"\t}\n");
			items++;
		}
	}
	fprintf(h,"\tif (!( expression.value = xptr )) return( 27 );\n");
	fprintf(h,"\treturn(delete_occi_sql_record(%c%s%c,&expression));\n",0x0022,C.name,0x0022);
	fprintf(h,"}\n");
	return;
}

private	void	generate_occi_sql_on_update( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	title(h,"sql on update record");
	fprintf(h,"\nprivate int %s_sql_on_update(char * id, struct %s *pptr)\n{\n",fullname,C.name);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, (void *) 0, (void *) 0};\n");
	fprintf(h,"\tchar * xptr=(char *) 0;\n");
	fprintf(h,"\tchar * wptr=(char *) 0;\n");
	fprintf(h,"\tchar buffer[2048];\n");
	fprintf(h,"\tint separator=' ';\n");


	fprintf(h,"\tif (!( xptr = allocate_string(%cSET%c))) return(27);\n",0x0022,0x0022);

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
		else if (!( strcmp( iptr->name, "id" ) ))
			continue;
		else if (!( strncmp( iptr->name, "first", strlen("first") ) ))
			continue;
		else if (!( strncmp( iptr->name, "last", strlen("last") ) ))
			continue;
		else
		{
			if (!( strcmp( iptr->name, "id" ) ))
				fprintf(h,"\tsprintf(buffer,%cid = '%cs'%c,id);\n",0x0022,0x0025,0x0022);
			else if (!( strcmp( iptr->basic, "int" ) ))
			 	fprintf(h,"\tsprintf(buffer,%c_%s = '%cu'%c,pptr->%s);\n",0x0022,iptr->name,0x0025,0x0022,iptr->name);
			else 	fprintf(h,"\tsprintf(buffer,%c_%s = '%cs'%c,(rest_valid_string(pptr->%s)?sql_escaped_value(pptr->%s):%c%c));\n",0x0022,iptr->name,0x0025,0x0022,iptr->name,iptr->name,0x0022,0x0022);
			fprintf(h,"\tif (!( xptr )) { if (!( xptr = allocate_string(buffer) )) return(27); else separator =','; }\n");
			fprintf(h,"\telse if (!( wptr = allocate( strlen( xptr ) + strlen( buffer ) + 8 ) )) return( 27 );\n");
			fprintf(h,"\telse { sprintf(wptr,%c%cs%cc%cs%c,xptr,separator,buffer); liberate( xptr ); xptr = wptr; separator = ','; }\n",0x0022,0x0025,0x0025,0x0025,0x0022);
			items++;
		}
	}

	fprintf(h,"\tsprintf(buffer,%c WHERE id = '%cs'%c,id);\n",0x0022,0x0025,0x0022);

	fprintf(h,"\tif (!( expression.value = allocate( strlen( xptr ) + strlen( buffer ) + 8 ) )) return( 27 );\n");
	fprintf(h,"\telse { sprintf(expression.value,%c%cs%cs%c,xptr,buffer); liberate( xptr ); }\n",0x0022,0x0025,0x0025,0x0022);

	fprintf(h,"\treturn(update_occi_sql_record(%c%s%c,&expression));\n",0x0022,C.name,0x0022);
	fprintf(h,"}\n");
	return;
}

private	void	generate_occi_sql_on_delete( FILE * h, char * nptr, char * fullname )
{
	struct	item * iptr;
	int	items=0;
	title(h,"sql on delete record");
	fprintf(h,"\nprivate int %s_sql_on_delete(char * id)\n{\n",fullname);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, (void *) 0, (void *) 0};\n");
	fprintf(h,"\tchar buffer[2048];\n");
	fprintf(h,"\tsprintf(buffer,%cWHERE id = '%cs'%c,id);\n",0x0022,0x0025,0x0022);
	fprintf(h,"\tif (!( expression.value = allocate_string(buffer))) return( 27 );\n");
	fprintf(h,"\treturn(delete_occi_sql_record(%c%s%c,&expression));\n",0x0022,C.name,0x0022);
	fprintf(h,"}\n");
	return;
}

private	void	generate_build_filter( FILE * h, char * nature )
{
	fprintf(h,"\tif ( nptr->filter ) { nptr->filter = liberate( nptr->filter ); }\n");
	fprintf(h,"\tif ((nptr->filter = build_sql_filter( tptr, vptr, type )) != (char *) 0)\n");
	fprintf(h,"\t\tif (!(expression.value = allocate_string( nptr->filter )))\n");
 	fprintf(h,"\t\t\treturn((struct occi_%s_node *) 0);\n",nature);
	return;
}

private	void	generate_use_filter( FILE * h )
{
	fprintf(h,"\tif ( nptr->filter )\n\t{\n");
	fprintf(h,"\t\tstrcat( buffer, \" AND \");\n");
	fprintf(h,"\t\tstrcat( buffer, nptr->filter );\n");
	fprintf(h,"\t}\n");
	return;
}

private	void	generate_occi_sql_on_first( FILE * h, char * nptr, char * fullname )
{
	char *	nature;
	title(h,"sql on first record");
	switch ( C.genrest )
	{
	case	_OCCI_LINK :
		nature = "link";
		break;
	case	_OCCI_KIND	:
		nature = "kind";
		break;
	}
	fprintf(h,"\nprivate struct occi_%s_node * %s_sql_on_first(struct occi_%s_node * nptr, char * tptr, char * vptr, int type)\n{\n",nature,fullname,nature);
	fprintf(h,"\tint status=0;\n");
	fprintf(h,"\tstruct %s *pptr=(struct %s*) 0;\n",C.name,C.name);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, %s_sql_on_field, (void *) 0 };\n",fullname);
	fprintf(h,"\tif (!( nptr ))\n\t\treturn( nptr );\n");
	fprintf(h,"\telse if (!( pptr = (struct %s *) nptr->contents ))\n\t{\n",C.name);
	fprintf(h,"\t\tif (!( pptr = (struct %s *) allocate_%s() ))\n\t\t\t return((struct occi_%s_node *) 0 );\n",C.name,C.name,nature);
	fprintf(h,"\t\telse\tnptr->contents = pptr;\n\t}\n");
	generate_build_filter(h,nature);
	fprintf(h,"\texpression.context = pptr;\n");
	fprintf(h,"\tif ((status = first_occi_sql_record(%c%s%c,&expression)) != 0)\n",0x0022,C.name,0x0022);
 	fprintf(h,"\t\treturn((struct occi_%s_node *) 0);\n",nature);
	fprintf(h,"\tif (!( pptr->id ))\n");
 	fprintf(h,"\t\treturn((struct occi_%s_node *) 0);\n",nature);
	fprintf(h,"\telse if ((status = %s_sql_on_search(pptr->id,pptr)) != 0)\n",fullname);
 	fprintf(h,"\t\treturn((struct occi_%s_node *) 0);\n",nature);
	fprintf(h,"\telse\treturn(nptr);\n");
	fprintf(h,"}\n");
	return;
}

private	void	generate_occi_sql_on_previous( FILE * h, char * nptr, char * fullname )
{
	char *	nature;
	title(h,"sql on previous record");
	switch ( C.genrest )
	{
	case	_OCCI_LINK :
		nature = "link";
		break;
	case	_OCCI_KIND	:
		nature = "kind";
		break;
	}
	fprintf(h,"\nprivate struct occi_%s_node * %s_sql_on_previous(struct occi_%s_node * nptr)\n{\n",nature,fullname,nature);
	fprintf(h,"\tchar buffer[2048];\n");
	fprintf(h,"\tint status=0;\n");
	fprintf(h,"\tstruct %s *pptr=(struct %s*) 0;\n",C.name,C.name);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, %s_sql_on_field, (void *) 0 };\n",fullname);
	fprintf(h,"\tif (!( nptr ))\n\t\treturn( nptr );\n");
	fprintf(h,"\telse if (!( pptr = (struct %s *) nptr->contents ))\n\t{\n",C.name);
	fprintf(h,"\t\tif (!( pptr = (struct %s *) allocate_%s() ))\n\t\t\t return((struct occi_%s_node *) 0 );\n",C.name,C.name,nature);
	fprintf(h,"\t\telse\tnptr->contents = pptr;\n\t}\n");
	fprintf(h,"\tif (!( pptr->id )) { return((struct occi_%s_node *) 0); }\n",nature);
	fprintf(h,"\texpression.context = pptr;\n");
	fprintf(h,"\tsprintf(buffer,%cWHERE id < '%cs'%c,pptr->id);\n",0x0022,0x0025,0x0022);
	fprintf(h,"\tpptr->id = liberate( pptr->id );\n");
	generate_use_filter(h);
	fprintf(h,"\tif (!( expression.value = allocate_string(buffer))) return((struct occi_%s_node *)0);\n",nature);
	fprintf(h,"\tif ((status = previous_occi_sql_record(%c%s%c,&expression)) != 0)\n",0x0022,C.name,0x0022);
 	fprintf(h,"\t\treturn((struct occi_%s_node *) 0);\n",nature);
	fprintf(h,"\tif (!( pptr->id ))\n");
 	fprintf(h,"\t\treturn((struct occi_%s_node *) 0);\n",nature);
	fprintf(h,"\telse if ((status = %s_sql_on_search(pptr->id,pptr)) != 0)\n",fullname);
 	fprintf(h,"\t\treturn((struct occi_%s_node *) 0);\n",nature);
	fprintf(h,"\telse\treturn(nptr);\n");
	fprintf(h,"}\n");
	return;
}

private	void	generate_occi_sql_on_next( FILE * h, char * nptr, char * fullname )
{
	char *	nature;
	title(h,"sql on next record");
	switch ( C.genrest )
	{
	case	_OCCI_LINK :
		nature = "link";
		break;
	case	_OCCI_KIND	:
		nature = "kind";
		break;
	}
	fprintf(h,"\nprivate struct occi_%s_node * %s_sql_on_next(struct occi_%s_node * nptr)\n{\n",nature,fullname,nature);
	fprintf(h,"\tchar buffer[2048];\n");
	fprintf(h,"\tint status=0;\n");
	fprintf(h,"\tstruct %s *pptr=(struct %s*) 0;\n",C.name,C.name);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, %s_sql_on_field, (void *) 0 };\n",fullname);
	fprintf(h,"\tif (!( nptr ))\n\t\treturn( nptr );\n");
	fprintf(h,"\telse if (!( pptr = (struct %s *) nptr->contents ))\n\t{\n",C.name);
	fprintf(h,"\t\tif (!( pptr = (struct %s *) allocate_%s() ))\n\t\t\t return((struct occi_%s_node *) 0 );\n",C.name,C.name,nature);
	fprintf(h,"\t\telse\tnptr->contents = pptr;\n\t}\n");
	fprintf(h,"\tif (!( pptr->id )) { return((struct occi_%s_node *) 0); }\n",nature);
	fprintf(h,"\texpression.context = pptr;\n");
	fprintf(h,"\tsprintf(buffer,%cWHERE id > '%cs'%c,pptr->id);\n",0x0022,0x0025,0x0022);
	generate_use_filter(h);
	fprintf(h,"\tpptr->id = liberate( pptr->id );\n");
	fprintf(h,"\tif (!( expression.value = allocate_string(buffer))) return((struct occi_%s_node *)0);\n",nature);
	fprintf(h,"\tif ((status = next_occi_sql_record(%c%s%c,&expression)) != 0)\n",0x0022,C.name,0x0022);
 	fprintf(h,"\t\treturn((struct occi_%s_node *) 0);\n",nature);
	fprintf(h,"\tif (!( pptr->id ))\n");
 	fprintf(h,"\t\treturn((struct occi_%s_node *) 0);\n",nature);
	fprintf(h,"\telse if ((status = %s_sql_on_search(pptr->id,pptr)) != 0)\n",fullname);
 	fprintf(h,"\t\treturn((struct occi_%s_node *) 0);\n",nature);
	fprintf(h,"\telse\treturn(nptr);\n");
	fprintf(h,"}\n");
	return;
}

private	void	generate_occi_sql_on_last( FILE * h, char * nptr, char * fullname )
{
	char *	nature;
	title(h,"sql on last record");
	switch ( C.genrest )
	{
	case	_OCCI_LINK :
		nature = "link";
		break;
	case	_OCCI_KIND	:
		nature = "kind";
		break;
	}
	fprintf(h,"\nprivate struct occi_%s_node * %s_sql_on_last(struct occi_%s_node * nptr, char * tptr, char * vptr, int type)\n{\n",nature,fullname,nature);
	fprintf(h,"\tint status=0;\n");
	fprintf(h,"\tstruct %s *pptr=(struct %s*) 0;\n",C.name,C.name);
	fprintf(h,"\tstruct occi_expression expression={(char *) 0, %s_sql_on_field, (void *) 0 };\n",fullname);
	fprintf(h,"\tif (!( nptr ))\n\t\treturn( nptr );\n");
	fprintf(h,"\telse if (!( pptr = (struct %s *) nptr->contents ))\n\t{\n",C.name);
	fprintf(h,"\t\tif (!( pptr = (struct %s *) allocate_%s() ))\n\t\t\t return((struct occi_%s_node *) 0 );\n",C.name,C.name,nature);
	fprintf(h,"\t\telse\tnptr->contents = pptr;\n\t}\n");
	fprintf(h,"\texpression.context = pptr;\n");
	generate_build_filter(h,nature);
	fprintf(h,"\tif ((status = last_occi_sql_record(%c%s%c,&expression)) != 0)\n",0x0022,C.name,0x0022);
 	fprintf(h,"\t\treturn((struct occi_%s_node *) 0);\n",nature);
	fprintf(h,"\tif (!( pptr->id ))\n");
 	fprintf(h,"\t\treturn((struct occi_%s_node *) 0);\n",nature);
	fprintf(h,"\telse if ((status = %s_sql_on_search(pptr->id,pptr)) != 0)\n",fullname);
 	fprintf(h,"\t\treturn((struct occi_%s_node *) 0);\n",nature);
	fprintf(h,"\telse\treturn(nptr);\n");
	fprintf(h,"}\n");
	return;
}

public	void	generate_occi_sql_builder( FILE * h, char * nptr )
{
	char	* prefix;
	int	item=0;
	char	buffer[512];
	char	fullname[512];
	struct	item * iptr;


	title(h,"OCCI MYSQL BUILDER START");

	fprintf(h,"\n#include <%s>\n","mysql/mysql.h");
	fprintf(h,"#include %c%s%c\n",0x0022,"occisql.h",0x0022);

	title(h,"allocation prototypes");
	fprintf(h,"public struct %s * allocate_%s();\n",C.name,C.name);
	fprintf(h,"public struct %s * liberate_%s(struct %s * pptr);\n",C.name,C.name,C.name);

	sprintf(fullname,"%s_%s",nptr,C.name);

	generate_set_occi_field( h );
	generate_prepare_location( h );
	generate_prepare_response( h );
	generate_occi_response( h );

	generate_occi_sql_on_field(h, nptr, fullname );
	generate_occi_sql_on_collection(h, nptr, fullname );
	generate_occi_sql_on_create(h, nptr, fullname );
	generate_occi_sql_on_remove(h, nptr, fullname );
	generate_occi_sql_on_insert(h, nptr, fullname );
	generate_occi_sql_on_search(h, nptr, fullname );
	generate_occi_sql_on_collect(h, nptr, fullname );
	generate_occi_sql_on_update(h, nptr, fullname );
	generate_occi_sql_on_delete(h, nptr, fullname );
	generate_occi_sql_on_delete_list(h, nptr, fullname );
	generate_occi_sql_on_first(h, nptr, fullname );
	generate_occi_sql_on_last(h, nptr, fullname );
	generate_occi_sql_on_previous(h, nptr, fullname );
	generate_occi_sql_on_next(h, nptr, fullname );

	title(h,fullname );

	switch ( C.genrest )
	{
	case	_OCCI_LINK :
		title(h,"occi first link node" );
		fprintf(h,"public struct  occi_link_node * occi_first_link_node(char * tptr, char * vptr, int type)\n{\n");
		fprintf(h,"\tstruct occi_link_node * nptr=(struct occi_link_node *) 0;\n");
		fprintf(h,"\tstruct occi_link_node * xptr=(struct occi_link_node *) 0;\n");
		fprintf(h,"\tif (!( nptr = allocate( sizeof( struct occi_link_node )) ))\n");
		fprintf(h,"\t\treturn( nptr );\n");
		fprintf(h,"\telse\tif (!( xptr = %s_sql_on_first( nptr,tptr,vptr,type ) ))\n",fullname);
		fprintf(h,"\t\treturn( liberate( nptr ) );\n");
		fprintf(h,"\telse\treturn( xptr );\n}\n");

		title(h,"occi next link node" );
		fprintf(h,"public struct  occi_link_node * occi_next_link_node(struct occi_link_node * nptr)\n{\n");
		fprintf(h,"\tstruct occi_link_node * xptr=(struct occi_link_node *) 0;\n");
		fprintf(h,"\tif (!( nptr ))\n");
		fprintf(h,"\t\treturn( nptr );\n");
		fprintf(h,"\telse\tif (!( xptr = %s_sql_on_next( nptr ) ))\n",fullname);
		fprintf(h,"\t\treturn( liberate( nptr ) );\n");
		fprintf(h,"\telse\treturn( xptr );\n}\n");

		title(h,"occi previous link node" );
		fprintf(h,"public struct  occi_link_node * occi_previous_link_node(struct occi_link_node * nptr)\n{\n");
		fprintf(h,"\tstruct occi_link_node * xptr=(struct occi_link_node *) 0;\n");
		fprintf(h,"\tif (!( nptr ))\n");
		fprintf(h,"\t\treturn( nptr );\n");
		fprintf(h,"\telse\tif (!( xptr = %s_sql_on_previous( nptr ) ))\n",fullname);
		fprintf(h,"\t\treturn( liberate( nptr ) );\n");
		fprintf(h,"\telse\treturn( xptr );\n}\n");

		title(h,"occi last link node" );
		fprintf(h,"public struct  occi_link_node * occi_last_link_node(char * tptr, char * vptr, int type)\n{\n");
		fprintf(h,"\tstruct occi_link_node * nptr=(struct occi_link_node *) 0;\n");
		fprintf(h,"\tstruct occi_link_node * xptr=(struct occi_link_node *) 0;\n");
		fprintf(h,"\tif (!( nptr = allocate( sizeof( struct occi_link_node )) ))\n");
		fprintf(h,"\t\treturn( nptr );\n");
		fprintf(h,"\telse\tif (!( xptr = %s_sql_on_last( nptr,tptr, vptr, type ) ))\n",fullname);
		fprintf(h,"\t\treturn( liberate( nptr ) );\n");
		fprintf(h,"\telse\treturn( xptr );\n}\n");
		break;

	case	_OCCI_KIND :
		title(h,"occi first kind node" );
		fprintf(h,"public struct  occi_kind_node * occi_first_%s_node(char * tptr, char * vptr, int type)\n{\n",C.name);
		fprintf(h,"\tstruct occi_kind_node * nptr=(struct occi_kind_node *) 0;\n");
		fprintf(h,"\tstruct occi_kind_node * xptr=(struct occi_kind_node *) 0;\n");
		fprintf(h,"\tif (!( nptr = allocate( sizeof( struct occi_kind_node )) ))\n");
		fprintf(h,"\t\treturn( nptr );\n");
		fprintf(h,"\telse\tif (!( xptr = %s_sql_on_first( nptr,tptr, vptr, type  ) ))\n",fullname);
		fprintf(h,"\t\treturn( liberate( nptr ) );\n");
		fprintf(h,"\telse\treturn( xptr );\n}\n");

		title(h,"occi next kind node" );
		fprintf(h,"public struct  occi_kind_node * occi_next_%s_node(struct occi_kind_node * nptr)\n{\n",C.name);
		fprintf(h,"\tstruct occi_kind_node * xptr=(struct occi_kind_node *) 0;\n");
		fprintf(h,"\tif (!( nptr ))\n");
		fprintf(h,"\t\treturn( nptr );\n");
		fprintf(h,"\telse\tif (!( xptr = %s_sql_on_next( nptr ) ))\n",fullname);
		fprintf(h,"\t\treturn( liberate( nptr ) );\n");
		fprintf(h,"\telse\treturn( xptr );\n}\n");

		title(h,"occi previous kind node" );
		fprintf(h,"public struct  occi_kind_node * occi_previous_%s_node(struct occi_kind_node * nptr)\n{\n",C.name);
		fprintf(h,"\tstruct occi_kind_node * xptr=(struct occi_kind_node *) 0;\n");
		fprintf(h,"\tif (!( nptr ))\n");
		fprintf(h,"\t\treturn( nptr );\n");
		fprintf(h,"\telse\tif (!( xptr = %s_sql_on_previous( nptr ) ))\n",fullname);
		fprintf(h,"\t\treturn( liberate( nptr ) );\n");
		fprintf(h,"\telse\treturn( xptr );\n}\n");

		title(h,"occi last kind node" );
		fprintf(h,"public struct  occi_kind_node * occi_last_%s_node(char * tptr, char * vptr, int type)\n{\n",C.name);
		fprintf(h,"\tstruct occi_kind_node * nptr=(struct occi_kind_node *) 0;\n");
		fprintf(h,"\tstruct occi_kind_node * xptr=(struct occi_kind_node *) 0;\n");
		fprintf(h,"\tif (!( nptr = allocate( sizeof( struct occi_kind_node )) ))\n");
		fprintf(h,"\t\treturn( nptr );\n");
		fprintf(h,"\telse\tif (!( xptr = %s_sql_on_last( nptr,tptr, vptr, type  ) ))\n",fullname);
		fprintf(h,"\t\treturn( liberate( nptr ) );\n");
		fprintf(h,"\telse\treturn( xptr );\n}\n");
		break;

	}


	title(h,"autosave operations");
	fprintf(h,"public  void set_autosave_%s_name(char * fn) { return; }\n",C.name);
	fprintf(h,"public  void autosave_%s_nodes() { return; }\n",C.name);
	fprintf(h,"public  int	autosave_%s_node(struct %s * pptr)\n{\n",C.name,C.name);
	fprintf(h,"\tif (!( pptr )) return(118);\n");
	fprintf(h,"\telse if (!( pptr->id )) return(118);\n");
	fprintf(h,"\telse return(%s_sql_on_update(pptr->id, pptr));\n",fullname);
	fprintf(h,"}\n");

	/* ----------------------- */
	/* generate occi collector */
	/* ----------------------- */
	title(h,"occi collector");
	fprintf(h,"private struct %s * %s_collector(\n",C.name,C.name);
	fprintf(h,"\tstruct occi_category * optr,\n");
	fprintf(h,"\tstruct rest_client   * cptr,\n");
	fprintf(h,"\tstruct rest_request  * rptr,\n");
	fprintf(h,"\tstruct rest_response * aptr,\n");
	fprintf(h,"\tstruct %s * pptr)\n{\n\n",C.name);
	fprintf(h,"\tif ((!( pptr )) && (!( pptr = allocate_%s())))\n",C.name);
	fprintf(h,"\t\treturn(pptr);\n");
	fprintf(h,"\tif (!( occi_process_atributs(optr, rptr, aptr, pptr, set_%s_field) ))\n",C.name);
	fprintf(h,"\t\treturn(liberate_%s(pptr));\n", C.name);
	fprintf(h,"\telse\treturn(pptr);\n");
	fprintf(h,"}\n");

	/* ----------------- */
	/* generate get list */
	/* ----------------- */
	title(h,"occi category rest interface method get list");
	fprintf(h,"private struct rest_response * %s_get_list(",C.name);
	fprintf(h,"struct occi_category * optr, struct rest_client * cptr, struct rest_request * rptr,struct rest_response * aptr)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tint\tstatus=0;\n");
	fprintf(h,"\tstruct response_context context;\n");
	fprintf(h,"\tstruct %s_occi_filter filter;\n",C.name);
	fprintf(h,"\tchar * reqhost=(char *) 0;\n");
	fprintf(h,"\tint    reqport=0;\n");

	/* prepare the response context */
	fprintf(h,"\tif (!( context.host = rest_request_host(rptr) ))\n");
	fprintf(h,"\t\treturn(rest_html_response( aptr, 400, %cBad Request Host%c ));\n",0x0022,0x0022);
	fprintf(h,"\telse\tcontext.port = rptr->port;\n");
	fprintf(h,"\tcontext.response = aptr;\n");
	fprintf(h,"\tcontext.location = optr->location;\n");

	/* collect the selection criteria filter */
	fprintf(h,"\tif (!( filter_%s_info(&filter, optr, rptr, aptr ) ))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 400, %cBad Request%c ) );\n",
		0x0022,0x0022);

	/* locate the sql records in the table */
	fprintf(h,"\telse if ((status = %s_sql_on_collect(&filter,&context)) != 0)\n\t{\n",fullname);
	fprintf(h,"\t\tliberate_%s(filter.attributes);\n", C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNOT FOUND%c) );\n",0x0022,0x0022);
	fprintf(h,"\t}\n\telse\n\t{\n");
	fprintf(h,"\t\tliberate_%s(filter.attributes);\n", C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 200, %cOK%c) );\n",0x0022,0x0022);
	fprintf(h,"\t}\n");
	fprintf(h,"}\n");

	/* ----------------- */
	/* generate get item */
	/* ----------------- */
	title(h,"occi category rest interface method get item");
	fprintf(h,"private struct rest_response * %s_get_item(",C.name);
	fprintf(h,"struct occi_category * optr, struct rest_client * cptr, struct rest_request * rptr,struct rest_response * aptr, char * id)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct occi_interface * iptr;\n");
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tint    status=0;\n");
	fprintf(h,"\tstruct occi_%s_node node;\n",C.klass);
	fprintf(h,"\tmemset(&node,0,sizeof(struct occi_%s_node));\n",C.klass);
	fprintf(h,"\tiptr = optr->callback;\n");

	/* retrieve occi fields */
	fprintf(h,"\tif (!( pptr = allocate_%s()))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500, %cAllocation Failure%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse\tnode.contents = pptr;\n");

	/* locate the sql record in the table */
	fprintf(h,"\tif ((status = %s_sql_on_search(id, pptr)) != 0)\n",fullname);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNOT FOUND%c) );\n",0x0022,0x0022);

	/* invoke overloaded method */
	fprintf(h,"\tif (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,&node,rptr);\n");

	/* prepare the response header */
	fprintf(h,"\telse\treturn(%s_occi_response(optr, cptr, rptr, aptr, pptr) );\n",C.name);

	fprintf(h,"}\n");

	/* ------------ */
	/* generate get */
	/* ------------ */
	title(h,"occi category rest interface method get");
	fprintf(h,"private struct rest_response * %s_get(",fullname);
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
	title(h,"occi category rest interface method head item");
	fprintf(h,"private struct rest_response * %s_head_item(",C.name);
	fprintf(h,"struct occi_category * optr, struct rest_client * cptr, struct rest_request * rptr,struct rest_response * aptr, char * id)\n");
	fprintf(h,"{\n");
	fprintf(h,"\treturn( rest_html_response( aptr, 400, %cBad Request%c) );\n",0x0022,0x0022);
	fprintf(h,"}\n");

	title(h,"occi category rest interface method head");
	fprintf(h,"private struct rest_response * %s_head(",fullname);
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

	/* ------------------ */
	/* generate post item */
	/* ------------------ */
	title(h,"occi category rest interface method post item");
	fprintf(h,"private struct rest_response * %s_post_item(",C.name);
	fprintf(h,"struct occi_category * optr, struct rest_client * cptr, struct rest_request * rptr,struct rest_response * aptr)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_header * hptr;\n");
	fprintf(h,"\tstruct occi_interface * iptr;\n");
	fprintf(h,"\tstruct %s * pptr=(struct %s *) 0;\n",C.name,C.name);
	fprintf(h,"\tchar * reqhost=(char *) 0;\n");
	fprintf(h,"\tint    reqport=0;\n");
	fprintf(h,"\tchar * id=(char *) 0;\n");
	fprintf(h,"\tint    status=0;\n");
	fprintf(h,"\tstruct occi_%s_node node;\n",C.klass);
	fprintf(h,"\tmemset(&node,0,sizeof(struct occi_%s_node));\n",C.klass);
	fprintf(h,"\tiptr = optr->callback;\n");

	/* prepare for response */
	fprintf(h,"\tif (!( reqhost = rest_request_host(rptr) ))\n");
	fprintf(h,"\t\treturn(rest_html_response( aptr, 400, %cBad Request Host%c ));\n",0x0022,0x0022);
	fprintf(h,"\telse\treqport = rptr->port;\n");

	/* retrieve occi fields */
	fprintf(h,"\tif (!(pptr = %s_collector(optr,cptr,rptr,aptr,pptr)))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 400, %cBad Request Header%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse\tnode.contents = pptr;\n");

	/* allocate a new uuid */
	fprintf(h,"\tif (!( id = rest_allocate_uuid() ))\n");
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500, %cCannot Allocate UUID%c) );\n",0x0022,0x0022);

	/* insert a new sql record into the table */
	fprintf(h,"\telse if ((status = %s_sql_on_insert(id, pptr)) != 0)\n",fullname);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500+status, %cSQL Insert Failure %c) );\n",0x0022,0x0022);

	/* invoke overloaded method */
	fprintf(h,"\tif (( iptr ) && (iptr->create)) (*iptr->create)(optr,&node,rptr);\n");

	/* prepare the result location */
	fprintf(h,"\tprepare_%s_location(cptr->buffer,reqhost,reqport,optr->location, id);\n",C.name);
	fprintf(h,"\tif (!( hptr = rest_response_header( aptr, %cX-OCCI-Location%c,cptr->buffer) ))\n",0x0022,0x0022);

	/* check for success and return */
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500, %cServer Location Failure%c ) );\n",0x0022,0x0022);
	fprintf(h,"\telse if (!( occi_success( aptr ) ))\n");
	fprintf(h,"\t\treturn( rest_response_status( aptr, 500, %cServer OCCI Failure%c ) );\n",0x0022,0x0022);
	fprintf(h,"\telse\treturn( rest_response_status( aptr, 200, %cOK%c ) );\n",0x0022,0x0022);
	fprintf(h,"}\n");

	/* -------------------- */
	/* generate post action */
	/* -------------------- */
	title(h,"occi category rest interface method post action");
	fprintf(h,"private struct rest_response * %s_post_action(",C.name);
	fprintf(h,"struct occi_category * optr, struct rest_client * cptr, struct rest_request * rptr,struct rest_response * aptr, char * id)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tchar * mptr;\n");
	fprintf(h,"\tstruct occi_action * fptr;\n");
	fprintf(h,"\tstruct occi_interface * iptr;\n");
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tint    status=0;\n");
	fprintf(h,"\tstruct occi_%s_node node;\n",C.klass);
	fprintf(h,"\tmemset(&node,0,sizeof(struct occi_%s_node));\n",C.klass);
	fprintf(h,"\tiptr = optr->callback;\n");

	/* retrieve create occi fields */
	fprintf(h,"\tif (!( pptr = allocate_%s()))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500, %cAllocation Failure%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse\tnode.contents = pptr;\n");

	/* locate the sql record in the table */
	fprintf(h,"\tif ((status = %s_sql_on_search(id, pptr)) != 0)\n",fullname);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500+status, %cSQL Insert Failure %c) );\n",0x0022,0x0022);

	/* step over parameters to action name */
	fprintf(h,"\tmptr = (rptr->parameters+strlen(%caction=%c));\n",0x0022,0x0022);

	/* locate the action relay */
	fprintf(h,"\tfor ( fptr=optr->firstact;\n");
	fprintf(h,"\t\tfptr != (struct occi_action *) 0;\n");
	fprintf(h,"\t\tfptr = fptr->next )\n");
	fprintf(h,"\t\tif (!( strncmp( mptr, fptr->name, strlen( fptr->name )) ))\n");

	/* invoke the action relay */
	fprintf(h,"\t\t\treturn( occi_invoke_action(fptr,optr,cptr,rptr,aptr,pptr) );\n");

	/* or return failre */
	fprintf(h,"\treturn( rest_html_response( aptr, 400, %cIncorrect Action Request%c));\n",	0x0022,0x0022);
	fprintf(h,"}\n");

	/* ------------------ */
	/* generate post link */
	/* ------------------ */
	title(h,"occi category rest interface method post link");
	fprintf(h,"private struct rest_response * %s_post_link(",C.name);
	fprintf(h,"struct occi_category * optr, struct rest_client * cptr, struct rest_request * rptr,struct rest_response * aptr, char * id)\n");
	fprintf(h,"{\n");
	fprintf(h,"\treturn( rest_html_response( aptr, 400, %cBad Request%c) );\n",0x0022,0x0022);
	fprintf(h,"}\n");

	/* ------------------- */
	/* generate post mixin */
	/* ------------------- */
	title(h,"occi category rest interface method post mixin");
	fprintf(h,"private struct rest_response * %s_post_mixin(",C.name);
	fprintf(h,"struct occi_category * optr, struct rest_client * cptr, struct rest_request * rptr,struct rest_response * aptr, char * id)\n");
	fprintf(h,"{\n");
	fprintf(h,"\treturn( rest_html_response( aptr, 400, %cBad Request%c) );\n",0x0022,0x0022);
	fprintf(h,"}\n");

	/* ------------- */
	/* generate post */
	/* ------------- */
	title(h,"occi category rest interface method post");
	fprintf(h,"private struct rest_response * %s_post(",fullname);
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
	title(h,"occi category rest interface method put item");
	fprintf(h,"private struct rest_response * %s_put_item(",C.name);
	fprintf(h,"struct occi_category * optr, struct rest_client * cptr, struct rest_request * rptr,struct rest_response * aptr, char * id)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct occi_interface * iptr;\n");
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tint    status=0;\n");
	fprintf(h,"\tstruct occi_%s_node node;\n",C.klass);
	fprintf(h,"\tmemset(&node,0,sizeof(struct occi_%s_node));\n",C.klass);
	fprintf(h,"\tiptr = optr->callback;\n");

	/* retrieve occi fields */
	fprintf(h,"\tif (!( pptr = allocate_%s()))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500, %cAllocation Failure%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse\tnode.contents = pptr;\n");

	/* locate the sql record in the table */
	fprintf(h,"\tif ((status = %s_sql_on_search(id, pptr)) != 0)\n",fullname);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNOT FOUND%c) );\n",0x0022,0x0022);

	/* retrieve occi fields */
	fprintf(h,"\tif (!(pptr = %s_collector(optr,cptr,rptr,aptr,pptr)))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 400, %cBad Request Header%c) );\n",0x0022,0x0022);

	/* update the sql record into the table */
	fprintf(h,"\telse if ((status = %s_sql_on_update(id, pptr)) != 0)\n",fullname);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500+status, %cSQL Insert Failure %c) );\n",0x0022,0x0022);

	/* invoke the interface method */
	fprintf(h,"\tif (( iptr ) && (iptr->update)) (*iptr->update)(optr,&node,rptr);\n");

	/* prepare the response header */
	fprintf(h,"\telse if (( status = prepare_%s_response(aptr,id,pptr)) != 0)\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500+status, %cServer Response Failure%c ) );\n",0x0022,0x0022);

	/* check for success and return */
	fprintf(h,"\telse if (!( occi_success( aptr ) ))\n");
	fprintf(h,"\t\treturn( rest_response_status( aptr, 500, %cServer OCCI Failure%c ) );\n",0x0022,0x0022);
	fprintf(h,"\telse\treturn( rest_response_status( aptr, 200, %cOK%c ) );\n",0x0022,0x0022);

	fprintf(h,"}\n");

	title(h,"occi category rest interface method put");
	fprintf(h,"private struct rest_response * %s_put(",fullname);
	fprintf(h,"void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_response * aptr;\n");
	fprintf(h,"\tstruct rest_header   * hptr;\n");
	fprintf(h,"\tstruct occi_category * optr;\n");
	fprintf(h,"\tchar * ctptr;\n");

	fprintf(h,"\tchar * mptr;\n");
	fprintf(h,"\tif (!( hptr = rest_resolve_header( rptr->first, %cContent-Type%c ) ))\n",0x0022,0x0022);
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

	/* ------------------- */
	/* generate delete all */
	/* ------------------- */
	title(h,"occi category rest interface method delete all");
	fprintf(h,"private struct rest_response * %s_delete_all(",C.name);
	fprintf(h,"struct occi_category * optr, struct rest_client * cptr, struct rest_request * rptr,struct rest_response * aptr)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tint\tstatus=0;\n");
	fprintf(h,"\tstruct %s_occi_filter filter;\n",C.name);

	/* collect the selection criteria filter */
	fprintf(h,"\tif (!( filter_%s_info(&filter, optr, rptr, aptr ) ))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 400, %cBad Request%c ) );\n",
		0x0022,0x0022);

	/* delete records from the table */
	fprintf(h,"\tstatus = %s_sql_on_delete_list(&filter);\n",fullname);
	fprintf(h,"\tliberate_%s(filter.attributes);\n", C.name);
	fprintf(h,"\tif ( status != 0 )\n");
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNOT FOUND%c) );\n",0x0022,0x0022);

	/* check for success and return */
	fprintf(h,"\telse if (!( occi_success( aptr ) ))\n");
	fprintf(h,"\t\treturn( rest_response_status( aptr, 500, %cServer OCCI Failure%c ) );\n",0x0022,0x0022);
	fprintf(h,"\telse\treturn( rest_response_status( aptr, 200, %cOK%c ) );\n",0x0022,0x0022);

	fprintf(h,"}\n");

	/* -------------------- */
	/* generate delete item */
	/* -------------------- */
	title(h,"occi category rest interface method delete item");
	fprintf(h,"private struct rest_response * %s_delete_item(",C.name);
	fprintf(h,"struct occi_category * optr, struct rest_client * cptr, struct rest_request * rptr,struct rest_response * aptr, char * id)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tstruct occi_interface * iptr;\n");
	fprintf(h,"\tint    status=0;\n");
	fprintf(h,"\tstruct occi_%s_node node;\n",C.klass);
	fprintf(h,"\tmemset(&node,0,sizeof(struct occi_%s_node));\n",C.klass);
	fprintf(h,"\tiptr = optr->callback;\n");

	/* retrieve occi fields */
	fprintf(h,"\tif (!( pptr = allocate_%s()))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500, %cAllocation Failure%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse\tnode.contents = pptr;\n");

	/* locate the sql record in the table */
	fprintf(h,"\tif ((status = %s_sql_on_search(id, pptr)) != 0)\n",fullname);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNOT FOUND%c) );\n",0x0022,0x0022);

	/* invoke the interface method */
	fprintf(h,"\tif (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,&node,rptr);\n");

	/* delete the sql record from table */
	fprintf(h,"\tif ((status = %s_sql_on_delete(id)) != 0)\n",fullname);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNOT FOUND%c) );\n",0x0022,0x0022);

	/* check for success and return */
	fprintf(h,"\telse if (!( occi_success( aptr ) ))\n");
	fprintf(h,"\t\treturn( rest_response_status( aptr, 500, %cServer OCCI Failure%c ) );\n",0x0022,0x0022);
	fprintf(h,"\telse\treturn( rest_response_status( aptr, 200, %cOK%c ) );\n",0x0022,0x0022);

	fprintf(h,"}\n");

	/* --------------- */
	/* generate delete */
	/* --------------- */
	title(h,"occi category rest interface method delete");
	fprintf(h,"private struct rest_response * %s_delete(",fullname);
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

	fprintf(h,"private void	redirect_%s_mt( struct rest_interface * iptr )\n{\n",fullname);
		fprintf(h,"\tiptr->get = %s_get;\n",fullname);
		fprintf(h,"\tiptr->post = %s_post;\n",fullname);
		fprintf(h,"\tiptr->put = %s_put;\n",fullname);
		fprintf(h,"\tiptr->delete = %s_delete;\n",fullname);
		fprintf(h,"\tiptr->head = %s_head;\n",fullname);
	fprintf(h,"\treturn;\n}\n");

	/* ---------------- */
	/* generate builder */
	/* ---------------- */
	title(h,"occi category builder");
	fprintf(h,"/* occi category rest instance builder for : %s */\n",fullname);
	fprintf(h,"public struct occi_category * %s_builder(char * a,char * b)\n{\n",fullname);
	fprintf(h,"\tchar * c=%c%s%c;\n",0x0022,C.scheme,0x0022);
	fprintf(h,"\tchar * d=%c%s%c;\n",0x0022,C.klass,0x0022);
	fprintf(h,"\tchar * e=%c%s%c;\n",0x0022,C.rel,0x0022);
	fprintf(h,"\tchar * f=%cCompatibleOne OCCI resource %s%c;\n",0x0022,C.name,0x0022);
	fprintf(h,"\tstruct occi_category * optr;\n");
	generate_file_description(h);

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
	fprintf(h,"\telse if (!( %s_table = initialise_occi_sql_table(%c%s%c,description) ))\n",C.name,0x0022,C.name,0X0022);
	fprintf(h,"\t\treturn(liberate_occi_category(optr));\n");

	/* -------------------------------------- */
	/* process the sql crud interface methods */
	/* -------------------------------------- */
	fprintf(h,"\telse\n\t{\n\t\tredirect_%s_mt(optr->interface);\n",fullname);

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
	// fprintf(h,"private struct rest_response * %s_get(",fullname);
	// fprintf(h,"private struct rest_response * %s_head(",fullname);
	// fprintf(h,"private struct rest_response * %s_post(",fullname);
	// fprintf(h,"private struct rest_response * %s_put(",fullname);
	// fprintf(h,"private struct rest_response * %s_delete(",fullname);
	// fprintf(h,"private void	redirect_%s_mt( struct rest_interface * iptr )\n{\n",fullname);
	// fprintf(h,"private struct rest_response * post_action_%s(",C.name);
	// fprintf(h,"private struct rest_response * get_action_%s(",C.name);
	// fprintf(h,"private struct rest_response * delete_action_%s(",C.name);
	// fprintf(h,"private struct rest_response * put_action_%s(",C.name);

	/* ------------- */
#endif	/* _strukt_sql_c */
	/* ------------- */

