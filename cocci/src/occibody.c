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

#ifndef	_occi_body_c
#define	_occi_body_c

#include "occibody.h"

/*	------------------------------------------------------------	*/
/*		o c c i _ c o n s u m e _ h e a d e r 			*/
/*	------------------------------------------------------------	*/
private	struct rest_header * occi_consume_header( struct rest_header * hptr )
{
	struct	rest_header * previous;
	struct	rest_header * next;
	if ((previous = hptr->previous) != (struct rest_header *) 0)
		previous->next = hptr->next;
	if (( next = hptr->next) != (struct rest_header *)0)
		next->previous = next;
	liberate_rest_header( hptr );
	return( next );
}

/*	------------------------------------------------------------	*/
/*		o c c i _ c o n t e n t _ l e n g t h 			*/
/*	------------------------------------------------------------	*/
private	char *	occi_content_length( struct rest_header * hptr, char * filename )
{
	struct	stat info;
	char	length[64];
	if ( stat( filename,&info ) < 0 )
		return( filename );
	else
	{
		sprintf(length,"%u",info.st_size);
		rest_replace_header( hptr, length );
		return( filename );
	}
}	

/*	------------------------------------------------------------	*/
/*	  		o c c i _ j s o n _ b o d y 			*/
/*	------------------------------------------------------------	*/
private	char *	occi_json_body( 
		struct occi_category * cptr,
		struct rest_header  * hptr )
{
	FILE *	h;
	char *	filename;
	char		buffer[2048];
	char *	vptr;
	int		attributs=0;
	struct	rest_header * contentlength=(struct rest_header *) 0;
	if (!( filename = rest_temporary_filename( "json" ) ))
		return( filename );

	else if (!( h = fopen(filename,"w")))
	{
		return(liberate(filename));
	}
	else
	{
		fprintf(h,"{ %c%s%c : ",0x0022,cptr->id,0x0022 );
		while ( hptr )
		{
			if (!( strcasecmp( hptr->name, _HTTP_CONTENT_TYPE ) ))
			{
				rest_replace_header( hptr, _OCCI_MIME_JSON );
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_LENGTH ) ))
			{
				contentlength = hptr;
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _OCCI_ATTRIBUTE ) ))
			{
				strcpy((vptr = buffer),hptr->value);
				while ( *vptr )
				{
					if ( *vptr == '=' )
					{
						*(vptr++) = 0;
						break;
					}
					else	vptr++;
				}
				if (!( attributs++ ))
					fprintf(h,"{\n");
				else	fprintf(h,",\n");
				fprintf(h,"\t%c%s%c : %c%s%c",0x0022,buffer,0x0022,0x0022,vptr,0x0022);
				hptr = occi_consume_header( hptr );
			}
			else	hptr = hptr->next;
		}
		if ( attributs )
			fprintf(h,"\t}\n");
		else	fprintf(h,"%c%c\n",0x0022,0x0022);
		fprintf(h,"}\n");

		fclose(h);
		return( occi_content_length(contentlength, filename ));
	}
}

/*	------------------------------------------------------------	*/
/*		   	o c c i _ t e x t _ b o d y 			*/
/*	------------------------------------------------------------	*/
private	char *	occi_text_body( 
		struct occi_category * cptr,
		struct rest_header  * hptr )
{
	FILE *	h;
	struct	rest_header * contentlength=(struct rest_header *) 0;
	char *	filename;
	char		buffer[2048];
	char *	vptr;
	int		attributs=0;

	if (!( filename = rest_temporary_filename( "txt" ) ))
		return((char *) 0);

	else if (!( h = fopen(filename,"w") ))
		return( liberate( filename ) );

	else
	{
		fprintf(h,"{ Category: %c%s%c; ",0x0022,cptr->id,0x0022 );
		while ( hptr )
		{
			if (!( strcasecmp( hptr->name, _HTTP_CONTENT_TYPE ) ))
			{
				rest_replace_header( hptr, _OCCI_MIME_JSON );
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_LENGTH ) ))
			{
				contentlength = hptr;
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _OCCI_ATTRIBUTE ) ))
			{
				fprintf(h,"%s: %s\n",hptr->name,hptr->value);
				hptr = occi_consume_header( hptr );
			}
			else	hptr = hptr->next;
		}
		if ( attributs )
			fprintf(h,"\t}\n");
		else	fprintf(h,"%c%c\n",0x0022,0x0022);
		fprintf(h,"}\n");

		fclose(h);
		return( occi_content_length(contentlength, filename ));
	}
}

/*	------------------------------------------------------------	*/
/*		   	o c c i _ p h p _ b o d y 			*/
/*	------------------------------------------------------------	*/
private	char *	occi_php_body( 
		struct occi_category * cptr,
		struct rest_header  * hptr )
{
	FILE *	h;
	struct	rest_header * contentlength=(struct rest_header *) 0;
	char *	filename;
	char		buffer[2048];
	char *	vptr;
	int		attributs=0;
	if (!( filename = rest_temporary_filename( "php" ) ))
		return( filename );

	else if (!( h = fopen(filename,"w")))
	{
		return( liberate( filename ) );
	}
	else
	{
		fprintf(h,"<?php\n\%c%s%c => array(",0x0022,cptr->id,0x0022);
		while ( hptr )
		{
			if (!( strcasecmp( hptr->name, _HTTP_CONTENT_TYPE ) ))
			{
				rest_replace_header( hptr, _OCCI_MIME_JSON );
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_LENGTH ) ))
			{
				contentlength = hptr;
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _OCCI_ATTRIBUTE ) ))
			{
				strcpy((vptr = buffer),hptr->value);
				while ( *vptr )
				{
					if ( *vptr == '=' )
					{
						*(vptr++) = 0;
						break;
					}
					else	vptr++;
				}
				if ( attributs++ )
					fprintf(h,",\n");
				fprintf(h,"\t%c%s%c => %c%s%c",0x0022,buffer,0x0022,0x0022,vptr,0x0022);
				hptr = occi_consume_header( hptr );
			}
			else	hptr = hptr->next;
		}
		fprintf(h," );\n");
		fprintf(h,"?>");

		fclose(h);
		return( occi_content_length(contentlength, filename ));
	}
}

/*	------------------------------------------------------------	*/
/*	     		o c c i _ x m l _ b o d y 			*/
/*	------------------------------------------------------------	*/
private	char * 	occi_xml_body( 
		struct occi_category * cptr,
		struct rest_header  * hptr )
{
	FILE *	h;
	struct	rest_header * contentlength=(struct rest_header *) 0;
	char *	filename;
	char		buffer[2048];
	char *	vptr;
	int		elements=0;
	int		attributs=0;
	if (!( filename = rest_temporary_filename( "xml" ) ))
		return( filename );

	else if (!( h = fopen(filename,"w")))
	{
		return(liberate( filename ) );
	}
	else
	{
		fprintf(h,"<?xml version='1.0' encoding='UTF-8'?>\n");

		while ( hptr )
		{
			if (!( strcasecmp( hptr->name, _HTTP_CONTENT_TYPE ) ))
			{
				rest_replace_header( hptr, _OCCI_MIME_XML );
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_LENGTH ) ))
			{
				
				contentlength = hptr;
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _OCCI_LOCATION  ) ))
			{

				if (!( elements++ ))
					fprintf(h,"<%s>\n",cptr->id);
				fprintf(h,"<location href=%c%s%c/>\n",0x0022,hptr->value,0x0022);
				hptr = occi_consume_header( hptr );

			}
			else if (!( strcasecmp( hptr->name, _OCCI_ATTRIBUTE ) ))
			{
				strcpy((vptr = buffer),hptr->value);
				while ( *vptr )
				{
					if ( *vptr == '=' )
					{
						*(vptr++) = 0;
						break;
					}
					else	vptr++;
				}
				if (!( attributs++ ))
					fprintf(h,"<%s\n",cptr->id);
				fprintf(h,"\t%s=%c%s%c\n",buffer,0x0022,vptr,0x0022);
				hptr = occi_consume_header( hptr );
			}
			else	hptr = hptr->next;
		}
		if ( elements )
			fprintf(h,"</%s>\n",cptr->id);
		else if ( attributs )
			fprintf(h," />\n");
		fclose(h);
		return( occi_content_length(contentlength, filename ));
	}
}

/*	---------------------------------------------------	*/
/*		o c c i _ r e s p o n s e _ b o d y  		*/
/*	---------------------------------------------------	*/
public	char * occi_response_body( char * accepts, struct occi_category * cptr, struct rest_header * hptr )
{
	if (!( strcasecmp( accepts, _OCCI_TEXT_OCCI ) ))
		return( occi_text_body( cptr, hptr ) );
	else if ((!( strcasecmp( accepts, _OCCI_OCCI_PHP ) ))
	     ||  (!( strcasecmp( accepts, _OCCI_APP_PHP  ) ))
	     ||  (!( strcasecmp( accepts, _OCCI_TEXT_PHP ) )))
		return( occi_php_body(  cptr, hptr ) );
	else if ((!( strcasecmp( accepts, _OCCI_OCCI_JSON ) ))
	     ||  (!( strcasecmp( accepts, _OCCI_APP_JSON  ) ))
	     ||  (!( strcasecmp( accepts, _OCCI_TEXT_JSON ) )))
		return( occi_json_body(  cptr, hptr ) );
	else if ((!( strcasecmp( accepts, _OCCI_MIME_XML  ) ))
	     ||  (!( strcasecmp( accepts, _OCCI_APP_XML   ) ))
	     ||  (!( strcasecmp( accepts, _OCCI_TEXT_XML  ) )))
		return( occi_xml_body(  cptr, hptr  ) );
	else	return((char *) 0);
}

	/* ---------- */
#endif	/* _occi_body */
	/* ---------- */

