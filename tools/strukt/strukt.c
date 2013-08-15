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

#include <stdio.h>
#include <memory.h>
#include <string.h>
#define	public
#define	private static
#include "allocate.h"
#include "allocate.c"
#include "item.h"
#include "strukt.h"

struct	strukt_context C = 	{ 
		0,		/* schema */
		(char *) 0,	/* license */
		(char *) 0,	/* scheme */
		(char *) 0,	/* klass */
		(char *) 0,	/* rel */
		0,0,0,0,
		0,0,0,0,
		0,0,
		0,0,0,0,
		0,0,
		(FILE *) 0,
		(char *) 0,
		(struct item *) 0,
		(struct item *) 0
		};

#include "item.c"

int	failure(int v, char * m, char * n)
{
	if ( v )
	{
		printf("\n** error : %u : %s : %s ** \n",v,m,n);
	}
	return(v);
}

void	expand( FILE * h, char * sptr )
{
	int	c;
	while ((c = *(sptr++)) != 0)
		fprintf(h,"%c ",c);
	return;
}

void	gener( FILE * h, int c ,int n )
{
	while ( n-- ) fprintf(h,"%c",c);		
	return;
}

void	title( FILE * h, char * sptr )
{
	fprintf(h,"\n/*\t");
	gener( h, '-', strlen( sptr ) * 2 );
	fprintf(h,"\t*/\n");

	fprintf(h,"/*\t");
	expand( h, sptr );
	fprintf(h,"\t*/\n");

	fprintf(h,"/*\t");
	gener( h, '-', strlen( sptr ) * 2 );
	fprintf(h,"\t*/\n");
	return;
}

void	generate_reset( FILE *h, char * nptr )
{
	int	i;
	struct	item * iptr;
	char	buffer[512];
	sprintf(buffer,"%s_%s",nptr,C.name);
	title(h, buffer );
	fprintf(h,"public struct %s * %s(struct %s * sptr)\n{\n",C.name,buffer,C.name);
	fprintf(h,"\tif ( sptr )\n\t{\n");
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( iptr->dimension ))
		{
			fprintf(h,"\t\tsptr->%s = ",iptr->name);
			if ( iptr->indirection )
			{
				fprintf(h,"(");
				if (!( strcmp( iptr->basic,"struct" ) ))
					fprintf(h,"struct %s",iptr->type);
				else	fprintf(h,"%s",iptr->basic);
				for ( i=0;i < iptr->indirection; i++ )
						fprintf(h,"*");
				fprintf(h,") 0;\n");
			}
			else	fprintf(h," 0;\n");
		}
		else	
		{
			fprintf(h,"\t\tmemset(&sptr->%s,0,sizeof(",iptr->name);
			if (!( strcmp( iptr->basic,"struct" ) ))
				fprintf(h,"struct %s",iptr->type);
			else	fprintf(h,"%s",iptr->basic);
			for ( i=0;i < iptr->indirection; i++ )
				fprintf(h,"*");
			fprintf(h,")*%u);\n",iptr->dimension);
		}
	}
	fprintf(h,"\t}\n");
	fprintf(h,"\treturn(sptr);\n");
	fprintf(h,"\n}\n");

}

void	generate_allocate( FILE *h, char * nptr )
{
	int	i;
	struct	item * iptr;
	char	buffer[512];
	sprintf(buffer,"%s_%s",nptr,C.name);
	title(h, buffer );
	fprintf(h,"public struct %s * %s()\n{\n",C.name,buffer);
	fprintf(h,"\tstruct %s * sptr;\n",C.name);
	fprintf(h,"\tif (!( sptr = allocate( sizeof( struct %s ) ) ))\n\t\treturn( sptr );\n",C.name);
	fprintf(h,"\telse\treturn( reset_%s(sptr) );",C.name);
	fprintf(h,"\n}\n");
}

void	generate_add( FILE *h, char * nptr )
{
	int	i;
	struct	item * iptr;
	char	buffer[512];
	sprintf(buffer,"%s_%s",nptr,C.name);
	title(h, buffer );
	fprintf(h,"public struct %s * %s(struct %s * pptr)\n{\n",C.name,buffer,C.parent);
	fprintf(h,"\tstruct %s * sptr;\n",C.name);
	fprintf(h,"\tif (!( sptr = allocate( sizeof( struct %s ) ) ))\n\t\treturn( sptr );\n",C.name);
	fprintf(h,"\telse if (!( sptr = reset_%s(sptr) ))\n\t\treturn( sptr );\n",C.name);
	fprintf(h,"\telse\n\t{\n");
	fprintf(h,"\t\tif (!( sptr->previous = pptr->last ))\n");
	fprintf(h,"\t\t\tpptr->first = sptr;\n");
	fprintf(h,"\t\telse\tsptr->previous->next = sptr;\n");
	fprintf(h,"\t\tpptr->last = sptr;\n");
	fprintf(h,"\t\tsptr->parent = pptr;\n");
	fprintf(h,"\t\treturn( sptr );\n");
	fprintf(h,"\t}\n");
	fprintf(h,"\n}\n");
}

void	generate_drop( FILE *h, char * nptr )
{
	int	i;
	struct	item * iptr;
	char	buffer[512];
	sprintf(buffer,"%s_%s",nptr,C.name);
	title(h, buffer );
	fprintf(h,"public struct %s * %s(struct %s * sptr)\n{\n",C.name,buffer,C.name);
	fprintf(h,"\tif ( sptr )\n\t{\n");
	fprintf(h,"\t\tif (!( sptr->parent )) return(sptr);\n");

		fprintf(h,"\t\tif (!( sptr->previous ))\n");
		fprintf(h,"\t\t{\n");
			fprintf(h,"\t\t\tif (!( sptr->parent->first = sptr->next ))\n");
				fprintf(h,"\t\t\t\tsptr->parent->last = (struct %s *) 0;\n",C.name);
			fprintf(h,"\t\t\telse	sptr->parent->first->previous = (struct %s *) 0;\n",C.name);
		fprintf(h,"\t\t}\n");
		fprintf(h,"\t\telse if (!( sptr->previous->next = sptr->next ))\n");
			fprintf(h,"\t\t\tsptr->parent->last = sptr->previous;\n");

		fprintf(h,"\t\tif (!( sptr->next ))\n"); 
		fprintf(h,"\t\t{\n");
			fprintf(h,"\t\t\tif (!( sptr->parent->last = sptr->previous ))\n");
				fprintf(h,"\t\t\t\tsptr->parent->first = (struct %s *) 0;\n",C.name);
			fprintf(h,"\t\t\telse	sptr->parent->last->next = (struct %s *) 0;\n",C.name);
		fprintf(h,"\t\t}\n");
		fprintf(h,"\t\telse if (!( sptr->next->previous = sptr->previous ))\n");
			fprintf(h,"\t\t\tsptr->parent->first = sptr->next;\n");
		fprintf(h,"\t\tsptr = liberate_%s(sptr);\n",C.name);

	fprintf(h,"\t}\n");
	fprintf(h,"\treturn((struct %s *) 0);",C.name);
	fprintf(h,"\n}\n");
}

void	generate_occi_request2file( FILE * h, char * nptr )
{
	char	* prefix;
	int	item=0;
	char	buffer[512];
	struct	item * iptr;
	prefix = "c1";
	sprintf(buffer,"%s_%s",nptr,C.name);
	title(h, buffer );
	fprintf(h,"public int %s(FILE * fh,struct %s * sptr,char * prefix, char * nptr)\n{\n",buffer,C.name);
	fprintf(h,"\tstruct xml_element * wptr;\n");
	fprintf(h,"\tif (!( sptr )) return(0);\n");
	fprintf(h,"\tfprintf(fh,%cPOST /%cs/ HTTP/1.1\\r\\n%c,nptr);\n",0x0022,0x0025,0X0022);
	fprintf(h,"\tfprintf(fh,%cCategory: %cs; scheme='http://scheme.%cs.org/occi/%cs#'; class='kind';\\r\\n%c,nptr,prefix,prefix);\n",
		0x0022, 0x0025, 0x0025,0x0025,0x0022);
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( strcmp( iptr->basic,"struct" ) ))
			continue;
		else if ( iptr->indirection )
		{
			fprintf(h,"\tfprintf(fh,%cX-OCCI-Attribute: %cs.%cs.%s='%cs'\\r\\n%c,prefix,nptr,(sptr->%s?sptr->%s:%c%c));\n",
				0x0022,0x0025,0x0025,iptr->name,0x0025,0x0022,iptr->name,iptr->name,0x0022,0x0022);
		}
		else 
		{
			fprintf(h,"\tfprintf(fh,%cX-OCCI-Attribute: %cs.%cs.%s='%cu'\\r\\n%c,prefix,nptr,sptr->%s);\n",
				0x0022,0x0025,0x0025,iptr->name,0x0025,0x0022,iptr->name);
		}
	}
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"\n}\n");
}

void	generate_interpret_xml( FILE * h, char * nptr )
{
	char	* prefix;
	int	item=0;
	char	buffer[512];
	struct	item * iptr;
	sprintf(buffer,"%s_%s",nptr,C.name);
	title(h, buffer );
	fprintf(h,"public int %s(struct %s * sptr,struct xml_element * eptr)\n{\n",buffer,C.name);
	fprintf(h,"\tstruct xml_element * wptr;\n");
	fprintf(h,"\tif (!( eptr )) return(0);\n");
	fprintf(h,"\tif (!( sptr )) return(0);\n");
	fprintf(h,"\tfor ( wptr=eptr->first; wptr != (struct xml_element *) 0; wptr=wptr->next)\n\t{\n");
	prefix = "\0";
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if (!( strcmp( iptr->basic,"struct" ) ))
			continue;
		fprintf(h,"\t\t%sif (!( strcmp(wptr->name,%c%s%c) ))\n",prefix,0x0022,iptr->name,0x0022);
		prefix = "else ";
		fprintf(h,"\t\t{\n");
		if ( iptr->indirection )
		{
			fprintf(h,"\t\t\tif ( wptr->value ) { sptr->%s = allocate_string(wptr->value); }\n",iptr->name);
		}
		else
		{
			fprintf(h,"\t\t\tif ( wptr->value ) { sptr->%s = atoi(wptr->value); }\n",iptr->name);
		}
		fprintf(h,"\t\t}\n");
	}
	fprintf(h,"\t}\n");
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"\n}\n");
}




void	generate_liberate( FILE *h, char * nptr )
{
	char	buffer[512];
	struct	item * iptr;
	sprintf(buffer,"%s_%s",nptr,C.name);
	title(h, buffer );
	fprintf(h,"public struct %s * %s(struct %s * sptr)\n{\n",C.name,buffer,C.name);
	if ( C.hasfirst ) 
	{
		fprintf(h,"\tstruct %s * mptr;\n",C.premier);
	}
	fprintf(h,"\tif ( sptr )\n\t{\n");
	for ( 	iptr= C.first;
		iptr != (struct item *) 0;
		iptr = iptr->next )
	{
		if ( C.hasfirst ) 
		{
			if (!( strcmp( iptr->name, "last" ) ))
				continue;
			if (!( strcmp( iptr->name, "first" ) ))
			{
				fprintf(h,"\t\twhile ((mptr = sptr->first) != (struct %s *) 0)\n\t\t{\n",C.premier);
				fprintf(h,"\t\t\tsptr->first = mptr->next;\n");
				fprintf(h,"\t\t\tmptr = liberate_%s( mptr );\n\t\t}\n",C.premier);
				continue;
			}
		}
		if (!( strcmp( iptr->name, "parent" ) ))
			continue;
		else if (!( strcmp( iptr->basic, "void" ) ))
			continue;
		else if ( iptr->indirection == 1 )
		{
			if (!( strcmp( iptr->name, "previous" )))
				continue;
			else if (!( strcmp( iptr->name, "next" )))
				continue;
			fprintf(h,"\t\tif ( sptr->%s )\n\t\t\t sptr->%s = liberate",iptr->name,iptr->name);
			if (!( strcmp( iptr->basic, "struct" ) ))
				fprintf(h,"_%s",iptr->type);
			fprintf(h,"(sptr->%s);\n",iptr->name);
		}
	}
	fprintf(h,"\t\tsptr = liberate( sptr );\n");
	fprintf(h,"\t}\n");
	fprintf(h,"\treturn((struct %s *) 0);\n",C.name);
	fprintf(h,"\n}\n");
}

int	generate()
{
	struct	item * iptr;
	if ( C.schema )
	{
		fprintf(C.target,"\n<xsd:complexType name=%c%s%c>\n",0x0022,C.name,0x0022);
		for ( 	iptr= C.first;
			iptr != (struct item *) 0;
			iptr = iptr->next )
		{
			fprintf(C.target,"\t<xsd:attribute name=%c%s%c",0x0022,iptr->name,0x0022);
			if (!( strcmp( iptr->basic , "int" ) ))
				fprintf(C.target," type=%c%s%c",0x0022,"xsd:integer");
			else	fprintf(C.target," type=%c%s%c",0x0022,"xsd:string");
			fprintf(C.target,"/>\n");
		}
		fprintf(C.target,"</xsd:complexType>\n");
	}
	else if ( C.genrest )
	{
		switch ( C.genrest )
		{
		case	_OCCI_KIND	:
			C.klass  = "kind";
			C.scheme = "http://scheme.compatibleone.fr/scheme/compatible#";
			C.rel    = "http://scheme.ogf.org/occi/resource#";
			break;
		case	_OCCI_LINK	:
			C.klass  = "link";
			C.scheme = "http://scheme.compatibleone.fr/scheme/compatible#";
			C.rel    = "http://scheme.ogf.org/occi/link#";
			break;
		case	_OCCI_ACTION	:
			C.klass  = "action";
			C.scheme = "http://scheme.compatibleone.fr/scheme/compatible#";
			C.rel    = "http://scheme.ogf.org/occi/mixin#";
			break;
		case	_OCCI_MIXIN	:
			C.klass  = "mixin";
			C.scheme = "http://scheme.compatibleone.fr/scheme/compatible#";
			C.rel    = "http://scheme.ogf.org/occi/action#";
			break;
		default	:
			return( failure(30,"incorrect","cords value") );
		}
		if ( C.gensql )
		{
			generate_occi_sql_builder( C.target, "occi" );
		}
		else
		{
			generate_occi_rest_builder( C.target, "occi" );
			generate_occi_rest_request( C.target, "rest_occi" );
		}
	}
	else
	{
		generate_liberate( C.target, "liberate" );
		generate_reset( C.target, "reset" );
		generate_allocate( C.target, "allocate" );
		if (( C.hasparent ) 
		&&  ( C.hasprevious )
		&&  ( C.hasnext ))
		{
			generate_add( C.target, "add" );
			generate_drop( C.target, "drop" );
		}
		if ( C.genxml )
		{
			generate_interpret_xml( C.target, "xmlin" );
		}
		if ( C.genocci )
		{
			generate_occi_request2file( C.target, "rest_occi" );
		}
	}
	return(0);
}

int	is_punctuation(int c)
{
	if (( c >= 0x0021 ) && ( c <= 0x002F ))
		return(1);
	else if (( c >= 0x003a ) && ( c <= 0x003f ))
		return(1);
	else if (( c >= 0x005b) && ( c <= 0x005e ))
		return(1);
	else if (( c >= 0x007b) && ( c <= 0x007f ))
		return(1);
	else	return(0);
}

int	ungetch(int c)
{
	return((C.ungotc = c));
}

int	getch(FILE *h)
{
	int	c;
	if ((c = C.ungotc) != 0)
		C.ungotc = 0;
	else if ((c = fgetc(h)) == -1)
		c = 0;
	else if ( C.echo )
		printf("%c",c);
	return( c );
}

int	is_white( int c )
{
	switch ( c )
	{
	case	' '	:
	case	'\r'	:
	case	'\t'	:
	case	'\n'	: return( c );
	default		: return( 0 );
	}
}

int	remove_white_space(FILE * h)
{
	int	c;
	while (1)
	{
		switch ((c = getch(h)))
		{
		case	0x23	:
			while (((c = getch(h)) != 0) && ( c != '\n'));
			if ( c  == '\n' )
				continue;
		case	0	:
		case	26	: 
			return(0);
		default	:
			if ( is_white( c ) )
				continue;
			else	return(ungetch(c));
		}
	}

}

int	get_token( FILE * h, char * token, int tlen )
{
	int	c;
	int	i=0;
	int	quoting=0;
	while ( i < tlen )
	{
		if (!(c = getch(h)))
			break;
		else if ( c == '"' )
		{
			quoting = c;
			continue;
		}
		else if ( c == quoting ) 
		{
			quoting = 0;
			break;
		}
		else if ( quoting )
		{
			*(token+i) = c;
			i++;
			continue;
		}
		else if ( is_white( c ) )
		{
			ungetch( c );
			break;
		}
		else if ( is_punctuation( c ) )
		{
			ungetch( c );
			break;
		}
		else
		{
			*(token+i) = c;
			i++;
			continue;
		}
	}
	*(token+i) = 0;
	return(i);
}

int	add_member()
{
	struct	item * iptr;
	if (!( iptr = allocate_item()))
		return( failure( 27,"allocation","item") );
	else if (!( iptr->name = allocate_string( C.member )))
		return( failure( 27,"allocation","item->name") );
	else if (!( iptr->basic = allocate_string( C.basic )))
		return( failure( 27,"allocation","item->basic") );
	else 	
	{
		if (!( strcmp( C.basic, "struct" ) ))
			if (!( iptr->type = allocate_string( C.special )))
				return( failure( 27,"allocation","item->type") );
		iptr->dimension = C.dimension;
		iptr->indirection = C.isptr;
		C.isptr = 0;
		C.dimension=0;
		if (!( iptr->previous = C.last ))
			C.first = iptr;
		else	iptr->previous->next = iptr;
		C.last = iptr;
		if (!( strcmp( C.basic, "struct" ) )) 
		{
			if (!( strcmp(C.member, "first" ) ))
			{
				C.hasfirst=1;
				strcpy( C.premier, C.special );
			}
			else if (!( strcmp(C.member, "parent" ) ))
			{
				C.hasparent=1;
				strcpy( C.parent, C.special );
			}
			else if (!( strcmp(C.member, "previous" ) ))
			{
				C.hasprevious=1;
			}
			else if (!( strcmp(C.member, "next" ) ))
			{
				C.hasnext=1;
			}
		}
		return( 0 );
	}
}

int	handle_punctuation( FILE * sh, int c )
{
	int	status;
	char 	b[2];
	switch ( C.state )
	{
	case	0 :	/* awaiting struct */
	case	1 :	/* awaiting struct name */
		break;
	case	2 :
		if ( c == '{' )
		{
			C.state = C.next;
			return( 0 );
		}
		else	break;

	case	3 :	/* awaiting type keyword */
		if ( c == '}' )
		{
			C.state = 9;
			return(0);
		}
		else	break;
	case	4 :	/* awaiting extended type */
	case	5 :	/* awaiting name or pointer */
		if ( c == '*' )
		{
			C.isptr++;
			return(0);
		}
		break;
	case	6 :	/* awaiting colon or dimension */
		if ( c == ';' )
		{
			if (!( status = add_member() ))
			{
				C.state = C.next;
				return( 0 );
			}
			else	break;
		}
		else if ( c == '[' )
		{
			C.state = 7;
			return( 0 );
		}
		else	break;
	case	8 :	/* awaiting closure of dimension */
		if ( c == ']' )
		{
			C.state = 6;
			return(0);
		}
		else	break;
	case	9 :	/* awaiting colon or dimension */
		if ( c == ';' )
		{
			C.state = 0;
			return(generate());
		}
	}
	b[0] = c;
	b[1] = 0;
	return(failure(30,"incorrect punctuation",b));
}

char *	is_basic_type(char * tptr)
{
	if (!(strcmp( tptr,"struct")))
		return( "struct" );
	else if (!(strcmp( tptr,"float")))
		return( "float" );
	else if (!(strcmp( tptr,"double")))
		return( "double" );
	else if (!(strcmp( tptr,"char")))
		return( "char" );
	else if (!(strcmp( tptr,"int")))
		return( "int" );
	else if (!(strcmp( tptr,"short")))
		return( "short" );
	else if (!(strcmp( tptr,"long")))
		return( "long" );
	else if (!(strcmp( tptr,"void")))
		return( "void" );
	else	return( (char *) 0);
}

int	handle_token( FILE * sh, char * tptr )
{
	switch ( C.state )
	{
	case	0 :	/* awaiting struct */
		if (!( strcmp( tptr, "struct" ) ))
		{
			C.state = 1;
			return(0);
		}
		else	break;
	case	1 :	/* awaiting struct name */
		strcpy( C.name, tptr );
		C.next = 3;
		C.state = 2;
		return(0);
	case	2 :	/* awaiting opener */
		break;
	case	3 :	/* awaiting type */
		if ((C.basic = is_basic_type( tptr )) != (char *) 0)
		{
			if (!(strcmp( C.basic,"struct")))
				C.state = 4;
			else	C.state = 5;
			return(0);
		}
		else	break;

	case 	4 :	/* awaiting type name */
		strcpy(C.special,tptr);
		C.state = 5;
		return(0);
	case 	5 :	/* awaiting name */
		strcpy(C.member,tptr);
		C.state = 6;
		return(0);
	case	6 :	/* awaiting semi or dimensions */
		break;
	case	7 :	/* awaiting dimension */
		C.dimension = atoi( tptr );
		C.state = 8;
		return(0);
	}
	return( failure( 30 , "incorrect token", tptr ) );
}


void	file_symbol( FILE * h, char * sptr )
{
	int	c;
	fprintf(h,"_");
	while ((c = *(sptr++)) != 0)
	{
		if ( is_punctuation( c ) )
			fprintf(h,"_");
		else	fprintf(h,"%c",c);
	}
	fprintf(h,"_");
	return;
}

void	prepend_license( FILE * target, char * filename )
{
	int	c;
	FILE * h;
	if ((h = fopen( filename, "r")) != (FILE *) 0)
	{
		while ((c = fgetc(h)) > 0)
			fputc(c,target);
		fclose(h);
	}
	return;
}

void	schema_header( FILE * h, char * nptr, char * iptr)
{
	fprintf(h,"\n<xsd:sxhema xmlns:xsd=%chttp://www.w3c.org/2001/XMLSchema%c>\n",0x0022,0x0022);
	return;
}

void	schema_footer( FILE * h, char * nptr )
{
	fprintf(h,"\n</xsd:sxhema>\n");
	return;
}

void	file_header( FILE * h, char * nptr, char * iptr, const char *filter_name)
{
	char	buffer[1024];
	if ( C.genrest )
		sprintf(buffer,"occi%s",nptr);
	else	strcpy(buffer,nptr);
	if ( C.license ) { prepend_license( h, C.license ); }
	fprintf(h,"/* STRUKT WARNING : this file has been generated and should not be modified by hand */\n");
	fprintf(h,"#ifndef ");
	file_symbol( h, buffer );
	fprintf(h,"\n");
	fprintf(h,"#define ");
	file_symbol( h, buffer );
	fprintf(h,"\n");
	if ( C.genxml )
	{
		fprintf(h,"\n#include %c%s%c\n",0x0022,"element.h",0x0022);
	}
	fprintf(h,"\n#include %c%s%c\n",0x0022,iptr,0x0022);
	fprintf(h,"#include %c%s%c\n",0x0022, filter_name, 0x0022);
	return;
}

void	file_footer( FILE * h, char * nptr)
{
	char	buffer[1024];
	if ( C.genrest )
		sprintf(buffer,"occi%s",nptr);
	else	strcpy(buffer,nptr);
	fprintf(h,"\n#endif\t/* ");
	file_symbol( h, buffer );
	fprintf(h," */\n");
	return;
}

int	process( char * struct_name )
{
	int	status;
	int	c;
	int	l;
	char	occi_header_filename[512];
	char	filter_filename[512];
	char	tn[512];
	char 	token[512];
	FILE * sh;
	sprintf(occi_header_filename,"%s.h",struct_name);
	sprintf(filter_filename, "%s_occi_filter.h", struct_name);
	sprintf(tn,"%s.c", struct_name);
	if (!( sh = fopen( occi_header_filename, "r" )))
		return( failure(40,"file not found",occi_header_filename) );
	else if (!( C.target = fopen( tn, "w" )))
	{
		fclose(sh);
		return( failure(46,"creating file",tn) );
	}
	else
	{
		file_header( C.target, tn, occi_header_filename, filter_filename);
		while ((c = remove_white_space( sh )))
		{
			if ( is_punctuation(c) )
			{
				if (!(status = handle_punctuation(sh,getch(sh))))
					continue;
				else	break;
			}
			else if (!( l = get_token(sh,token,512) ))
				break;
			else if (!( status = handle_token(sh,token) ))
				continue;
			else	break;
		}

		file_footer( C.target, tn );
		fclose(C.target);
		fclose(sh);
	}		
	return( 0 );
}

int	schema( char * nptr )
{
	int	status;
	int	c;
	int	l;
	char	sn[512];
	char	tn[512];
	char 	token[512];
	FILE * sh;
	sprintf(sn,"%s.h",nptr);
	sprintf(tn,"%s.xsd",nptr);
	if (!( sh = fopen( sn, "r" )))
		return( failure(40,"file not found",sn) );
	else if (!( C.target = fopen( tn, "w" )))
	{
		fclose(sh);
		return( failure(46,"creating file",tn) );
	}
	else
	{
		schema_header( C.target, tn, sn );
		while ((c = remove_white_space( sh )))
		{
			if ( is_punctuation(c) )
			{
				if (!(status = handle_punctuation(sh,getch(sh))))
					continue;
				else	break;
			}
			else if (!( l = get_token(sh,token,512) ))
				break;
			else if (!( status = handle_token(sh,token) ))
				continue;
			else	break;
		}

		schema_footer( C.target, tn );
		fclose(C.target);
		fclose(sh);
	}		
	return( 0 );
}

private	int	banner()
{
	printf("\n Structure Constructor : Version 3.1a.0.0.2");
	printf("\n Provisoire du 05/01/2013");
	printf("\n Copyright (c) 2013 Iain James Marshall\n");
	printf("\n Options : \n");
	printf("\n --verbose                 activate verbose messages ");
	printf("\n --debug                   activate debug   messages ");
	printf("\n --echo                    echo source during input  ");
	printf("\n --xml                     generate xml production   ");
	printf("\n --occi                    generate occi production  ");
	printf("\n --mysql                   generate for mysql engine ");
	printf("\n --crud                    generate crud actions     ");
	printf("\n --schema                  generate cords schema     ");
	printf("\n --cords <1:k|2:l|3:a|4:m> cords entity object type  ");
	printf("\n\n");
	return(0);
}

int	main(int argc, char * argv[])
{
	int	status=0;
	int	argi=1;
	char *	aptr;
	if ( argc == 1 )
		return( banner() );

	else	
	{
		while ( argi < argc )
		{
			if (!( aptr = argv[argi++] ))
				break;
			else if ( *aptr == '-' )
			{
				switch ( *(++aptr) )
				{
				case	'-'	:
					aptr++;
					if (!( strcmp( aptr, "cords" ) ))
					{
						if (!( aptr = argv[argi++] ))
							return(failure( 30,"cords","expected parameter" ));
						else if (!( strcmp( aptr,"kind" )))
							C.genrest = _OCCI_KIND;
						else if (!( strcmp( aptr,"link" )))
							C.genrest = _OCCI_LINK;
						else if (!( strcmp( aptr,"action" )))
							C.genrest = _OCCI_ACTION;
						else if (!( strcmp( aptr,"mixin" )))
							C.genrest = _OCCI_KIND;
						else return(failure( 30,"cords","incorrect parameter" ));
					}
					else if (!( strcmp( aptr, "schema" ) ))
						C.schema = 1;
					else if (!( strcmp( aptr, "license" ) ))
						C.license = argv[argi++];
					else if (!( strcmp( aptr, "echo" ) ))
						C.echo = 1;
					else if (!( strcmp( aptr, "verbose" ) ))
						C.verbose = 1;
					else if (!( strcmp( aptr, "xml" ) ))
						C.genxml = 1;
					else if (!( strcmp( aptr, "mysql" ) ))
						C.gensql = 1;
					else if (!( strcmp( aptr, "occi" ) ))
						C.genocci = 1;
					else if (!( strcmp( aptr, "crud" ) ))
						C.gencrud = 1;
					continue;
				case	'l'	: C.license = aptr; continue;
				case 	'r'	: C.genrest = 1; continue;
				case 	'o'	: C.genocci = 1; continue;
				case	'c'	: C.gencrud = 1; continue;
				case 	'x'	: C.genxml = 1; continue;
				case	'e'	: C.echo=1; continue;
				case	'v'	: C.verbose=1; continue;
				default		: continue;
				}
			}
			else if ( C.schema )
			{
				if ((status = schema( aptr )) != 0)
					break;
				else	continue;
			}
			else if (( status = process( aptr )) != 0)
				break;
		}
	
	}
	return(0);
}


