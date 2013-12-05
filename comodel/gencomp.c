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

#ifndef	_gencomp_c
#define	_gencomp_c

#include "standard.h"
#include "occi.h"

private	void	date_string( char * result )
{
	int 	tt;
	char 	buffer[256];
	struct	tm * tptr;
	time_t t;
	t = (time_t) time((long *) 0);

	if (!( tptr = localtime( &t )))
		sprintf(buffer,"%u",t);
	else
	{
		sprintf(buffer,"%u/%u/%u %u:%u:%u",
			tptr->tm_mday, tptr->tm_mon+1,tptr->tm_year+1900,
			tptr->tm_hour, tptr->tm_min, tptr->tm_sec);
	}
	strcpy( result, buffer );
	return;
}

/*	--------------------------------------------------	*/
/*	  g e n e r a t e _ f i l e _ e x c l u s i o n  	*/
/*	--------------------------------------------------	*/
private	void	generate_file_exclusion(FILE * h, char * sptr, int mode )
{
	if ( mode == 1 )
	{
		fprintf(h,"#ifndef %s\n#define %s\n",sptr,sptr);
	}
	else
	{
		fprintf(h,"#endif /* %s */\n",sptr);

	}
}

/*	--------------------------------------------------	*/
/*	  g e n e r a t e _ f i l e _ i n c l u s i o n  	*/
/*	--------------------------------------------------	*/
private	void	generate_file_inclusion( FILE * h, char * nptr )
{
	fprintf(h,"#include %c%s%c\n",0x0022,nptr,0x0022);
	return;
}

/*	--------------------------------------------------	*/
/*	  g e n e r a t e _ a c c o r d s _ c o n f i g  	*/
/*	--------------------------------------------------	*/
private	void	generate_accords_config( FILE * h, char * nptr, char * prefix )
{
	char	datebuffer[256];
	title(h,"accords configuration");
	fprintf(h,"private struct accords_configuration Configuration = {\n");
	fprintf(h,"\t0,0,0,0,0,0,\n");
	fprintf(h,"\t(char *) 0,\n");
	fprintf(h,"\t(char *) 0,\n");
	fprintf(h,"\t_CORDS_DEFAULT_PUBLISHER,\n");
	fprintf(h,"\t_CORDS_DEFAULT_OPERATOR,\n");
	fprintf(h,"\t_CORDS_DEFAULT_USER,\n");
	fprintf(h,"\t_CORDS_DEFAULT_PASSWORD,\n");
	fprintf(h,"\t%c%s%c,%u,\n",0x0022,"http",0x0022,80);
	fprintf(h,"\t%c%s%c,%u,\n",0x0022,"xmpp",0x0022,8000);
	fprintf(h,"\t%c%s%c,\n",0x0022,"domain",0x0022);
	fprintf(h,"\t%c%s.xml%c,\n",0x0022,nptr,0x0022);
	fprintf(h,"\t%c%s%c,\n",0x0022,"europe",0x0022);
	fprintf(h,"\t%c%s%c,\n",0x0022,"storage",0x0022);
	fprintf(h,"\t%c%s%c,\n",0x0022,nptr,0x0022);
	fprintf(h,"\t(struct occi_category *) 0,\n");
	fprintf(h,"\t(struct occi_category *) 0\n");
	fprintf(h,"\t};\n");

	title(h,"standard relays");
	fprintf(h,"public int check_debug() { return( Configuration.debug ); }\n");
	fprintf(h,"public int check_verbose() { return( Configuration.verbose ); }\n");
	fprintf(h,"public char * default_publisher() { return(Configuration.publisher); }\n");
	fprintf(h,"public char * default_operator() { return(Configuration.operator); }\n");
	fprintf(h,"public char * default_storage() { return(Configuration.storage); }\n");
	fprintf(h,"public char * default_tls() { return(Configuration.tls); }\n");
	fprintf(h,"public char * default_zone() { return(Configuration.zone); }\n");

	title(h,"failure");
	fprintf(h,"public int failure( int e, char * m1, char * m2 )\n{\n");
	fprintf(h,"\tif ( e )\n\t{\n");
	fprintf(h,"\t\tprintf(\"\\n*** failure %cu\",e);\n",0x0025);
	fprintf(h,"\t\tif ( m1 ) printf(\" : %cs\",m1);\n",0x0025);
	fprintf(h,"\t\tif ( m2 ) printf(\" : %cs\",m2);\n",0x0025);	
	fprintf(h,"\t\tprintf(\" **\\n\");\n");
	fprintf(h,"\t}\n\treturn( e );\n}\n");
	fprintf(h,"private void	%s_configuration()\n{\n",nptr);
	fprintf(h,"\tload_accords_configuration( &Configuration, \"%s\");\n",nptr);
	fprintf(h,"\treturn;\n}\n");

	title(h,"banner");
	fprintf(h,"private int %s_banner()\n{\n",nptr);
	fprintf(h,"\tprintf(\"\\n   CompatibleOne Generated Service %s: Version 1.0a.0.01\");\n",nptr);
	date_string( datebuffer);
	fprintf(h,"\tprintf(\"\\n   Beta Version : %s \");\n",datebuffer);
	if ( prefix )
	{
		if (!( strcmp( prefix, "sql" ) ))
			fprintf(h,"\tprintf(\"\\n   Back End : SQL%s \\n\");\n",(check_testflag() ? "(test)" : "") );
		else	fprintf(h,"\tprintf(\"\\n   Back End : XML%s \\n\");\n",(check_testflag() ? "(test)" : "") );
	}
	fprintf(h,"\tprintf(\"\\n   Copyright (c) 2013 Iain James Marshall\");\n",nptr);
	fprintf(h,"\tprintf(\"\\n\");\n",nptr);
	fprintf(h,"\taccords_configuration_options();\n",nptr);
	fprintf(h,"\tprintf(\"\\n\\n\");\n");
	fprintf(h,"\treturn(0);\n}\n",nptr);

	return;
}

/*	--------------------------------------------------	*/
/*	    c a t e g o r y _ a c t i o n _ f i l e		*/
/*	--------------------------------------------------	*/
private	char * category_action_file( char * category, char * action, char * suffix )
{
	char	filename[2048];
	FILE *	h;
	sprintf(filename,"%s_%s.%s",action,category,suffix);
	if (( h = fopen( filename, "r" )) != (FILE *) 0)
	{
		fclose(h);
		return( allocate_string( filename ) );
	}
	sprintf(filename,"%s_%s.%s",category,action,suffix);
	if (( h = fopen( filename, "r" )) != (FILE *) 0)
	{
		fclose(h);
		return( allocate_string( filename ) );
	}
	sprintf(filename,"%s-%s.%s",action,category,suffix);
	if (( h = fopen( filename, "r" )) != (FILE *) 0)
	{
		fclose(h);
		return( allocate_string( filename ) );
	}
	sprintf(filename,"%s-%s.%s",category,action,suffix);
	if (( h = fopen( filename, "r" )) != (FILE *) 0)
	{
		fclose(h);
		return( allocate_string( filename ) );
	}
	sprintf(filename,"%s/%s.%s",category,action,suffix);
	if (( h = fopen( filename, "r" )) != (FILE *) 0)
	{
		fclose(h);
		return( allocate_string( filename ) );
	}
	return((char *) 0);
}

/*	----------------------------------------------------------------	*/
/*	 g e n e r a t e _ c a t e g o r y  _ a c t i o n _ s c r i p t	 	*/
/*	----------------------------------------------------------------	*/
private	int	generate_category_action_script( struct occi_category * cptr, char * action )
{
	FILE * h;
	char buffer[2048];
	char filename[1024];

	/* ------------------------ */
	/* check for file existance */
	/* ------------------------ */
	sprintf(filename,"csp-action-%s-%s.txt",action,cptr->id);
	if (( h = fopen( filename, "r" )) != (FILE *) 0)
	{
		fclose(h);
		return(1);
	}
	/* ------------------------ */
	/* create cords script file */
	/* ------------------------ */
	else if (( h = fopen( filename, "w" )) != (FILE *) 0)
	{
		sprintf(buffer,"category action script: %s",filename);
		title( h, buffer );
		fprintf(h,"$caller    = $1;\n");
		fprintf(h,"$instance  = $2;\n");
		fprintf(h,"$action    = $3;\n");
		fprintf(h,"$publisher = $4;\n");
		fprintf(h,"$operator  = $5;\n");
		fprintf(h,"if ( $caller == '%s.%s' )\n",cptr->id,action);
		fprintf(h,"{\n");
		fprintf(h,"\t$instance.display();\n");
		fprintf(h,"\t$record = $instance.get();\n");
		fprintf(h,"\tforeach ($record as $tuple)\n");
		fprintf(h,"\t{\n");
		fprintf(h,"\t\t$tuple.display();\n");
		fprintf(h,"\t}\n");
		fprintf(h,"}\n");
		title(h,"end of file");
		fclose(h);
		return(1);
	}
	else	return( 0 );
}
	

/*	--------------------------------------------------	*/
/*	g e n e r a t e _ c a t e g o r y  _ a c t i o n s 	*/
/*	--------------------------------------------------	*/
private	void	generate_category_actions( FILE * h, struct occi_category * cptr, char * prefix )
{
	struct	occi_action * aptr;
	char *	nptr;
	char	buffer[1024];
	char * mh;
	int	c;

	if (!( comodel_category_filter( cptr ) ))
		return;

	fprintf(h,"/* Category : %s */\n",cptr->id);
	sprintf(buffer,"%s.h",cptr->id);
	generate_file_inclusion(h,buffer);
	sprintf(buffer,"%s_filter.h",cptr->id);
	generate_file_inclusion(h,buffer);
	sprintf(buffer,"%s.c",cptr->id);
	generate_file_inclusion(h,buffer);
	sprintf(buffer,"%s%s.c",prefix,cptr->id);
	generate_file_inclusion(h,buffer);

	for (	aptr=cptr->firstact;
		aptr != (struct occi_action *) 0;
		aptr = aptr->next )
	{
		if (!( nptr = aptr->name ))
			continue;
		else
		{
			while ( *nptr )
			{
				if ( *nptr == '#' )
					break;
				else	nptr++;
			}
			if ( *nptr != '#' )
				continue;
			else	nptr++;
			if (!( strcmp( nptr, "DELETE" )))
				continue;
			else if (!( mh = category_action_file( cptr->id, nptr, "c" ) ))
			{
				title( h, nptr );
				fprintf(h,"private struct rest_response * _%s_%s(\n",nptr,cptr->id);
				fprintf(h,"\tstruct occi_category * optr,\n");
				fprintf(h,"\tstruct rest_client * cptr,\n");
				fprintf(h,"\tstruct rest_request * rptr,\n");
				fprintf(h,"\tstruct rest_response * aptr,\n");
				fprintf(h,"\tvoid * vptr )\n{\n");
				fprintf(h,"\tstruct cords_%s * pptr;\n",cptr->id);
				fprintf(h,"\tif (!( pptr = (struct cords_%s *) vptr ))\n",cptr->id);
				fprintf(h,"\t\treturn( rest_html_response( aptr, 400, \"Incorrect Request\" ) );\n");
				fprintf(h,"\telse\treturn( category_action_script( aptr, \"%s\", \"%s\", pptr->id ) );\n",cptr->id, nptr);
				fprintf(h,"}\n");
				generate_category_action_script( cptr, nptr );
			}
			else
			{
				title( h, nptr );
				generate_file_inclusion(h,mh);
				liberate( mh );
			}
		}
	}
 	return;
}


/*	--------------------------------------------------	*/
/*	  g e n e r a t e _ a d d _ c a t e g o r y		*/ 
/*	--------------------------------------------------	*/
private	void	generate_add_category( FILE * h, struct occi_category * cptr )
{
	struct	occi_action * aptr;
	char *	nptr;
	if (!( comodel_category_filter( cptr ) ))
		return;
	fprintf(h,"\tif (!( optr = occi_cords_%s_builder( Configuration.domain, %c%s%c ) ))\n",cptr->id,0x0022,cptr->id,0x0022);
	fprintf(h,"\t\treturn( 27 );\n");
	fprintf(h,"\tif (!( optr->previous = last ))\n");
	fprintf(h,"\t\tfirst = optr;\n");
	fprintf(h,"\telse\toptr->previous->next = optr;\n");
	fprintf(h,"\tlast = optr;\n");
	for (	aptr=cptr->firstact;
		aptr != (struct occi_action *) 0;
		aptr = aptr->next )
	{
		if (!( nptr = aptr->name ))
			continue;
		else
		{
			while ( *nptr )
			{
				if ( *nptr == '#' )
					break;
				else	nptr++;
			}
			if ( *nptr != '#' )
				continue;
			else	nptr++;
			if (!( strcmp( nptr, "DELETE" )))
				continue;
			fprintf(h,"\tif (!( optr = occi_add_action( optr, %c%s%c, \"\", _%s_%s ) ))\n",0x0022,nptr,0x0022,nptr,cptr->id);
			fprintf(h,"\t\treturn( 27 );\n");
		}
	}
 	return;
}

/*	--------------------------------------------------	*/
/*	  g e n e r a t e _ f i l e _ a c t i o n s 		*/
/*	--------------------------------------------------	*/
private	void	generate_file_actions( FILE * h, struct occi_category * cptr, char * prefix )
{
	for (;	
		cptr != (struct occi_category *) 0; 
		cptr = cptr->next )
		generate_category_actions(h, cptr, prefix );
	return;
}

/*	--------------------------------------------------	*/
/*	  g e n e r a t e _ f i l e _ o p e r a t i o n		*/
/*	--------------------------------------------------	*/
private	void	generate_file_operation( FILE * h, char * nptr, struct occi_category * cptr, char * prefix )
{
	title(h,"operation");
	fprintf(h,"\nprivate int %s_operation( char * sptr )\n{\n",nptr);
	fprintf(h,"\tchar buffer[2048];\n");
	fprintf(h,"\tstruct occi_category * first=(struct occi_category *) 0;\n");
	fprintf(h,"\tstruct occi_category * last=(struct occi_category *) 0;\n");
	fprintf(h,"\tstruct occi_category * optr=(struct occi_category *) 0;\n");
	fprintf(h,"\tset_autosave_cords_xlink_name(\"links_%s.xml\");\n",nptr);
	fprintf(h,"\trest_initialise_log( Configuration.monitor );\n");
	if ( prefix )
	{
		if (!( strcmp( prefix, "sql" ) ))
		{
			fprintf(h,"\tsprintf(buffer,%c%cs_database%c,default_operator());\n",
				0x0022,0x0025,0x0022);
			fprintf(h,"\tinitialise_occi_sql(default_storage(),buffer,get_default_user(),get_default_password());\n");
		}
	}
	for (	;	
		cptr != (struct occi_category *) 0; 
		cptr = cptr->next )
	{
		generate_add_category(h, cptr );
	}
	fprintf(h,"\tif (!( Configuration.identity ))\n");
	fprintf(h,"\t\treturn( occi_server(sptr,Configuration.restport, Configuration.tls,Configuration.threads, first, (char *) 0 ) );\n");
	fprintf(h,"\telse\n\t{\n");
	fprintf(h,"\t\tinitialise_occi_publisher(Configuration.publisher,(char *) 0,(char *) 0,(char *) 0);\n");
	fprintf(h,"\t\treturn( publishing_occi_server(Configuration.user, Configuration.password,Configuration.identity,sptr,Configuration.restport,Configuration.tls,Configuration.threads,first));\n");
	fprintf(h,"\t}\n");
	fprintf(h,"}\n");
	return;
}

/*	--------------------------------------------------	*/
/*		g e n e r a t e _ f i l e _ s t a r t 		*/
/*	--------------------------------------------------	*/
private	void	generate_file_start( FILE * h, char * nptr )
{
	title(h,"start");
	fprintf(h,"\nprivate int %s( int argc, char * argv[] )\n{\n",nptr);
	fprintf(h,"\tint argi=0;\n");
	fprintf(h,"\tchar * aptr;\n");
	fprintf(h,"\t%s_configuration();\n",nptr);
	fprintf(h,"\twhile ( argi < argc ) {\n");
	fprintf(h,"\t\tif (!( aptr = argv[++argi] )) break;\n");
	fprintf(h,"\t\telse if ( *aptr != '-' ) return( %s_operation( aptr ) );\n",nptr);
	fprintf(h,"\t\telse if ( *(++aptr) != '-' ) return( failure( 30, aptr, \"\" ) );\n");
	fprintf(h,"\t\telse if (!( argi = accords_configuration_option((aptr+1), argi, argv) )) break;\n");
	fprintf(h,"\t\telse continue;\n"); 
	fprintf(h,"\t}\n");
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}

/*	--------------------------------------------------	*/
/*		g e n e r a t e _ f i l e _ m a i n 		*/
/*	--------------------------------------------------	*/
private	void	generate_file_main( FILE * h, char * nptr )
{
	title(h,"main");
	fprintf(h,"\npublic int main( int argc, char * argv[] )\n{\n");
	fprintf(h,"\tif ( argc == 1 ) return( %s_banner() );\n",nptr);
	fprintf(h,"\telse return( %s(argc,argv) );\n",nptr);
	fprintf(h,"}\n");
	return;
}

/*	---------------------------------------------------	*/
/*	g e n e r a t e _ s e r v i c e _ c o m p o n e n t	*/
/*	---------------------------------------------------	*/
public	int	generate_service_component( char * name, struct occi_category * cptr, char * prefix )
{
	FILE * h;
	char buffer[1024];
	sprintf(buffer,"%s.c",name);
	if (!( h =fopen( buffer, "w" ) ))
		return( 46 );
	else
	{
		sprintf(buffer,"_%s_c",name);
		generate_file_exclusion(h,buffer,1);
		generate_file_inclusion(h,"standard.h");
		generate_file_inclusion(h,"rest.h");
		generate_file_inclusion(h,"occi.h");
		generate_file_inclusion(h,"document.h");
		generate_file_inclusion(h,"cordspublic.h");
		generate_file_inclusion(h,"occipublisher.h");
		generate_file_inclusion(h,"accords.h");
		generate_file_inclusion(h,"cspi.h");
		if ( prefix )
		{
			if (!( strcmp( prefix, "sql" ) ))
			{
				generate_file_inclusion(h,"occisql.h");
			}
		}
		generate_accords_config(h,name,prefix);
		generate_file_actions(h,cptr,prefix);
		generate_file_operation(h,name,cptr,prefix);
		generate_file_start(h,name);
		generate_file_main(h,name);
		generate_file_exclusion(h,buffer,2);
		fclose(h);
		return(0);
	}
}

/*	---------------------------------------------------	*/
/*	 g e n e r a t e _ s e r v i c e _ m a k e f i l e 	*/
/*	---------------------------------------------------	*/
public	int	generate_service_makefile( char * name, struct occi_category * cptr, char * prefix )
{
	FILE * h;
	char buffer[1024];
	sprintf(buffer,"Makefile.am");
	if (!( h =fopen( buffer, "w" ) ))
		return( 46 );
	else
	{
		fprintf(h,"#\n# %s Makefile.am\n",name);
		fprintf(h,"#\n# Copyright 2013, All rights reserved\n");
		fprintf(h,"#\n# Authors: \n");
		fprintf(h,"#\tComodel Version 1.0a\n");
		fprintf(h,"#\tStrukt  Version 3.0a\n");
		fprintf(h,"#\n");
		fprintf(h,"bin_PROGRAMS = %s \n",name);
		fprintf(h,"extra_DIST = \n");
		fprintf(h,"%s_SOURCES = %s.c\n",name,name);
		fprintf(h,"%s_CFLAGS  = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_COSQL) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR) $(CFLAGS_COMONS) $(CFLAGS_COCSPI)\n",name);
		fprintf(h,"%s_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCSPI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LD_COSQL) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_MYSQL) $(LDFLAGS_THREADS)\n",name);
		fclose(h);
		return(0);
	}
}

/*	---------------------------------------------------	*/
/*	        g e n e r a t e _ c s p _ i n p u t  		*/
/*	---------------------------------------------------	*/
private	int	generate_csp_input( struct occi_category * cptr, char * name )
{
	struct	occi_attribute * iptr;
	FILE * h;
	char buffer[1024];
	sprintf(buffer,"csp-input-%s.txt",cptr->id);
	if (!( h = fopen( buffer, "w" ) ))
		return( 46 );
	else
	{
		title(h,buffer);
		fprintf(h,"\"<html><head><title>%s</title>\".display();\n",buffer);
		fprintf(h,"\"<link href='style.css' rel='STYLESHEET' type='text/css' media='SCREEN'>\".display();\n");
		fprintf(h,"\"</head><body>\".display();\n");
		fprintf(h,"\"<form method=POST action='%s.php'/>\".display();\n",name);
		fprintf(h,"\"<div align=center><table class=create></div>\".display();\n");
		fprintf(h,"\"<tr><th colspan=2>Create %s</th></tr>\".display();\n",cptr->id);
		for ( 	iptr= cptr->first;
			iptr != (struct occi_attribute *) 0;
			iptr = iptr->next )
		{
			fprintf(h,"\"<tr><th>%s</th>\".display();\n",iptr->name);
			fprintf(h,"\"<th class=data><input type=text size=80 name=%s value=''></th>\".display();\n",iptr->name);
		}
		fprintf(h,"\"<tr><th>action</th>\".display();\n");
		fprintf(h,"\"<th class=data><input type=submit class=action name=action value='create-%s'></th>\".display();\n",cptr->id);
		fprintf(h,"\"<tr><th colspan=2><a href='%s.php?action=list-%s'>list %s records</a></th></tr>\".display();\n",
				name, cptr->id, cptr->id );
		fprintf(h,"\"<tr><th colspan=2><a href='%s.php?action=menu-%s'>return to menu of %s</a></th></tr>\".display();\n",
				name, name, name );
		fprintf(h,"\"</table></div>\".display();\n");
		fprintf(h,"\"</form>\".display();\n");
		fprintf(h,"\"</body></html>\".display();\n");
		title(h,"end of file");
		fclose(h);
		return(0);
	}
}

/*	---------------------------------------------------	*/
/*	        g e n e r a t e _ c s p _ c r e a t e		*/
/*	---------------------------------------------------	*/
private	int	generate_csp_create( struct occi_category * cptr, char * name )
{
	struct	occi_attribute * iptr;
	int	item=0;
	FILE * h;
	char buffer[1024];
	sprintf(buffer,"csp-create-%s.txt",cptr->id);
	if (!( h = fopen( buffer, "w" ) ))
		return( 46 );
	else
	{
		title(h,buffer);
		fprintf(h,"$o = {};\n");
		for ( 	iptr= cptr->first;
			iptr != (struct occi_attribute *) 0;
			iptr = iptr->next )
		{
			fprintf(h,"$%s = $%u;\n",iptr->name,++item);
			fprintf(h,"$t = \"'occi.%s.%s':'\"#$%s#\"'\";\n",cptr->id,iptr->name,iptr->name);
			fprintf(h,"$o = $o + $t;\n");
		}
		fprintf(h,"$r = %s.post($o);\n",cptr->id);
		fprintf(h,"include 'csp-list-%s.txt';\n",cptr->id);
		title(h,"end of file");
		fclose(h);
		return(0);
	}
}

/*	---------------------------------------------------	*/
/*	        g e n e r a t e _ c s p _ c r e a t e		*/
/*	---------------------------------------------------	*/
private	int	generate_csp_update( struct occi_category * cptr, char * name )
{
	struct	occi_attribute * iptr;
	int	item=1;
	FILE * h;
	char buffer[1024];
	sprintf(buffer,"csp-update-%s.txt",cptr->id);
	if (!( h = fopen( buffer, "w" ) ))
		return( 46 );
	else
	{
		title(h,buffer);
		fprintf(h,"if ( $1.length() > 0 )\n{\n");
		fprintf(h,"\t$id = $1;\n");
		fprintf(h,"\t$o = {};\n");
		for ( 	iptr= cptr->first;
			iptr != (struct occi_attribute *) 0;
			iptr = iptr->next )
		{
			fprintf(h,"\t$%s = $%u;\n",iptr->name,++item);
			fprintf(h,"\t$t = \"'occi.%s.%s':'\"#$%s#\"'\";\n",cptr->id,iptr->name,iptr->name);
			fprintf(h,"\t$o = $o + $t;\n");
		}
		fprintf(h,"\t$r = $id.put($o);\n}\n");

		fprintf(h,"include 'csp-list-%s.txt';\n",cptr->id);
		title(h,"end of file");
		fclose(h);
		return(0);
	}
}

/*	---------------------------------------------------	*/
/*	        g e n e r a t e _ c s p _ r e t r ie v e	*/
/*	---------------------------------------------------	*/
private	int	generate_csp_retrieve( struct occi_category * cptr, char * name )
{
	struct	occi_attribute * iptr;
	FILE * h;
	char buffer[1024];
	sprintf(buffer,"csp-retrieve-%s.txt",cptr->id);
	if (!( h = fopen( buffer, "w" ) ))
		return( 46 );
	else
	{
		title(h,buffer);
		fprintf(h,"if ( $1.length() > 0 )\n{\n");
		fprintf(h,"\t$id = $1;\n");
		fprintf(h,"\t$r = $id.get();\n");
		fprintf(h,"\"<html><head><title>%s</title>\".display();\n",buffer);
		fprintf(h,"\"<link href='style.css' rel='STYLESHEET' type='text/css' media='SCREEN'>\".display();\n");
		fprintf(h,"\"</head><body>\".display();\n");
		fprintf(h,"\"<form method=POST action='%s.php'/>\".display();\n",name);
		fprintf(h,"\"<div align=center><table class=create></div>\".display();\n");
		fprintf(h,"$m = \"<tr><th colspan=2>\"#$id#\"</th></tr>\";\n");
		fprintf(h,"$m.display();\n");
		fprintf(h,"$v = $r['occi.core.id'];\n");
		fprintf(h,"$m = \"<tr><th>id</th>\";\n");
		fprintf(h,"$m = $m#\"<th class=data><input type=text size=80 name=id readonly \";\n");
		fprintf(h,"$m = $m#\"value='\"#$id#\"'></th>\";\n");
		fprintf(h,"$m.display();\n");

		for ( 	iptr= cptr->first;
			iptr != (struct occi_attribute *) 0;
			iptr = iptr->next )
		{
			fprintf(h,"$v = $r['occi.%s.%s'];\n",cptr->id,iptr->name);
			fprintf(h,"if ( $v.length() == 0 ) { $v = ''; }\n");
			fprintf(h,"else { if ( $v == '(null)' ) { $v = ''; } }\n");
			fprintf(h,"$m = \"<tr><th>%s</th>\";\n",iptr->name);
			fprintf(h,"$m = $m#\"<th class=data><input type=text size=80 name=%s \";\n",iptr->name);
			fprintf(h,"$m = $m#\"value='\"#$v#\"'></th>\";\n");
			fprintf(h,"$m.display();\n");
		}
		fprintf(h,"\"<tr><th>action</th>\".display();\n");
		fprintf(h,"\"<th class=data><input type=submit class=action name=action value='update-%s'>\".display();\n",cptr->id);
		fprintf(h,"\"&nbsp;<input type=submit class=action name=action value='delete-%s'></th>\".display();\n",cptr->id);
		fprintf(h,"\"<tr><th colspan=2><a href='%s.php?action=list-%s'>list %s records</a></th></tr>\".display();\n",
				name, cptr->id, cptr->id );
		fprintf(h,"\"<tr><th colspan=2><a href='%s.php?action=menu-%s'>return to menu of %s</a></th></tr>\".display();\n",
				name, name, name );
		fprintf(h,"\"</table></div>\".display();\n");
		fprintf(h,"\"</form>\".display();\n");
		fprintf(h,"\"</body></html>\".display();\n");
		fprintf(h,"}\nelse\n{\n");
		fprintf(h,"include 'csp-list-%s.txt';\n",cptr->id);
		fprintf(h,"}\n");
		title(h,"end of file");
		fclose(h);
		return(0);
	}
}

/*	---------------------------------------------------	*/
/*	        g e n e r a t e _ c s p _ d e l e t e 		*/
/*	---------------------------------------------------	*/
private	int	generate_csp_delete( struct occi_category * cptr, char * name )
{
	struct	occi_attribute * iptr;
	FILE * h;
	char buffer[1024];
	sprintf(buffer,"csp-delete-%s.txt",cptr->id);
	if (!( h = fopen( buffer, "w" ) ))
		return( 46 );
	else
	{
		title(h,buffer);
		fprintf(h,"if ( $1.length() > 0 )\n{\n");
		fprintf(h,"\t$id = $1;\n");
		fprintf(h,"\t$id.delete();\n}\n");
		fprintf(h,"include 'csp-list-%s.txt';\n",cptr->id);
		title(h,"end of file");
		fclose(h);
		return(0);
	}
}

/*	---------------------------------------------------	*/
/*	        g e n e r a t e _ c s p _ m e n u     		*/
/*	---------------------------------------------------	*/
private	int	generate_csp_menu( struct occi_category * cptr, char * name )
{
	struct	occi_attribute * iptr;
	FILE * h;
	char buffer[1024];
	sprintf(buffer,"csp-menu-%s.txt",name);
	if (!( h = fopen( buffer, "w" ) ))
		return( 46 );
	else
	{
		title(h,buffer);
		fprintf(h,"\"<html><head><title>%s</title>\".display();\n",name);
		fprintf(h,"\"<link href='style.css' rel='STYLESHEET' type='text/css' media='SCREEN'>\".display();\n");
		fprintf(h,"\"</head><body>\".display();\n");
		fprintf(h,"\"<div align=center><table>\".display();\n");
		fprintf(h,"\"<tr><th class=title>Main Menu of %s</th></tr>\".display();\n",name);
		fprintf(h,"\"<tr><th>\".display();\n");
		fprintf(h,"\"<div align=center><table>\".display();\n");
		for ( 	;
			cptr != (struct occi_category *) 0;
			cptr = cptr->next )
			fprintf(h,"\"<tr><th><a href='%s.php?action=list-%s'>%s</a></th>\".display();\n",name,cptr->id,cptr->id);
		fprintf(h,"\"</tr>\".display();\n");
		fprintf(h,"\"</table></div>\".display();\n");
		fprintf(h,"\"<tr><th><hr><a href='http://www.compatibleone.org/'>www.compatiblone.org</a><hr></th></tr>\".display();\n");
		fprintf(h,"\"</table></div>\".display();\n");
		fprintf(h,"\"</body></html>\".display();\n");
		title(h,"end of file");
		fclose(h);
		return(0);
	}
}

/*	---------------------------------------------------	*/
/*	        g e n e r a t e _ c s p _ l i s t     		*/
/*	---------------------------------------------------	*/
private	int	generate_csp_list( struct occi_category * cptr, char * name )
{
	struct	occi_attribute * iptr;
	FILE * h;
	char buffer[1024];
	sprintf(buffer,"csp-list-%s.txt",cptr->id);
	if (!( h = fopen( buffer, "w" ) ))
		return( 46 );
	else
	{
		title(h,buffer);
		fprintf(h,"\"<html><head><title>%s</title>\".display();\n",buffer);
		fprintf(h,"\"<link href='style.css' rel='STYLESHEET' type='text/css' media='SCREEN'>\".display();\n");
		fprintf(h,"\"</head><body>\".display();\n");
		fprintf(h,"\"<div align=center><table>\".display();\n");
		fprintf(h,"\"<tr><th class=title>List of %s Records</th></tr>\".display();\n",cptr->id);
		fprintf(h,"\"<tr><th>\".display();\n");
		fprintf(h,"\"<div align=center><table><tr><th>id\".display();\n");
		for ( 	iptr= cptr->first;
			iptr != (struct occi_attribute *) 0;
			iptr = iptr->next )
		{
			fprintf(h,"\"<th>%s</th>\".display();\n",iptr->name);
		}
		fprintf(h,"\"</tr>\".display();\n");
	
		fprintf(h,"$l = %s.list();\n",cptr->id);
		fprintf(h,"foreach ( $l as $i )\n{\n",cptr->id);
		fprintf(h,"\t$r = $i.get();\n");
		fprintf(h,"\t\"<tr>\".display();\n");

		fprintf(h,"\t$v = $r['occi.core.id'];\n");
		fprintf(h,"\t$m = \"<td>\";\n");
		fprintf(h,"\t$m = $m#\"<a href='%s.php?action=retrieve-%s&id=\"#$i#\"'>\";\n",name,cptr->id);
		fprintf(h,"\t$m = $m#$v;\n");
		fprintf(h,"\t$m = $m#\"</a>\";\n");
		fprintf(h,"\t$m.display();\n");

		for ( 	iptr= cptr->first;
			iptr != (struct occi_attribute *) 0;
			iptr = iptr->next )
		{
			fprintf(h,"\t$v = $r['occi.%s.%s'];\n",cptr->id,iptr->name);
			fprintf(h,"\tif ( $v.length() == 0 ) { $v = ''; }\n");
			fprintf(h,"\telse { if ( $v == '(null)' ) { $v = ''; } }\n");
			fprintf(h,"\t$m = \"<td>\";\n");
			fprintf(h,"\t$m = $m#$v;\n");
			fprintf(h,"\t$m.display();\n");
		}

		fprintf(h,"}\n");
		fprintf(h,"\"</tr>\".display();\n");
		fprintf(h,"\"</table></div>\".display();\n");
		fprintf(h,"\"<tr><th><a href='%s.php?action=input-%s'>create new %s record</a></th></tr>\".display();\n",
				name, cptr->id, cptr->id );
		fprintf(h,"\"<tr><th><a href='%s.php?action=menu-%s'>return to menu of %s</a></th></tr>\".display();\n",
				name, name, name );

		fprintf(h,"\"</table></div>\".display();\n");
		fprintf(h,"\"</body></html>\".display();\n");
		title(h,"end of file");
		fclose(h);
		return(0);
	}
}

/*	---------------------------------------------------	*/
/*	g e n e r a t e _ s e r v i c e _ i n t e r f a c e 	*/
/*	---------------------------------------------------	*/
public	int	generate_service_interface( char * name, struct occi_category * cptr, char * prefix )
{
	FILE * h;
	struct	occi_attribute * iptr;
	char buffer[1024];
	sprintf(buffer,"%s.php",name);
	if (!( h = fopen( buffer, "w" ) ))
		return( 46 );
	else
	{
		fprintf(h,"<?php\n");
		sprintf(buffer,"web user interface for component : %s ",name);
		title(h,buffer);

		generate_csp_menu( cptr, name  );

		fprintf(h,"function compile_cordscript($s, $o)\n{\n");
			fprintf(h,"\t$a = array();\n");
			fprintf(h,"\t$result = exec('bash ./dashboard-command run '.$s.' \"'.$o.'\"',$a);\n");
			fprintf(h,"\tforeach ($a as $l )\n");
			fprintf(h,"\t{	print $l.\"\\n\";	}\n");
		fprintf(h,"\treturn;\n}\n");

		fprintf(h,"switch ( $_REQUEST['action'] )\n{\n");
		for (	;	
			cptr != (struct occi_category *) 0; 
			cptr = cptr->next )
		{
			generate_csp_input( cptr, name );
			generate_csp_create( cptr, name );
			generate_csp_retrieve( cptr, name  );
			generate_csp_update( cptr, name );
			generate_csp_delete( cptr, name  );
			generate_csp_list( cptr, name  );

			title( h, cptr->id );

			fprintf(h,"case 'input-%s' :\n",cptr->id);
			fprintf(h,"\t$args = '';\n");
			fprintf(h,"\tcompile_cordscript('csp-input-%s.txt', $args );\n",cptr->id);
			fprintf(h,"\tbreak;\n");

			fprintf(h,"case 'create-%s' :\n",cptr->id);
			fprintf(h,"\t$args = '';\n");

			for ( 	iptr= cptr->first;
				iptr != (struct occi_attribute *) 0;
				iptr = iptr->next )
				fprintf(h,"\t$args = $args.' '.$_REQUEST['%s'];\n",iptr->name);

			fprintf(h,"\tcompile_cordscript('csp-create-%s.txt', $args );\n",cptr->id);
			fprintf(h,"\tbreak;\n");

			fprintf(h,"case 'retrieve-%s' :\n",cptr->id);
			fprintf(h,"\t$args = $_REQUEST['id'];\n");
			fprintf(h,"\tcompile_cordscript('csp-retrieve-%s.txt', $args );\n",cptr->id);
			fprintf(h,"\tbreak;\n");

			fprintf(h,"case 'update-%s' :\n",cptr->id);
			fprintf(h,"\t$args = $_REQUEST['id'];\n");

			for ( 	iptr= cptr->first;
				iptr != (struct occi_attribute *) 0;
				iptr = iptr->next )
				fprintf(h,"\t$args = $args.' '.$_REQUEST['%s'];\n",iptr->name);

			fprintf(h,"\tcompile_cordscript('csp-update-%s.txt', $args );\n",cptr->id);
			fprintf(h,"\tbreak;\n");

			fprintf(h,"case 'delete-%s' :\n",cptr->id);
			fprintf(h,"\t$args = $_REQUEST['id'];\n");
			fprintf(h,"\tcompile_cordscript('csp-delete-%s.txt', $args );\n",cptr->id);
			fprintf(h,"\tbreak;\n");

			fprintf(h,"case 'list-%s' :\n",cptr->id);
			fprintf(h,"\t$args = '';\n");
			fprintf(h,"\tcompile_cordscript('csp-list-%s.txt', $args );\n",cptr->id);
			fprintf(h,"\tbreak;\n");


		}
		fprintf(h,"case 'menu-%s' :\n",name);
		fprintf(h,"default :\n");
		fprintf(h,"\t$args = '';\n");
		fprintf(h,"\tcompile_cordscript('csp-menu-%s.txt', $args );\n",name);
		fprintf(h,"\tbreak;\n");
		fprintf(h,"}\n");
		title(h,"end of file");
		fprintf(h,"?>\n");
		fclose(h);
		return( 0 );
	}
}

/*	---------------------------------------------------	*/
/*	  g e n e r a t e _ p r o c c i _ m a k e f i l e 	*/
/*	---------------------------------------------------	*/
public	int	generate_procci_makefile( char * name, struct occi_category * cptr, char * prefix )
{
	FILE * h;
	char buffer[1024];
	sprintf(buffer,"%s.c",name);
	if (!( h =fopen( buffer, "w" ) ))
		return( 46 );
	else
	{
		fprintf(h,"#\n# %s Makefile.am\n",name);
		fprintf(h,"#\n# Copyright 2013, All rights reserved\n");
		fprintf(h,"#\n# Authors: \n");
		fprintf(h,"#\tComodel Version 1.0a\n");
		fprintf(h,"#\tStrukt  Version 3.0a\n");
		fprintf(h,"#\n");
		fprintf(h,"bin_PROGRAMS = %s \n",name);
		fprintf(h,"extra_DIST = \n");
		fprintf(h,"%s_SOURCES = %s.c\n",name);
		fprintf(h,"%s_CFLAGS  = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR) $(CFLAGS_COMONS) $(CFLAGS_COCSPI)\n",name);
		fprintf(h,"%s_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCSPI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)\n",name);
		fclose(h);
		return(0);
	}
}

/*	-------------------------------------------------	*/
/*	g e n e r a t e _ p r o c c i _ c o m p o n e n t	*/
/*	-------------------------------------------------	*/
public	int	generate_procci_component( char * name, struct occi_category * cptr, int backend, char * prefix  )
{
	FILE * h;
	char buffer[1024];
	sprintf(buffer,"%s.c",name);
	if (!( h =fopen( buffer, "w" ) ))
		return( 46 );
	else
	{
		sprintf(buffer,"_%s_c",name);
		generate_file_exclusion(h,buffer,1);
		generate_file_inclusion(h,"standard.h");
		generate_file_inclusion(h,"rest.h");
		generate_file_inclusion(h,"occi.h");
		generate_file_inclusion(h,"document.h");
		generate_file_inclusion(h,"cordspublic.h");
		generate_file_inclusion(h,"occispublisher.h");
		generate_accords_config(h,name,prefix);
		
		generate_file_actions(h,cptr, prefix);
		generate_file_operation(h,name,cptr,prefix);
		generate_file_start(h,name);
		generate_file_main(h,name);
		generate_file_exclusion(h,buffer,2);
		fclose(h);
		return(0);
	}
}


#endif	/* _gencomp_c */
