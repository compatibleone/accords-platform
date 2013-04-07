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
#ifndef	_occi_body_c
#define	_occi_body_c

#include "occibody.h"
#include "xlink.h"
/*	------------------------------------------------------------	*/
/*		o c c i _ c o n s u m e _ h e a d e r 			*/
/*	------------------------------------------------------------	*/
private	struct rest_header * occi_consume_header( struct rest_header * hptr )
{
	struct	rest_header * previous;
	struct	rest_header * next;
	if ( hptr->name )
		hptr->name = liberate( hptr->name );
	return( hptr->next );
}

/*	------------------------------------------------------------	*/
/*		o c c i _ c o n t e n t _ l e n g t h 			*/
/*	------------------------------------------------------------	*/
public	char *	occi_content_length( struct rest_header * hptr, char * filename )
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
/*	this function generates the new style JSON OCCI body message	*/
/*	------------------------------------------------------------	*/
private	char *	occi_json_body( 
		struct occi_category * cptr,
		struct rest_header  * hptr )
{
	FILE *	h;
	char *	filename;
	char	buffer[2048];
	char *	vptr;
	char *	nptr;
	int	attributs=0;
	int	locations=0;
	struct	rest_header * contentlength=(struct rest_header *) 0;
	if (!( filename = rest_temporary_filename( "json" ) ))
		return( filename );

	else if (!( h = fopen(filename,"w")))
	{
		return(liberate(filename));
	}
	else
	{
		while ( hptr )
		{
			if (!( hptr->name ))
				hptr = hptr->next;
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_TYPE ) ))
			{
				rest_replace_header( hptr, _OCCI_OCCI_JSON );
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_LENGTH ) ))
			{
				contentlength = hptr;
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _OCCI_LOCATION ) ))
			{
				if (!( locations++ ))
				{
					fprintf(h,"{ %c%s%c : { %c%s%c : { %clocation%c : [\n",
						0x0022,cptr->domain,0x0022, 
						0x0022,cptr->id,0x0022,
						0x0022, 0x0022 );
				}
				else	fprintf(h,",\n");
				fprintf(h,"\t%c%s%c",0x0022,hptr->value,0x0022);
				hptr = occi_consume_header( hptr );
			}
			else if (!( strcasecmp( hptr->name, _OCCI_ATTRIBUTE ) ))
			{
				if (!( attributs++ ))
				{
					fprintf(h,"{ %c%s%c : { %c%s%c : { \n",
						0x0022,cptr->domain,0x0022, 
						0x0022,cptr->id,0x0022 );
				}
				else	fprintf(h,",\n");
				strcpy((nptr = vptr = buffer),hptr->value);
				while ( *vptr )
				{
					if ( *vptr == '=' )
					{
						*(vptr++) = 0;
						break;
					}
					else if ( *(vptr++) == '.' )
						nptr = vptr;
				}
				fprintf(h,"\t%c%s%c : %c%s%c",0x0022,nptr,0x0022,0x0022,vptr,0x0022);
				hptr = occi_consume_header( hptr );
			}
			else	hptr = hptr->next;
		}
		if ( attributs )
			fprintf(h,"\t}\n");
		else if ( locations )
			fprintf(h,"\t] } \n");
		fprintf(h,"}\n}\n");
		fclose(h);
		return( occi_content_length(contentlength, filename ));
	}
}

/*	------------------------------------------------------------	*/
/*	  	   o c c i _ o l d _ j s o n _ b o d y 			*/
/*	------------------------------------------------------------	*/
/*	this function generates an old format JSON OCCI body message	*/
/*	------------------------------------------------------------	*/
private	char *	occi_old_json_body( 
		struct occi_category * cptr,
		struct rest_header  * hptr )
{
	FILE *	h;
	char *	filename;
	char	buffer[2048];
	char *	vptr;
	char *	nptr;
	int	attributs=0;
	int	locations=0;
	struct	rest_header * contentlength=(struct rest_header *) 0;
	if (!( filename = rest_temporary_filename( "json" ) ))
		return( filename );

	else if (!( h = fopen(filename,"w")))
	{
		return(liberate(filename));
	}
	else
	{
		fprintf(h,"{\n");
		while ( hptr )
		{
			if (!( hptr->name ))
				hptr = hptr->next;
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_TYPE ) ))
			{
				rest_replace_header( hptr, _OCCI_OLD_JSON );
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_LENGTH ) ))
			{
				contentlength = hptr;
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _OCCI_LOCATION ) ))
			{
				if (!( locations++ ))
					fprintf(h," %clocation%c : [\n",0x0022,0x0022);
				else	fprintf(h,",\n");
				fprintf(h,"\t%c%s%c",0x0022,hptr->value,0x0022);
				hptr = occi_consume_header( hptr );
			}
			else if (!( strcasecmp( hptr->name, _OCCI_ATTRIBUTE ) ))
			{
				if (!( attributs++ ))
				{
					fprintf(h,"%ckind%c : {\n",0x0022,0x0022);
					fprintf(h,"\t%cterm%c : %c%s%c,\n",0x0022,0x0022,0x0022,cptr->id,0x0022);
					fprintf(h,"\t%cscheme%c : %c%s%c,\n",0x0022,0x0022,0x0022,cptr->scheme,0x0022);
					fprintf(h,"\t%cclass%c : %c%s%c\n\t},\n",0x0022,0x0022,0x0022,"kind",0x0022);
					fprintf(h,"\t%cattributes%c : {\n",0x0022,0x0022 );
				}
				else	fprintf(h,",\n");
				strcpy((nptr = vptr = buffer),hptr->value);
				while ( *vptr )
				{
					if ( *vptr == '=' )
					{
						*(vptr++) = 0;
						break;
					}
					else if ( *(vptr++) == '.' )
						nptr = vptr;
				}
				fprintf(h,"\t%c%s.%s.%s%c : %c%s%c",0x0022,cptr->domain,cptr->id,nptr,0x0022,0x0022,vptr,0x0022);
				hptr = occi_consume_header( hptr );
			}
			else	hptr = hptr->next;
		}

		if ( attributs )
			fprintf(h,"\t}\n");
		else if ( locations )
			fprintf(h,"\t]\n");

		fprintf(h,"}\n");		

		fclose(h);
		return( occi_content_length(contentlength, filename ));
	}
}

/*	------------------------------------------------------------	*/
/*	  		o c c i _ h t m l _ s t y l e 			*/
/*	------------------------------------------------------------	*/
private	void	occi_html_style( FILE * h, char * filename )
{
	FILE * css;
	int	c;
	if (!( filename ))
		return;
	else if (!( css = fopen( filename,"r" ) ))
		return;
	else	
	{
		fprintf(h,"<style type='text/css' media=SCREEN>\n");
		while ((c = fgetc(css)) > 0)
			fputc(c,h);
		fclose(css);
		fprintf(h,"</style>\n");
		return;
	}
}

/*	------------------------------------------------------------	*/
/*	  		o c c i _ h t m l _ h e a d e r 		*/
/*	------------------------------------------------------------	*/
private	void	occi_html_header( FILE * h, char * title, char * extra )
{
	fprintf(h,"<html><head><title>%s%s</title>\n",title,extra);
	occi_html_style(h,"style.css");
	fprintf(h,"</head><body><div align=center>\n");
	return;
}

/*	------------------------------------------------------------	*/
/*	  		o c c i _ h t m l _ p u b l i s h e r 		*/
/*	------------------------------------------------------------	*/
private	void	occi_html_publisher( FILE * h )
{
	char *	publisher;
	if ((publisher = default_publisher()) != (char *) 0)
	{
		fprintf(h,"<tr><th>");
		fprintf(h,"<a href='%s/publication/'>",publisher);
		fprintf(h,"%s/publication/</a>\n",publisher);
		fprintf(h,"</th></tr>");
	}
	fprintf(h,"<tr><th>");
	fprintf(h,"<a href='/-/'>/-/</a>\n");
	fprintf(h,"</th></tr>");
	return;
}

/*	------------------------------------------------------------	*/
/*	  		o c c i _ h t m l _ f o o t e r 		*/
/*	------------------------------------------------------------	*/
private	void	occi_html_footer( FILE * h )
{
	fprintf(h,"<table><tr><th><a href='http://www.compatibleone.org'>http : // www . compatibleone . org / </a></table>\n");
	fprintf(h,"</div></body></html>\n");
	return;
}

/*	------------------------------------------------------------	*/
/*	  		o c c i _ h t m l _ c a p a c i t i e s		*/
/*	------------------------------------------------------------	*/
public	char *	occi_html_capacities( 
		struct occi_category * cptr,
		struct rest_response * aptr )
{
	FILE *	h;
	char *	filename;
	char	buffer[2048];
	char *	vptr;
	char *	nptr;
	struct	rest_header * contentlength=(struct rest_header *) 0;
	struct	rest_header * contenttype=(struct rest_header *) 0;

	if (!( filename = rest_temporary_filename( "html" ) ))
		return( filename );

	else if (!( h = fopen(filename,"w")))
	{
		return(liberate(filename));
	}
	else
	{
		occi_html_header(h,"Accords OCCI Capacities","");
		fprintf(h,"<table><tr><th><h2>ACCORDS OCCI Capacities</h2></th></tr>\n");
		occi_html_publisher(h);
		if ( cptr )
		{
			fprintf(h,"<tr><td><div align=center><table>\n");
			while ( cptr->previous ) cptr = cptr->previous;
			while ( cptr )
			{
				fprintf(h,"<tr><th><a href='/%s/'>/%s/</a>\n",cptr->id,cptr->id);
				cptr = cptr->next;
			}
			fprintf(h,"</table></div>\n");
		}
		fprintf(h,"</table>\n");
		occi_html_footer(h);
		fclose(h);
		if (!( contentlength = rest_response_header( aptr, _HTTP_CONTENT_LENGTH, "0" ) ))
			return( filename );
		else	return( occi_content_length(contentlength, filename ));
	}
}

/*	------------------------------------------------------------	*/
/*	  		o c c i _ x m l _ c a p a c i t i e s		*/
/*	------------------------------------------------------------	*/
public	char *	occi_xml_capacities( 
		struct occi_category * cptr,
		struct rest_response * aptr )
{
	FILE *	h;
	char *	filename;
	char	buffer[2048];
	char *	identity;
	char *	vptr;
	char *	nptr;
	struct	rest_header * contentlength=(struct rest_header *) 0;
	struct	rest_header * contenttype=(struct rest_header *) 0;

	if (!( identity = get_component()))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename( "xml" ) ))
		return( filename );

	else if (!( h = fopen(filename,"w")))
		return(liberate(filename));
	else	
	{
		fprintf(h,"<component name=\"%s\">\n", identity );
		while ( cptr->previous ) cptr = cptr->previous;
		while ( cptr )
		{
			if ((vptr = occi_xml_capacity( cptr )) != (char *) 0 )
			{
				fprintf(h,"%s\n",vptr);
				liberate( vptr );
			}
			cptr = cptr->next;
		}
		fprintf(h,"</component>\n");
		fclose(h);
		if (!( contentlength = rest_response_header( aptr, _HTTP_CONTENT_LENGTH, "0" ) ))
			return( filename );
		else	return( occi_content_length(contentlength, filename ));
	}
}

/*	------------------------------------------------------------	*/
/*			o c c i _ a c c e p t _ h e a d e r 		*/
/*	------------------------------------------------------------	*/
public	struct	rest_header * occi_accept_header( struct rest_response * aptr )
{
	return( rest_response_header( aptr, _HTTP_ACCEPT, "text/occi,text/plain,text/html" ) );
}

/*	------------------------------------------------------------	*/
/*	  		o c c i _ t e x t _ c a p a c i t i e s		*/
/*	------------------------------------------------------------	*/
public	char *	occi_text_capacities( 
		struct occi_category * cptr,
		struct rest_response * aptr )
{
	FILE *	h;
	char *	mptr;
	char *	filename;
	char	buffer[2048];
	char *	vptr;
	char *	nptr;
	struct	rest_header * contentlength=(struct rest_header *) 0;
	struct	rest_header * contenttype=(struct rest_header *) 0;
	struct	rest_header * hptr;

	if (!( filename = rest_temporary_filename( "txt" ) ))
		return( filename );

	else if (!( h = fopen(filename,"w")))
	{
		return(liberate(filename));
	}
	else
	{
		for (	;
			cptr != (struct occi_category *) 0;
			cptr = cptr->next )
		{
			if ( cptr->access & _OCCI_SECRET )
				continue;
			else if (!( mptr = occi_http_capacity( cptr ) ))
				continue;
			else
			{
				fprintf(h,"Category: %s\n",mptr);
				liberate( mptr );
			}
		}
		fclose(h);
		if (!( hptr = occi_accept_header( aptr )))
			return( filename );
		else if (!( contentlength = rest_response_header( aptr, _HTTP_CONTENT_LENGTH, "0" ) ))
			return( filename );
		else	return( occi_content_length(contentlength, filename ));
	}
}

/*	------------------------------------------------------------	*/
/*	  		o c c i _ h t m l _ b o d y 			*/
/*	------------------------------------------------------------	*/
public	char *	occi_html_body( 
		struct occi_category * cptr,
		struct rest_header  * hptr )
{
	FILE *	h;
	char *	extra="";
	char *	prefix;
	char *	filename;
	char *	hostname;
	char 	catroot[1024];
	char	buffer[2048];
	char *	vptr;
	char *	xptr;
	char *	name;
	char *	wptr;
	int	attributs=0;
	int	locations=0;
	int	formactions=0;
	char	id[64];
	char *	publisher;
	struct	occi_action * aptr;
	struct	rest_header * contentlength=(struct rest_header *) 0;
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	int	linkto=0;
	int	allow_methods=1;

	id[0] = 0;

	if (!( prefix = rest_http_prefix() ))
		return( prefix );
	else if (( hostname = get_identity()) != (char *) 0)
		sprintf(catroot,"%s/%s/",hostname,cptr->id);
	else	sprintf(catroot,"/%s/",cptr->id);

	if (!( filename = rest_temporary_filename( "html" ) ))
		return( filename );

	else if (!( h = fopen(filename,"w")))
	{
		return(liberate(filename));
	}
	else
	{
		while ( hptr )
		{
			if (!( hptr->name ))
				hptr = hptr->next;
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_TYPE ) ))
			{
				rest_replace_header( hptr, _OCCI_TEXT_HTML );
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_LENGTH ) ))
			{
				contentlength = hptr;
				hptr = hptr->next;
			}

			else if (!( strcasecmp( hptr->name, _OCCI_LOCATION ) ))
			{
				/* --------------------------------------------- */
				/* its a category instance list type of response */
				/* --------------------------------------------- */
				if (!( locations++ ))
				{
					occi_html_header(h,"Accords OCCI Resource List : ",cptr->id);
					fprintf(h,"<form  method='POST' action='/%s/' enctype='application/x-www-form-urlencoded'>\n",cptr->id);
					fprintf(h,"<table><tr><th><h2>ACCORDS OCCI Location List</h2></th></tr>\n");
					occi_html_publisher(h);
					fprintf(h,"<tr><th><a href='/%s/'>/%s/</a></th></tr><tr><td><div align=center><table>\n",cptr->id,cptr->id);
				}
				if (!(xptr = occi_category_id( hptr->value )))
					fprintf(h,"<tr><th><a href='%s://%s'>%s://%s</a></th></tr>\n",prefix,hptr->value,prefix,hptr->value);
				else
				{
					fprintf(h,"<tr><th><a href='%s'>%s</a></th></tr>\n",xptr, xptr );
					liberate( xptr );
				}
				hptr = occi_consume_header( hptr );
			}

			else if (!( strcasecmp( hptr->name, _OCCI_ATTRIBUTE ) ))
			{
				/* ----------------------------------------------- */
				/* its a category instance record type of response */
				/* ----------------------------------------------- */
				if (!( attributs++ ))
				{
					occi_html_header(h,"Accords OCCI Resource Instance : ",cptr->id);
					fprintf(h,"<form  method='POST' action='/%s/' enctype='application/x-www-form-urlencoded'>\n",cptr->id);
					fprintf(h,"<table><tr><th><h2>ACCORDS OCCI Category Instance</h2></th></tr>\n");
					occi_html_publisher(h);
					fprintf(h,"<tr><th><a href='/%s/'>/%s/</a></th></tr><tr><td><div align=center>\n",cptr->id,cptr->id);
					fprintf(h,"<input type=hidden id='domain' name='domain' value='%s'>\n",cptr->domain);
					fprintf(h,"<input type=hidden id='category' name='category' value='%s'>\n",cptr->id);
					fprintf(h,"<table>\n");
				}
				strcpy((name = vptr = buffer),hptr->value);
				while ( *vptr )
				{
					if ( *vptr == '=' )
					{
						*(vptr++) = 0;
						break;
					}
					else if ( *(vptr++) == '.' )
						name = vptr;
				}
				/* ---------------------------------------------------------- */
				/* detect an HTTP or HTTPS prefixed string and build url link */
				/* ---------------------------------------------------------- */
				if (!( vptr ))
					linkto=0;
				else if (!( vptr = allocate_string( vptr ) ))
					linkto = 0;
				else if (!( vptr = occi_unquoted_value( vptr ) ))
					linkto = 0;
				else if ((!( strncmp( vptr, "http://", strlen( "http://" ) ) ))
				     ||  (!( strncmp( vptr, "https://", strlen( "https://" ) ) )))
					linkto=1;
				else	linkto=0;

				if (!( linkto ))
					fprintf(h,"<tr><th>%s</th>",name);
				else	fprintf(h,"<tr><th><a href='%s'>%s</a></th>",vptr,name);

				if ( name )
				{
					if (!( strcmp(name,"id") ))
					{
						extra = "readonly";
						strcpy(id,vptr);
					}
					else if (!( strcmp(name,"state") ))
						extra = "readonly";
					else	extra="";
				}
				else	extra="";

				fprintf(h,"<td><input class=input %s type=text name='%s' value='%s'>\n",extra,name,vptr);
				if ( vptr ) liberate( vptr );

				/* ---------------------------------------------------------- */
				/* detect the ID element value and store for eventual actions */
				/* ---------------------------------------------------------- */
				fprintf(h,"</td></tr>\n");
				hptr = occi_consume_header( hptr );
			}
			else	hptr = hptr->next;
		}
		if ( attributs )
		{
			/* --------------------------------------- */
			/* generate the list of links if available */
			/* --------------------------------------- */
			fprintf(h,"<tr><th>Links</th><td><div align=center><table>\n");
			for (	nptr=occi_first_link_node();
				nptr != (struct occi_link_node *) 0;
				nptr = nptr->next )
			{
				if (!( lptr = nptr->contents ))
					continue;
				else if (!( lptr->source ))
					continue;
				else if (!( lptr->target ))
					continue;
				else if (!( wptr = occi_category_id( lptr->source ) ))
					continue;
				else if ( strcmp( wptr, id ) != 0)
					continue;
				else	fprintf(h,"<tr><td><a href='%s'>%s</a>",lptr->target,lptr->target);
			}
			fprintf(h,"</table></div></td></tr>\n");

			if ( allow_methods )
			{
				/* ---------------------------- */
				/* generate the list of methods */
				/* ---------------------------- */
				fprintf(h,"<tr><th>Methods</th><td>\n");
				fprintf(h,"<div align=center><table>\n");
				fprintf(h,"<tr><td><input class=button type=submit name='action' value='POST'>\n");
				fprintf(h,"<tr><td><input class=button type=submit name='action' value='GET'>\n");
				fprintf(h,"</table></div></td></tr>\n");
			}

			/* ----------------------------------------- */
			/* generate the list of actions if available */
			/* ----------------------------------------- */
			if ((aptr = cptr->firstact) != (struct occi_action *) 0)
			{
				fprintf(h,"<tr><th>Actions</th><td><div align=center><table>\n");
				while ( aptr )
				{
					if (!( formactions++ ))
						fprintf(h,"</form>\n");
					fprintf(h,"<tr><td><form action='%s%s?action=%s' method='POST' enctype='application/x-www-form-urlencoded'>\n",catroot,id,aptr->name);
					fprintf(h,"<input class=action type=submit value='%s' name='%s'></form>\n",aptr->name,aptr->name);
					aptr = aptr->next;
				}
				fprintf(h,"</table></div></td></tr>\n");
			}
			fprintf(h,"</table>\n");
		}
		/* ------------------------ */
		/* generate the page footer */
		/* ------------------------ */
		else if ( locations )
		{
			fprintf(h,"</table>\n");
			fprintf(h,"<tr><th><input class=button type=submit name='POST' value='POST'></tr>\n");
		}
		else	
		{
			occi_html_header(h,"Accords OCCI Resource List : ",cptr->id);
			fprintf(h,"<form  method='POST' action='/%s/' enctype='application/x-www-form-urlencoded'>\n",cptr->id);
			fprintf(h,"<table><tr><th><h2>ACCORDS OCCI Location List</h2></th></tr>\n");
			occi_html_publisher(h);
			fprintf(h,"<tr><th><a href='/%s/'>/%s/</a></th></tr><tr><td><div align=center>\n",cptr->id,cptr->id);
			fprintf(h,"<table><tr><th>&nbsp;</th></tr></table></div>\n");
			fprintf(h,"<tr><th><input class=button type=submit value='POST' name='POST'></tr>\n");
		}
		fprintf(h,"</table>\n");
		if (!( formactions ))
			fprintf(h,"</form>\n");
		occi_html_footer(h);
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
	struct	rest_header * hhptr;
	char *	filename;
	char	buffer[2048];
	char *	vptr;
	char *	mptr;
	int	attributs=0;
	int	mode=0;

	if (!( filename = rest_temporary_filename( "txt" ) ))
		return((char *) 0);

	else if (!( h = fopen(filename,"w") ))
		return( liberate( filename ) );

	else
	{
		for (	hhptr=hptr;
			hhptr != (struct rest_header *) 0;
			hhptr = hhptr->next )
		{
			if (!( strcasecmp( hhptr->name, _OCCI_LOCATION ) ))
			{
				mode=1;
				break;
			}
			else if (!( strcasecmp( hhptr->name, _OCCI_ATTRIBUTE ) ))
			{
				mode=2;
				break;
			}
		}

		if ( mode == 1 )
		{
			for (	;
				hhptr != (struct rest_header *) 0;
				hhptr = hhptr->next )
			{

				if (!( strcasecmp( hhptr->name, _OCCI_LOCATION ) ))
					fprintf(h,"%s: %s\n",hhptr->name, hhptr->value);
			}			

		}
		else
		{
			if (( mptr = occi_http_category( cptr )) != (char *) 0)
			{
				fprintf(h,"Category: %s\n",mptr);
				liberate( mptr );
			}
			for (	;
				hhptr != (struct rest_header *) 0;
				hhptr = hhptr->next )
			{

				if (!( strcasecmp( hhptr->name, _OCCI_ATTRIBUTE ) ))
					fprintf(h,"%s: %s\n",hhptr->name, hhptr->value);
			}			
		}
		while ( hptr )
		{
			if (!( hptr->name ))
				hptr = hptr->next;
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_TYPE ) ))
			{
				rest_replace_header( hptr, _OCCI_TEXT_PLAIN );
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_LENGTH ) ))
			{
				contentlength = hptr;
				hptr = hptr->next;
			}
			else if ((!( strcasecmp( hptr->name, _OCCI_LOCATION  ) ))
			     ||  (!( strcasecmp( hptr->name, _OCCI_ATTRIBUTE ) ))
			     ||  (!( strcasecmp( hptr->name, _OCCI_CATEGORY  ) )))
			{
				hptr = occi_consume_header( hptr );
			}
			else	hptr = hptr->next;
		}
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
	char *	nptr;
	int		locations=0;
	int		attributs=0;
	if (!( filename = rest_temporary_filename( "php" ) ))
		return( filename );

	else if (!( h = fopen(filename,"w")))
	{
		return( liberate( filename ) );
	}
	else
	{
		fprintf(h,"<?php\n");
		while ( hptr )
		{
			if (!( hptr->name ))
				hptr = hptr->next;
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_TYPE ) ))
			{
				rest_replace_header( hptr, _OCCI_MIME_JSON );
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_LENGTH ) ))
			{
				contentlength = hptr;
				hptr = hptr->next;
			}
			else if (!( strcasecmp( hptr->name, _OCCI_LOCATION ) ))
			{
				if (!( locations++ ))
				{
					fprintf(h," return( ");
					fprintf(h," array( %c%s%c ",0x0022,"occi",0x0022);
					fprintf(h," => array( %c%s%c", 0x0022,cptr->id, 0x0022);
					fprintf(h," => array( %c%s%c", 0x0022,"location", 0x0022);
					fprintf(h," => array( \n");
				}
				else	fprintf(h,",\n");
				fprintf(h,"\t%c%s%c",0x0022,hptr->value,0x0022);
				hptr = occi_consume_header( hptr );
			}
			else if (!( strcasecmp( hptr->name, _OCCI_ATTRIBUTE ) ))
			{
				if (!( attributs++ ))
				{
					fprintf(h," return( ");
					fprintf(h," array( %c%s%c ",0x0022,"occi",0x0022);
					fprintf(h," => array( %c%s%c", 0x0022,cptr->id, 0x0022);
					fprintf(h," => array( \n");
				}
				else	fprintf(h,",\n");

				strcpy((nptr = vptr = buffer),hptr->value);
				while ( *vptr )
				{
					if ( *vptr == '=' )
					{
						*(vptr++) = 0;
						break;
					}
					else if ( *(vptr++) == '.' )
						nptr = vptr;
				}
				if ( attributs++ )
					fprintf(h,",\n");
				fprintf(h,"\t%c%s%c => %c%s%c",0x0022,nptr,0x0022,0x0022,vptr,0x0022);
				hptr = occi_consume_header( hptr );
			}
			else	hptr = hptr->next;
		}

		if ( locations )
			fprintf(h," )))) );\n");

		else if ( attributs )
			fprintf(h," ))) );\n");
		
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
	char *	nptr;
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
			if (!( hptr->name ))
				hptr = hptr->next;
			else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_TYPE ) ))
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
				strcpy((nptr = vptr = buffer),hptr->value);
				while ( *vptr )
				{
					if ( *vptr == '=' )
					{
						*(vptr++) = 0;
							break;
					}
					else if ( *(vptr++) == '.' )
						nptr = vptr;
				}
				if (!( attributs++ ))
					fprintf(h,"<%s\n",cptr->id);
				fprintf(h,"\t%s=%c%s%c\n",nptr,0x0022,vptr,0x0022);
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

/*	--------------------------------------------------------------	*/
/*		a c c e p t _ s t r i n g _ i n c l u d e s		*/
/*	--------------------------------------------------------------	*/
public	int	accept_string_includes( char * sptr, char * tptr )
{
	char *	xptr;
	char *	wptr;
	char *	root;
	if (!( wptr = allocate_string( sptr ) ))
		return( 0 );
	else	root = wptr;
	while ( *wptr )
	{
		xptr = wptr;
		while ( *xptr )
		{
			if (( *xptr == ',' )|| ( *xptr == ';' ))
			{
				*(xptr++) = 0;
				break;
			}
			else	xptr++;
		}
		if (!( strcmp( wptr, tptr ) ))
		{
			liberate( root );
			return( 1 );
		}
		wptr = xptr;	
	}
	liberate( root );
	return( 0 );
}

/*	---------------------------------------------------	*/
/*		o c c i _ r e s p o n s e _ b o d y  		*/
/*	---------------------------------------------------	*/
/*								*/
/*	This function is called from two different places:	*/
/*								*/
/*	First is called from the OCCI Client to prepare the 	*/
/*	POST and PUT methods which provide attribute values	*/
/*								*/
/*	Second called from the OCCI Server to prepare the 	*/
/*	response message after the request has been handled	*/
/*								*/
/*	---------------------------------------------------	*/
public	char * occi_response_body( char * accepts, struct occi_category * cptr, struct rest_header * hptr )
{
	if ((!( strncasecmp( accepts, _OCCI_TEXT_PLAIN, strlen(_OCCI_TEXT_PLAIN) ) ))
	||  (!( strcasecmp( accepts, "*/*" ) )))
		return( occi_text_body( cptr, hptr ) );

	else if ( accept_string_includes( accepts, _OCCI_TEXT_HTML ) )
	{
		if ( cptr->html_rendering )
			return((*cptr->html_rendering)(cptr, hptr));
		else	return( occi_html_body( cptr, hptr ) );
	}
	else if ((!( strncasecmp( accepts, _OCCI_OCCI_PHP, strlen(_OCCI_OCCI_PHP) ) ))
	     ||  (!( strncasecmp( accepts, _OCCI_APP_PHP, strlen(_OCCI_APP_PHP)  ) ))
	     ||  (!( strncasecmp( accepts, _OCCI_TEXT_PHP, strlen(_OCCI_TEXT_PHP) ) )))
		return( occi_php_body(  cptr, hptr ) );

	else if ((!( strncasecmp( accepts, _OCCI_OCCI_JSON, strlen(_OCCI_OCCI_JSON) ) ))
	     ||  (!( strncasecmp( accepts, _OCCI_APP_JSON, strlen(_OCCI_APP_JSON)  ) ))
	     ||  (!( strncasecmp( accepts, _OCCI_TEXT_JSON, strlen(_OCCI_TEXT_JSON) ) )))
		return( occi_json_body(  cptr, hptr ) );

	else if (!( strncasecmp( accepts, _OCCI_OLD_JSON, strlen(_OCCI_OLD_JSON) ) ))
		return( occi_old_json_body(  cptr, hptr ) );

	else if ((!( strncasecmp( accepts, _OCCI_MIME_XML, strlen(_OCCI_MIME_XML)  ) ))
	     ||  (!( strncasecmp( accepts, _OCCI_APP_XML, strlen(_OCCI_APP_XML)  ) ))
	     ||  (!( strncasecmp( accepts, _OCCI_TEXT_XML, strlen(_OCCI_TEXT_XML)  ) )))
		return( occi_xml_body(  cptr, hptr  ) );

	else	return( occi_text_body( cptr, hptr ) );
}

	/* ---------- */
#endif	/* _occi_body */
	/* ---------- */

