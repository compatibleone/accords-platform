/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* (C) 2011 by Iain James Marshall <ijm667@hotmail.com>				*/
/* ---------------------------------------------------------------------------- */
/*										*/
/* This is free software; you can redistribute it and/or modify it		*/
/* under the terms of the GNU Lesser General Public License as			*/
/* published by the Free Software Foundation; either version 2.1 of		*/
/* the License, or (at your option) any later version.				*/
/*										*/
/* This software is distributed in the hope that it will be useful,		*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU		*/
/* Lesser General Public License for more details.				*/
/*										*/
/* You should have received a copy of the GNU Lesser General Public		*/
/* License along with this software; if not, write to the Free			*/
/* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA		*/
/* 02110-1301 USA, or see the FSF site: http://www.fsf.org.			*/
/*										*/
/* ---------------------------------------------------------------------------- */
#ifndef	_occirest_c
#define	_occirest_c

#include <stdio.h>
#include <string.h>
#include "item.h"
#include "strukt.h"

extern	struct	strukt_context C;

#define	_CRUD_DELETE

void	generate_occi_rest_builder( FILE * h, char * nptr )
{
	char	* prefix;
	int	item=0;
	char	buffer[512];
	struct	item * iptr;

	sprintf(buffer,"%s_%s",nptr,C.name);

	title(h,buffer );

	/* ------------------ */
	/* generate structure */
	/* ------------------ */

	title(h,"occi category management structure");
	fprintf(h,"struct %s * allocate_%s();\n",C.name,C.name);
	fprintf(h,"struct %s * liberate_%s(struct %s * optr);\n",C.name,C.name,C.name);

	fprintf(h,"private pthread_mutex_t list_%s_control=PTHREAD_MUTEX_INITIALIZER;\n",C.name);
	fprintf(h,"private struct occi_%s_node * %s_first = (struct occi_%s_node *) 0;\n",C.klass,C.name,C.klass);
	fprintf(h,"private struct occi_%s_node * %s_last  = (struct occi_%s_node *) 0;\n",C.klass,C.name,C.klass);
	switch ( C.genrest )
	{
	case	_OCCI_LINK :
		fprintf(h,"public struct  occi_%s_node * occi_first_%s_node() { return( %s_first ); }\n",
			C.klass,C.klass,C.name);
		fprintf(h,"public struct  occi_%s_node * occi_last_%s_node() { return( %s_last ); }\n",
			C.klass,C.klass,C.name);
		break;
	case	_OCCI_KIND :
		fprintf(h,"public struct  occi_%s_node * occi_first_%s_node() { return( %s_first ); }\n",
			C.klass,C.name,C.name);
		fprintf(h,"public struct  occi_%s_node * occi_last_%s_node() { return( %s_last ); }\n",
			C.klass,C.name,C.name);
		break;

	}
#ifdef	_need_specific_node_control


	fprintf(h,"\nstruct %s_node {\n",C.name);
	fprintf(h,"\tstruct %s_node * previous;\n",C.name);
	fprintf(h,"\tstruct %s_node * next;\n",C.name);
	fprintf(h,"\tstruct %s * contents;\n",C.name);
	switch ( C.genrest )
	{
	case	_OCCI_KIND	:
		fprintf(h,"\tstruct occi_mixin * firstmixin;\n");
		fprintf(h,"\tstruct occi_mixin * lastmixin;\n");
		fprintf(h,"\tstruct occi_link * firstlink;\n");
		fprintf(h,"\tstruct occi_link * lastlink;\n");
		break;
	case	_OCCI_LINK	:
		fprintf(h,"\tchar * source;\n");
		fprintf(h,"\tchar * target;\n");
	}
	fprintf(h,"\t};\n");


	/* ---------------------- */
	/* generate liberate node */
	/* ---------------------- */
	title(h,"occi category liberate node");
	fprintf(h,"private struct %s_node * liberate_%s_node(struct %s_node * nptr) {\n",
		C.name,C.name,C.name);
	switch ( C.genrest )
	{
	case	_OCCI_KIND	:
		fprintf(h,"\tstruct occi_mixin * mptr;\n");
		fprintf(h,"\tstruct occi_link * lptr;\n");
	}

	fprintf(h,"\tif ( nptr ) {\n");
	switch ( C.genrest )
	{
	case	_OCCI_KIND	:
		fprintf(h,"\t\twhile ((mptr = nptr->firstmixin) != (struct occi_mixin*) 0) {\n");
		fprintf(h,"\t\t\tnptr->firstmixin = mptr->next;\n");
		fprintf(h,"\t\t\tmptr = liberate_occi_mixin( mptr );\n");
		fprintf(h,"\t\t\t}\n");
		fprintf(h,"\t\twhile ((lptr = nptr->firstlink) != (struct occi_link*) 0) {\n");
		fprintf(h,"\t\t\tnptr->firstlink = lptr->next;\n");
		fprintf(h,"\t\t\tlptr = liberate_occi_link( lptr );\n");
		fprintf(h,"\t\t\t}\n");
		break;
	case	_OCCI_LINK	:
		fprintf(h,"\t\tif ( nptr->source )\n");
		fprintf(h,"\t\t\tnptr->source = liberate( nptr->source );\n");
		fprintf(h,"\t\tif ( nptr->target )\n");
		fprintf(h,"\t\t\tnptr->target = liberate( nptr->target );\n");
		break;
	}
	fprintf(h,"\t\tif ( nptr->contents )\n");
	fprintf(h,"\t\t\tliberate_%s( nptr->contents );\n",C.name);
	fprintf(h,"\t\tliberate( nptr );\n");
	fprintf(h,"\t\t}\n");
	fprintf(h,"}\n");

#endif	 /* node code exclusion */

	/* ------------------ */
	/* generate drop node */
	/* ------------------ */
	title(h,"occi category drop node");
	fprintf(h,"private struct occi_%s_node * ll_drop_%s_node(struct occi_%s_node * nptr) {\n",
		C.klass,C.name,C.klass);
	fprintf(h,"\tif ( nptr ) {\n");
	fprintf(h,"\tif (!( nptr->previous ))\n");
	fprintf(h,"\t\t%s_first = nptr->next;\n",C.name);
	fprintf(h,"\telse\tnptr->previous->next = nptr->next;\n");
	fprintf(h,"\tif (!( nptr->next ))\n");
	fprintf(h,"\t\t%s_last = nptr->previous;\n",C.name);
	fprintf(h,"\telse\tnptr->next->previous = nptr->previous;\n");
	fprintf(h,"\t\tliberate_occi_%s_node( nptr );\n",C.klass);
	fprintf(h,"\t\t}\n");
	fprintf(h,"\treturn((struct occi_%s_node *)0);\n",C.klass);
	fprintf(h,"}\n");

	fprintf(h,"private struct occi_%s_node * drop_%s_node(struct occi_%s_node * nptr) {\n",
		C.klass,C.name,C.klass);
	fprintf(h,"\tpthread_mutex_lock( &list_%s_control );\n",C.name);
	fprintf(h,"\tnptr = ll_drop_%s_node( nptr );\n",C.name);
	fprintf(h,"\tpthread_mutex_unlock( &list_%s_control );\n",C.name);
	fprintf(h,"\treturn(nptr);\n");
	fprintf(h,"}\n");



	/* -------------------- */
	/* generate locate node */
	/* -------------------- */
	title(h,"occi category locate node");
	fprintf(h,"private struct occi_%s_node * ll_locate_%s_node(char * id) {\n",C.klass,C.name);
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tfor ( nptr = %s_first;\n",C.name);
	fprintf(h,"\t\tnptr != (struct occi_%s_node *) 0;\n",C.klass);
	fprintf(h,"\t\tnptr = nptr->next ) {\n");
	fprintf(h,"\t\tif (!( pptr = nptr->contents )) continue;\n");
	fprintf(h,"\t\telse if (!( pptr->id )) continue;\n");
	fprintf(h,"\t\telse if (!( strcmp(pptr->id,id) )) break;\n");
	fprintf(h,"\t\t}\n");
	fprintf(h,"\treturn( nptr );\n");
	fprintf(h,"}\n");

	fprintf(h,"private struct occi_%s_node * locate_%s_node(char * id) {\n",C.klass,C.name);
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tpthread_mutex_lock( &list_%s_control );\n",C.name);
	fprintf(h,"\tnptr = ll_locate_%s_node(id);\n",C.name);
	fprintf(h,"\tpthread_mutex_unlock( &list_%s_control );\n",C.name);
	fprintf(h,"\treturn( nptr );\n");
	fprintf(h,"}\n");

	/* ----------------- */
	/* generate add node */
	/* ----------------- */
	title(h,"occi category add node");
	fprintf(h,"private struct occi_%s_node * ll_add_%s_node(int mode) {\n",C.klass,C.name);
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tif (!( nptr = allocate_occi_%s_node() ))\n",C.klass);
	fprintf(h,"\t\treturn( nptr );\n");
	fprintf(h,"\telse\t{\n");
	fprintf(h,"\t\tif (!( nptr->contents = allocate_%s()))\n",C.name);
	fprintf(h,"\t\t\treturn( liberate_occi_%s_node(nptr) );\n",C.klass);
	fprintf(h,"\t\tif (!( pptr = nptr->contents ))\n");
	fprintf(h,"\t\t\treturn( liberate_occi_%s_node(nptr) );\n",C.klass);
	fprintf(h,"\t\telse if (( mode != 0 ) && (!( pptr->id = occi_allocate_uuid())))\n");
	fprintf(h,"\t\t\treturn( liberate_occi_%s_node(nptr) );\n",C.klass);
	fprintf(h,"\t\telse\t{\n");
	fprintf(h,"\t\t\tif (!( nptr->previous = %s_last ))\n",C.name);
	fprintf(h,"\t\t\t\t%s_first = nptr;\n",C.name);
	fprintf(h,"\t\t\telse\tnptr->previous->next = nptr;\n");
	fprintf(h,"\t\t\t%s_last = nptr;\n",C.name);
	fprintf(h,"\t\t\treturn( nptr );\n");
	fprintf(h,"\t\t\t}\n");
	fprintf(h,"\t\t}\n");
	fprintf(h,"}\n");

	fprintf(h,"private struct occi_%s_node * add_%s_node(int mode) {\n",
		C.klass,C.name);
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tpthread_mutex_lock( &list_%s_control );\n",C.name);
	fprintf(h,"\tnptr = ll_add_%s_node( mode );\n",C.name);
	fprintf(h,"\tpthread_mutex_unlock( &list_%s_control );\n",C.name);
	fprintf(h,"\treturn(nptr);\n");
	fprintf(h,"}\n");


	/* ----------------- */
	/* generate autoload */
	/* ----------------- */
	title(h,"occi category rest interface method auto load");
	fprintf(h,"private char*autosave_%s_name=%c%s.xml%c;\n",C.name,0x0022,C.name,0x0022);
	fprintf(h,"private void autoload_%s_nodes() {\n",C.name,C.name);
	fprintf(h,"\tchar * fn=autosave_%s_name;",C.name);
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tstruct xml_element * document;\n");
	fprintf(h,"\tstruct xml_element * eptr;\n");
	fprintf(h,"\tstruct xml_element * vptr;\n");
	fprintf(h,"\tstruct xml_atribut  * aptr;\n");

	fprintf(h,"\tif (!( document = document_parse_file(fn)))\n\t\treturn;\n");
	fprintf(h,"\tif ((eptr = document_element(document,%c%ss%c)) != (struct xml_element *) 0) {\n",
		0x0022,C.name,0x0022);
	fprintf(h,"\t\tfor (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {\n");

	fprintf(h,"\t\t\tif (!( vptr->name )) continue;\n");
	fprintf(h,"\t\t\telse if ( strcmp( vptr->name, %c%s%c ) ) continue;\n",
		0x0022,C.name,0x0022);
	fprintf(h,"\t\t\telse if (!( nptr = add_%s_node(0))) break;\n",C.name);
	fprintf(h,"\t\t\telse if (!( pptr = nptr->contents )) break;\n");

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
			fprintf(h,"\t\t\tif ((aptr = document_atribut( vptr, ");
			fprintf(h,"%c%s%c )) != (struct xml_atribut *) 0)\n",0x0022,iptr->name,0x0022);
			if ( iptr->indirection )
				fprintf(h,"\t\t\t\tpptr->%s = document_atribut_string(aptr);\n",iptr->name);
			else	
				fprintf(h,"\t\t\t\tpptr->%s = document_atribut_value(aptr);\n",iptr->name);
		}
	}

	fprintf(h,"\t\t\t}\n");
	fprintf(h,"\t\t}\n");
	fprintf(h,"\tdocument = document_drop( document );\n" );
	fprintf(h,"\treturn;\n");
	fprintf(h,"}\n");


	/* ----------------- */
	/* generate autosave */
	/* ----------------- */
	title(h,"occi category rest interface method auto save");

	fprintf(h,"public  void set_autosave_%s_name(char * fn) {\n",C.name);
	fprintf(h,"\tautosave_%s_name = fn;",C.name);
	fprintf(h,"\treturn;\n");
	fprintf(h,"}\n");

	fprintf(h,"public  void autosave_%s_nodes() {\n",C.name);
	fprintf(h,"\tchar * fn=autosave_%s_name;",C.name);
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tFILE * h;\n");
	fprintf(h,"\tpthread_mutex_lock( &list_%s_control );\n",C.name);
	fprintf(h,"\tif (( h = fopen(fn,%cw%c)) != (FILE *) 0) {\n",0x0022,0x0022);
	fprintf(h,"\tfprintf(h,%c<%ss>%cn%c);\n",0x0022,C.name,0x005C,0x0022);
	fprintf(h,"\tfor ( nptr = %s_first;\n",C.name);
	fprintf(h,"\t\tnptr != (struct occi_%s_node *) 0;\n",C.klass);
	fprintf(h,"\t\tnptr = nptr->next ) {\n");
	fprintf(h,"\t\tif (!( pptr = nptr->contents )) continue;\n");
	fprintf(h,"\t\tfprintf(h,%c<%s%cn%c);\n",0x0022,C.name,0x005C,0x0022);

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
			fprintf(h,"\t\tfprintf(h,%c %s=%cc%c,0x0022);\n",
				0x0022,iptr->name,0x0025,0x0022);
			if ( iptr->indirection )
			{
				fprintf(h, "\t\tfprintf_xml_string_attribute(h, \"%%s\", (pptr->%s?pptr->%s:\"\"));\n",
					iptr->name, iptr->name
				);
			}
			else
			{
				fprintf(h,"\t\tfprintf(h,%c%cu%c,pptr->%s);\n",
					0x0022,0x0025,0x0022,iptr->name);
			}
			fprintf(h,"\t\tfprintf(h,%c%cc%c,0x0022);\n",
				0x0022,0x0025,0x0022);
		}
	}


	fprintf(h,"\t\tfprintf(h,%c />%cn%c);\n",0x0022,0x005C,0x0022);
	fprintf(h,"\t\t}\n");
	fprintf(h,"\tfprintf(h,%c</%ss>%cn%c);\n",0x0022,C.name,0x005C,0x0022);
	fprintf(h,"\tfclose(h);\n");
	fprintf(h,"\t}\n");
	fprintf(h,"\tpthread_mutex_unlock( &list_%s_control );\n",C.name);
	fprintf(h,"\treturn;\n");
	fprintf(h,"}\n");

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
		if ( iptr->indirection )
			fprintf(h,"\t\t\tpptr->%s = allocate_string(value);\n",iptr->name);
		else	fprintf(h,"\t\t\tpptr->%s = atoi(value);\n",iptr->name);
	}
	fprintf(h,"\t\t}\n");
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
	

	/* ----------------- */
	/* generate get item */
	/* ----------------- */
	title(h,"occi category rest interface method get item");
	fprintf(h,"private struct rest_response * %s_get_item(\n",C.name);
	fprintf(h,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
	fprintf(h,"\tstruct rest_request * rptr, struct rest_response * aptr, char * id)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_header * hptr;\n");
	fprintf(h,"\tstruct occi_interface * iptr;\n");
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);

	fprintf(h,"\tiptr = optr->callback;\n");

	fprintf(h,"\tif (!( nptr = locate_%s_node(id)))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNot Found%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse if (!( pptr = nptr->contents ))\n");
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNot Found%c) );\n",0x0022,0x0022);
	fprintf(h,"\tif (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr,rptr);\n");
	fprintf(h,"\tautosave_%s_nodes();\n",C.name);
	fprintf(h,"\treturn( %s_occi_response(optr,cptr,rptr,aptr,pptr));\n",C.name);
	fprintf(h,"}\n");


	/* ------------------ */
	/* generate post link */
	/* ------------------ */
	title(h,"occi category rest interface method post link");
	fprintf(h,"private struct rest_response * %s_post_link(\n",C.name);
	fprintf(h,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
	fprintf(h,"\tstruct rest_request * rptr, struct rest_response * aptr,char * id)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_header * hptr;\n");
	fprintf(h,"\tstruct occi_interface * iptr;\n");
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tchar * reqhost;\n");
	fprintf(h,"\tif (!( nptr = locate_%s_node(id)))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNot Found%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse if (!( pptr = nptr->contents ))\n");
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNot Found%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse\treturn( rest_html_response( aptr, 400, %cBad Request%c) );\n",0x0022,0x0022);
	fprintf(h,"}\n");

	/* ------------------- */
	/* generate post mixin */
	/* ------------------- */
	switch( C.genrest )
	{
	case	_OCCI_KIND	:
	title(h,"occi category rest interface method post mixin");
	fprintf(h,"private struct rest_response * %s_post_mixin(\n",C.name);
	fprintf(h,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
	fprintf(h,"\tstruct rest_request * rptr, struct rest_response * aptr,char * id)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_header * hptr;\n");
	fprintf(h,"\tstruct occi_interface * iptr;\n");
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tchar * reqhost;\n");
	fprintf(h,"\tif (!( nptr = locate_%s_node(id)))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNot Found%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse if (!( pptr = nptr->contents ))\n");
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNot Found%c) );\n",0x0022,0x0022);

	fprintf(h,"\telse\treturn( rest_html_response( aptr, 400, %cBad Request%c));\n",0x0022,0x0022);
	fprintf(h,"}\n");
	}

	/* -------------------- */
	/* generate post action */
	/* -------------------- */
	switch( C.genrest )
	{
	case	_OCCI_KIND	:
	title(h,"occi category rest interface method post action");
	fprintf(h,"private struct rest_response * %s_post_action(\n",C.name);
	fprintf(h,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
	fprintf(h,"\tstruct rest_request * rptr, struct rest_response * aptr,char * id)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_header * hptr;\n");
	fprintf(h,"\tstruct occi_interface * iptr;\n");
	fprintf(h,"\tstruct occi_action * fptr;\n");
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tchar * reqhost;\n");
	fprintf(h,"\tchar * mptr;\n");
	fprintf(h,"\tif (!( nptr = locate_%s_node(id)))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNot Found%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse if (!( pptr = nptr->contents ))\n");
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNot Found%c) );\n",0x0022,0x0022);
	fprintf(h,"\tmptr = (rptr->parameters+strlen(%caction=%c));\n",0x0022,0x0022);
	fprintf(h,"\tfor ( fptr=optr->firstact;\n");
	fprintf(h,"\t\tfptr != (struct occi_action *) 0;\n");
	fprintf(h,"\t\tfptr = fptr->next )\n");
	fprintf(h,"\t\tif (!( strncmp( mptr, fptr->name, strlen( fptr->name )) ))\n");
	fprintf(h,"\t\t\treturn( occi_invoke_action(fptr,optr,cptr,rptr,aptr,pptr) );\n");
	fprintf(h,"\treturn( rest_html_response( aptr, 400, %cIncorrect Action Request%c));\n",
		0x0022,0x0022);
	fprintf(h,"}\n");
	}

	/* ------------------ */
	/* generate post item */
	/* ------------------ */
	title(h,"occi category rest interface method post item");
	fprintf(h,"private struct rest_response * %s_post_item(\n",C.name);
	fprintf(h,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
	fprintf(h,"\tstruct rest_request * rptr, struct rest_response * aptr)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_header * hptr;\n");
	fprintf(h,"\tstruct occi_interface * iptr;\n");
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tchar * reqhost;\n");
	fprintf(h,"\tint    reqport=0;\n");

	fprintf(h,"\tiptr = optr->callback;\n");

	fprintf(h,"\tif (!( reqhost = rest_request_host( rptr ) ))\n");
	fprintf(h,"\t\treturn( rest_html_response( aptr, 400, %cBad Request%c ) );\n",
		0x0022,0x0022);
	fprintf(h,"\telse reqport = rptr->port;\n");

	fprintf(h,"\tif (!( nptr = add_%s_node(1)))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500, %cServer Failure%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse if (!( pptr = nptr->contents ))\n");
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500, %cServer Failure%c) );\n",0x0022,0x0022);

	fprintf(h,"\tif (!( occi_process_atributs( optr, rptr,aptr, pptr, set_%s_field ) ))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500, %cServer Failure%c) );\n",0x0022,0x0022);
	
	fprintf(h,"\tif (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr,rptr);\n");

	fprintf(h,"\tautosave_%s_nodes();\n",C.name);

	fprintf(h,"\tsprintf(cptr->buffer,%c%cs:%cu%cs%cs%c,reqhost,reqport,optr->location,pptr->id);\n",
			0x0022,0x0025,0x0025,0x0025,0x0025,0x0022);
	fprintf(h,"\tif (!( hptr = rest_response_header( aptr, %cX-OCCI-Location%c,cptr->buffer) ))\n",
			0x0022,0x0022);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500, %cServer Failure%c ) );\n",
			0x0022,0x0022);

	fprintf(h,"\telse if (!( occi_success( aptr ) ))\n");
	fprintf(h,"\t\treturn( rest_response_status( aptr, 500, %cServer Failure%c ) );\n",
		0x0022,0x0022);
	fprintf(h,"\telse\treturn( rest_response_status( aptr, 200, %cOK%c ) );\n",
		0x0022,0x0022);
	
	fprintf(h,"}\n");

	/* ----------------- */
	/* generate put item */
	/* ----------------- */
	title(h,"occi category rest interface method put item");
	fprintf(h,"private struct rest_response * %s_put_item(\n",C.name);
	fprintf(h,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
	fprintf(h,"\tstruct rest_request * rptr, struct rest_response * aptr,char * id)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_header * hptr;\n");
	fprintf(h,"\tstruct occi_interface * iptr;\n");
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tiptr = optr->callback;\n");
	fprintf(h,"\tif (!( nptr = locate_%s_node(id)))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNot Found%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse if (!( pptr = nptr->contents ))\n");
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNot Found%c) );\n",0x0022,0x0022);
	fprintf(h,"\tif (!( occi_process_atributs(optr,rptr,aptr, pptr, set_%s_field ) ))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 500, %cServer Failure%c) );\n",0x0022,0x0022);
	fprintf(h,"\tif (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr,rptr);\n");
	fprintf(h,"\tautosave_%s_nodes();\n",C.name);
	fprintf(h,"\treturn( %s_occi_response(optr,cptr,rptr,aptr,pptr));\n",C.name);
	fprintf(h,"}\n");

	/* ------------------ */
	/* generate head item */
	/* ------------------ */
	title(h,"occi category rest interface method head item");
	fprintf(h,"private struct rest_response * %s_head_item(\n",C.name);
	fprintf(h,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
	fprintf(h,"\tstruct rest_request * rptr, struct rest_response * aptr,char * id)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_header * hptr;\n");
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tif (!( nptr = locate_%s_node(id)))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNot Found%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse if (!( pptr = nptr->contents ))\n");
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNot Found%c) );\n",0x0022,0x0022);
	fprintf(h,"\telse\treturn( rest_html_response( aptr, 400, %cBad Request%c) );\n",0x0022,0x0022);
	fprintf(h,"}\n");

	/* -------------------- */
	/* generate delete item */
	/* -------------------- */
	title(h,"occi category rest interface method delete item");
	fprintf(h,"private struct rest_response * %s_delete_item(\n",C.name);
	fprintf(h,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
	fprintf(h,"\tstruct rest_request * rptr, struct rest_response * aptr, char * id)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_header * hptr;\n");
	fprintf(h,"\tstruct occi_interface * iptr;\n");
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tiptr = optr->callback;\n");
	fprintf(h,"\tif (!( nptr = locate_%s_node(id)))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 404, %cNot Found%c) );\n",0x0022,0x0022);
	fprintf(h,"\tif (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr,rptr);\n");
	fprintf(h,"\tdrop_%s_node( nptr );\n",C.name);
	fprintf(h,"\tautosave_%s_nodes();\n",C.name);
	fprintf(h,"\tif (!( occi_success( aptr ) ))\n");
	fprintf(h,"\t\treturn( rest_response_status( aptr, 500, %cServer Failure%c ) );\n",
		0x0022,0x0022);
	fprintf(h,"\telse\treturn( rest_response_status( aptr, 200, %cOK%c ) );\n",
		0x0022,0x0022);
	fprintf(h,"}\n");

	/* ----------------- */
	/* generate get list */
	/* ----------------- */
	title(h,"occi category rest interface method get list");
	fprintf(h,"private struct rest_response * %s_get_list(\n",C.name);
	fprintf(h,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
	fprintf(h,"\tstruct rest_request * rptr, struct rest_response * aptr)\n");
	fprintf(h,"{\n");

	fprintf(h,"\tstruct rest_header * hptr;\n");
	fprintf(h,"\tstruct occi_%s_node * sptr;\n",C.klass);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tstruct %s_occi_filter filter;\n",C.name);

	fprintf(h,"\tchar * reqhost;\n");
	fprintf(h,"\tint reqport=0;\n");

	fprintf(h,"\tif (!( reqhost = rest_request_host( rptr ) ))\n");
	fprintf(h,"\t\treturn( rest_html_response( aptr, 400, %cBad Request%c ) );\n",
		0x0022,0x0022);
	fprintf(h,"\telse reqport = rptr->port;\n");
	fprintf(h,"\tif (!( filter_%s_info(&filter, optr, rptr, aptr ) ))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 400, %cBad Request%c ) );\n",
		0x0022,0x0022);
	fprintf(h,"\tfor ( sptr = %s_first;\n",C.name);
	fprintf(h,"\t\tsptr != (struct occi_%s_node *) 0;\n",C.klass);
	fprintf(h,"\t\tsptr = sptr->next ) {\n");

	fprintf(h,"\t\tif (!( pptr = sptr->contents ))\n");
	fprintf(h,"\t\t\tcontinue;\n");
	fprintf(h,"\t\tif (!( pass_%s_filter( pptr, &filter) ))\n",C.name);
	fprintf(h,"\t\t\tcontinue;\n");
	fprintf(h,"\t\tsprintf(cptr->buffer,%c%cs:%cu%cs%cs%c,reqhost,reqport,optr->location,pptr->id);\n",
			0x0022,0x0025,0x0025,0x0025,0x0025,0x0022);
	fprintf(h,"\t\tif (!( hptr = rest_response_header( aptr, %cX-OCCI-Location%c,cptr->buffer) )) {\n",
			0x0022,0x0022);
	fprintf(h,"\t\t\tliberate_%s(filter.attributes);\n", C.name);
	fprintf(h,"\t\t\treturn( rest_html_response( aptr, 500, %cServer Failure%c ) );\n",
			0x0022,0x0022);
	fprintf(h,"\t\t}\n");
	fprintf(h,"\t}\n");
	fprintf(h,"\tliberate_%s(filter.attributes);\n", C.name);

	fprintf(h,"\tif (!( occi_success( aptr ) ))\n");
	fprintf(h,"\t\treturn( rest_response_status( aptr, 500, %cServer Failure%c ) );\n",
		0x0022,0x0022);
	fprintf(h,"\telse\treturn( rest_response_status( aptr, 200, %cOK%c ) );\n",
		0x0022,0x0022);
	
	fprintf(h,"}\n");

	/* ------------------- */
	/* generate delete all */
	/* -------------------- */
	title(h,"occi category rest interface method delete all");
	fprintf(h,"private struct rest_response * %s_delete_all(\n",C.name);
	fprintf(h,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
	fprintf(h,"\tstruct rest_request * rptr, struct rest_response * aptr)\n");
	fprintf(h,"{\n");
	fprintf(h,"\tstruct rest_header * hptr;\n");
	fprintf(h,"\tstruct occi_interface * iptr;\n");
	fprintf(h,"\tstruct occi_%s_node * nptr;\n",C.klass);
	fprintf(h,"\tstruct occi_%s_node * sptr;\n",C.klass);
	fprintf(h,"\tstruct %s * pptr;\n",C.name);
	fprintf(h,"\tstruct %s_occi_filter filter;\n",C.name);
	fprintf(h,"\tiptr = optr->callback;\n");
	fprintf(h,"\tif (!( filter_%s_info(&filter, optr, rptr, aptr ) ))\n",C.name);
	fprintf(h,"\t\treturn( rest_html_response( aptr, 400, %cBad Request%c ) );\n",
		0x0022,0x0022);
	fprintf(h,"\tnptr=%s_first;\n",C.name);
	fprintf(h,"\twhile (nptr != (struct occi_%s_node *) 0) {\n",C.klass);
	fprintf(h,"\t\tif ((!( pptr = nptr->contents ))\n");
	fprintf(h,"\t\t||  (!( pass_%s_filter( pptr, &filter) ))) {\n",C.name);
	fprintf(h,"\t\t\tnptr = nptr->next;\n");
	fprintf(h,"\t\t\tcontinue;\n");
	fprintf(h,"\t\t\t}\n");
	fprintf(h,"\t\telse\t{\n");
	fprintf(h,"\t\t\tif (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr,rptr); }\n");
	fprintf(h,"\t\t\tsptr = nptr->next;\n");
	fprintf(h,"\t\t\tdrop_%s_node( nptr );\n",C.name);
	fprintf(h,"\t\t\tnptr = sptr;\n");
	fprintf(h,"\t\t}\n");
	fprintf(h,"\t}\n");
	fprintf(h,"\tliberate_%s(filter.attributes);\n", C.name);
	fprintf(h,"\tautosave_%s_nodes();\n",C.name);
	fprintf(h,"\tif (!( occi_success( aptr ) ))\n");
	fprintf(h,"\t\treturn( rest_response_status( aptr, 500, %cServer Failure%c ) );\n",
		0x0022,0x0022);
	fprintf(h,"\telse\treturn( rest_response_status( aptr, 200, %cOK%c ) );\n",
		0x0022,0x0022);
	fprintf(h,"}\n");

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

	if ( C.gencrud )
	{
#ifdef	_CRUD_POST
		title(h,"crud post action");
		fprintf(h,"private struct rest_response * post_action_%s(",C.name);
		fprintf(h,"struct occi_category * optr, \n");
		fprintf(h,"struct rest_client * cptr,  \n");
		fprintf(h,"struct rest_request * rptr,  \n");
		fprintf(h,"struct rest_response * aptr,  \n");
		fprintf(h,"void * vptr )\n{\n");
		fprintf(h,"\taptr = liberate_rest_response( aptr );\n");
		fprintf(h,"\treturn( occi_%s_post(optr,cptr,rptr));\n");
		fprintf(h,"}\n");
#endif
#ifdef	_CRUD_GET
		title(h,"crud get action");
		fprintf(h,"private struct rest_response * get_action_%s(",C.name);
		fprintf(h,"struct occi_category * optr, \n");
		fprintf(h,"struct rest_client * cptr,  \n");
		fprintf(h,"struct rest_request * rptr,  \n");
		fprintf(h,"struct rest_response * aptr,  \n");
		fprintf(h,"void * vptr )\n{\n");
		fprintf(h,"\taptr = liberate_rest_response( aptr );\n");
		fprintf(h,"\treturn( occi_%s_get(optr,cptr,rptr));\n");
		fprintf(h,"}\n");
#endif
#ifdef	_CRUD_DELETE
		title(h,"crud delete action");
		fprintf(h,"private struct rest_response * delete_action_%s(",C.name);
		fprintf(h,"struct occi_category * optr, \n");
		fprintf(h,"struct rest_client * cptr,  \n");
		fprintf(h,"struct rest_request * rptr,  \n");
		fprintf(h,"struct rest_response * aptr,  \n");
		fprintf(h,"void * vptr )\n{\n");
		fprintf(h,"\taptr = liberate_rest_response( aptr );\n");
		fprintf(h,"\treturn( occi_%s_delete(optr,cptr,rptr));\n",C.name);
		fprintf(h,"}\n");
#endif
#ifdef	_CRUD_PUT
		title(h,"crud put action");
		fprintf(h,"private struct rest_response * put_action_%s(",C.name);
		fprintf(h,"struct occi_category * optr, \n");
		fprintf(h,"struct rest_client * cptr,  \n");
		fprintf(h,"struct rest_request * rptr,  \n");
		fprintf(h,"struct rest_response * aptr,  \n");
		fprintf(h,"void * vptr )\n{\n");
		fprintf(h,"\taptr = liberate_rest_response( aptr );\n");
		fprintf(h,"\treturn( occi_%s_put(optr,cptr,rptr));\n");
		fprintf(h,"}\n");
#endif
	}

	/* ---------------- */
	/* generate builder */
	/* ---------------- */
	title(h,"occi category builder");
	fprintf(h,"/* occi category rest instance builder for : %s */\n",buffer);
	fprintf(h,"public struct occi_category * %s_builder(char * a,char * b) {\n",buffer);
	/* fprintf(h,"\tchar * b=%c%s%c;\n",0x0022,C.name,0x0022); */
	fprintf(h,"\tchar * c=%c%s%c;\n",0x0022,C.scheme,0x0022);
	fprintf(h,"\tchar * d=%c%s%c;\n",0x0022,C.klass,0x0022);
	fprintf(h,"\tchar * e=%c%s%c;\n",0x0022,C.rel,0x0022);
	fprintf(h,"\tchar * f=%cCompatibleOne OCCI resource %s%c;\n",0x0022,C.name,0x0022);
	fprintf(h,"\tstruct occi_category * optr;\n");
	fprintf(h,"\tif (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }\n");
	fprintf(h,"\telse {\n");

		fprintf(h,"\t\tredirect_%s_mt(optr->interface);\n",buffer);

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
				fprintf(h,"\t\tif (!( optr = occi_add_attribute(optr, %c%s%c,0,0) ))\n",
					0x0022,iptr->name,0x0022);
				fprintf(h,"\t\t\treturn(optr);\n");
			}
		}
		if ( C.gencrud )
		{
#ifdef	_CRUD_POST
			fprintf(h,"\t\tif (!( optr = occi_add_action( optr,%cPOST%c,%c%c,post_action_%s)))\n",
				0x0022,0x0022,0x0022,0x0022, C.name);
			fprintf(h,"\t\t\treturn( optr );\n");
#endif
#ifdef	_CRUD_GET
			fprintf(h,"\t\tif (!( optr = occi_add_action( optr,%cGET%c,%c%c,get_action_%s)))\n",
				0x0022,0x0022,0x0022,0x0022, C.name);
			fprintf(h,"\t\t\treturn( optr );\n");
#endif
#ifdef	_CRUD_DELETE
			fprintf(h,"\t\tif (!( optr = occi_add_action( optr,%cDELETE%c,%c%c,delete_action_%s)))\n",
				0x0022,0x0022,0x0022,0x0022, C.name);
			fprintf(h,"\t\t\treturn( optr );\n");
#endif
#ifdef	_CRUD_PUT
			fprintf(h,"\t\tif (!( optr = occi_add_action( optr,%cPUT%c,%c%c,put_action_%s)))\n",
				0x0022,0x0022,0x0022,0x0022, C.name);
			fprintf(h,"\t\t\treturn( optr );\n");
#endif
		}
		fprintf(h,"\t\tautoload_%s_nodes();\n",C.name,0x0022,C.name,0x0022);
		fprintf(h,"\t\treturn(optr);\n");
	fprintf(h,"\t}\n");
	fprintf(h,"\n}\n");

}

void	generate_occi_rest_request( FILE * h, char * nptr )
{
	char	* prefix;
	int	item=0;
	char	buffer[512];
	struct	item * iptr;
	prefix = "c1";
	sprintf(buffer,"%s_%s",C.name,"occi_headers");
	title(h, buffer );
	fprintf(h,"public struct rest_header *  %s(struct %s * sptr)\n{\n",buffer,C.name);
	fprintf(h,"\tstruct rest_header * first=(struct rest_header *) 0;\n");
	fprintf(h,"\tstruct rest_header * last=(struct rest_header *) 0;\n");
	fprintf(h,"\tstruct rest_header * hptr=(struct rest_header *) 0;\n");
	fprintf(h,"\tchar buffer[8192];\n");
	fprintf(h,"\tif (!( sptr )) return(0);\n");
	fprintf(h,"\tif (!( hptr = allocate_rest_header()))\n");
	fprintf(h,"\t\treturn(hptr);\n");
	fprintf(h,"\t\telse\tif (!( hptr->previous = last))\n");
	fprintf(h,"\t\t\tfirst = hptr;\n");
	fprintf(h,"\t\telse\thptr->previous->next = hptr;\n");
	fprintf(h,"\t\tlast = hptr;\n");
	fprintf(h,"\tif (!( hptr->name = allocate_string(%c%s%c)))\n",
			0x0022,"Category",0x0022);
	fprintf(h,"\t\treturn(first);\n");
	fprintf(h,"\tsprintf(buffer,%c%s; scheme='%s'; class='%s';\\r\\n%c);\n",
		0x0022, C.name, C.scheme, C.klass, 0x0022);
	fprintf(h,"\tif (!( hptr->value = allocate_string(buffer)))\n");
	fprintf(h,"\t\treturn(first);\n");
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
		else if (!( strcmp( iptr->basic,"struct" ) ))
			continue;

		fprintf(h,"\tif (!( hptr = allocate_rest_header()))\n");
		fprintf(h,"\t\treturn(first);\n");
		fprintf(h,"\t\telse\tif (!( hptr->previous = last))\n");
		fprintf(h,"\t\t\tfirst = hptr;\n");
		fprintf(h,"\t\telse\thptr->previous->next = hptr;\n");
		fprintf(h,"\t\tlast = hptr;\n");
		fprintf(h,"\tif (!( hptr->name = allocate_string(%c%s%c)))\n",
			0x0022,"X-OCCI-Attribute",0x0022);
		fprintf(h,"\t\treturn(first);\n");

		if (!( strcmp( iptr->name, "id" ) ))
		{
			fprintf(h,"\tsprintf(buffer,%cocci.core.id='%cs'\\r\\n%c,(sptr->id?sptr->id:%c%c));\n",
				0x0022,0x0025,0x0022,0x0022,0x0022);
		}
		else if ( iptr->indirection )
		{
			fprintf(h,"\tsprintf(buffer,%cocci.%s.%s='%cs'\\r\\n%c,(sptr->%s?sptr->%s:%c%c));\n",
				0x0022,C.name,iptr->name,0x0025,0x0022,iptr->name,iptr->name,0x0022,0x0022);
		}
		else 
		{
			fprintf(h,"\tsprintf(buffer,%cocci.%s.%s='%cu'\\r\\n%c,sptr->%s);\n",
				0x0022,C.name,iptr->name,0x0025,0x0022,iptr->name);
		}

		fprintf(h,"\tif (!( hptr->value = allocate_string(buffer)))\n");
		fprintf(h,"\t\treturn(first);\n");

	}
	fprintf(h,"\treturn(first);\n");
	fprintf(h,"\n}\n");
}


#endif	

