#ifndef	_gencomp_c
#define	_gencomp_c

#include "standard.h"
#include "occi.h"

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
private	void	generate_accords_config( FILE * h, char * nptr )
{
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
	fprintf(h,"\t%c%s%c,\n",0x0022,"europe",0x0022);
	fprintf(h,"\t%c%s.xml%c,\n",0x0022,nptr,0x0022);
	fprintf(h,"\t%c%s%c,\n",0x0022,"storage",0x0022);
	fprintf(h,"\t%c%s%c,\n",0x0022,nptr,0x0022);
	fprintf(h,"\t(struct occi_category *) 0,\n");
	fprintf(h,"\t(struct occi_category *) 0\n");
	fprintf(h,"\t};\n");


	fprintf(h,"public int check_debug() { return( Configuration.debug ); }\n");
	fprintf(h,"public int check_verbose() { return( Configuration.verbose ); }\n");
	fprintf(h,"public char * default_publisher() { return(Configuration.publisher); }\n");
	fprintf(h,"public char * default_operator() { return(Configuration.operator); }\n");
	fprintf(h,"public char * default_tls() { return(Configuration.tls); }\n");
	fprintf(h,"public char * default_zone() { return(Configuration.zone); }\n");

	fprintf(h,"public int %s_failure( int e, char * m1, char * m2 )\n{\n",nptr);
	fprintf(h,"\tif ( e )\n{\n");
	fprintf(h,"\t\tprintf(\"\\n*** failure \%u\",e);\n");
	fprintf(h,"\t\tif ( m1 ) printf(\" : \%s\",m1);\n");
	fprintf(h,"\t\tif ( m2 ) printf(\" : \%s\",m2);\n");	
	fprintf(h,"\t\tprintf(\" **\\n\");\n");
	fprintf(h,"\t}\nreturn( e );\n}\n");
	fprintf(h,"private void	%s_configuration()\n{\n",nptr);
	fprintf(h,"\tload_accords_configuration( &Configuration, \"%s\");\n",nptr);
	fprintf(h,"\treturn;\n}\n");

	fprintf(h,"private int %s_banner()\n{\n",nptr);
	fprintf(h,"printf(\"\\n   CompatibleOne Generated Service %s: Version 1.0a.0.01\");\n",nptr);
	fprintf(h,"printf(\"\\n   Beta Version : 12/04/2013 \");\n",nptr);
	fprintf(h,"printf(\"\\n   Copyright (c) 2013 Iain James Marshall\");\n",nptr);
	fprintf(h,"printf(\"\\n\");\n",nptr);
	fprintf(h,"accords_configuration_options();\n",nptr);
	fprintf(h,"printf(\"\\n\\n\");\n");
	fprintf(h,"return(0);\n}\n",nptr);

	return;
}

/*	--------------------------------------------------	*/
/*	g e n e r a t e _ c a t e g o r y  _ a c t i o n s 	*/
/*	--------------------------------------------------	*/
private	void	generate_category_actions( FILE * h, struct occi_category * cptr )
{
	struct	occi_action * aptr;
	char *	nptr;
	char	buffer[1024];

	sprintf(buffer,"%s.h",cptr->id);
	generate_file_inclusion(h,buffer);
	sprintf(buffer,"%s.c",cptr->id);
	generate_file_inclusion(h,buffer);
	sprintf(buffer,"occi%s.c",cptr->id);
	generate_file_inclusion(h,buffer);

	fprintf(h,"/* Actions of Category : %s */\n",cptr->id);
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
			else
			{
				fprintf(h,"private struct rest_response * _%s_%s(\n",nptr,cptr->id);
				fprintf(h,"\tstruct occi_category * optr,\n");
				fprintf(h,"\tstruct rest_client * cptr,\n");
				fprintf(h,"\tstruct rest_request * rptr,\n");
				fprintf(h,"\tstruct rest_response * aptr )\n{\n");
				fprintf(h,"\treturn( rest_response( aptr, 200, \"OK\" ) );\n");
				fprintf(h,"}\n");
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
	fprintf(h,"\tif (!( optr = occi_%s_builder( Configuration.domain, %c%s%c ) ))\n",cptr->id,0x0022,cptr->id,0x0022);
	fprintf(h,"\t\treturn( 27 )\n");
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
			fprintf(h,"\t\treturn( 27 )\n");
		}
	}
 	return;
}

/*	--------------------------------------------------	*/
/*	  g e n e r a t e _ f i l e _ a c t i o n s 		*/
/*	--------------------------------------------------	*/
private	void	generate_file_actions( FILE * h, struct occi_category * cptr )
{
	for (;	
		cptr != (struct occi_category *) 0; 
		cptr = cptr->next )
		generate_category_actions(h, cptr );
	return;
}

/*	--------------------------------------------------	*/
/*	  g e n e r a t e _ f i l e _ o p e r a t i o n		*/
/*	--------------------------------------------------	*/
private	void	generate_file_operation( FILE * h, char * nptr, struct occi_category * cptr )
{
	fprintf(h,"\nprivate int %s_operation( char * sptr )\n{\n",nptr);
	fprintf(h,"\tstruct occi_category * first=(struct occi_category *) 0;\n");
	fprintf(h,"\tstruct occi_category * last=(struct occi_category *) 0;\n");
	fprintf(h,"\tstruct occi_category * optr=(struct occi_category *) 0;\n");
	fprintf(h,"\tset_autosave_cords_xlink_name(\"links_%S.xml\");\n",nptr);
	fprintf(h,"\trest_initialise_log( Configuration.monitor );\n");
	for (	;	
		cptr != (struct occi_category *) 0; 
		cptr = cptr->next )
	{
		generate_add_category(h, cptr );
	}
	fprintf(h,"\tif (!( Configuration.identity ))\n");
	fprintf(h,"\t\tocci_server(sptr,Configuration.restport, Configuration.tls,Configuration.threads, first, (char *) 0 ) );\n");
	fprintf(h,"\telse\n\t{\n");
	fprintf(h,"\t\tinitialise_occi_publisher(Configuration.publisher,(char *) 0,(char *) 0,(char *) 0);\n");
	fprintf(h,"\t\treturn( publishing_occi_server(Configuration.user, Configuration.password,Configuration.identity,sptr,Configuration.restport,Configuration.tls,Configuration.threads,first));\n");
	fprintf(h,"\t}\n");
	fprintf(h,"\treturn(0);\n");
	fprintf(h,"}\n");
	return;
}

/*	--------------------------------------------------	*/
/*		g e n e r a t e _ f i l e _ s t a r t 		*/
/*	--------------------------------------------------	*/
private	void	generate_file_start( FILE * h, char * nptr )
{
	fprintf(h,"\nprivate int %s( int argc, char * argv[] )\n{\n",nptr);
	fprintf(h,"\tint argi=1;\n");
	fprintf(h,"\tchar * aptr;\n");
	fprintf(h,"\t%s_configuration();\n",nptr);
	fprintf(h,"\twhile ( argi < argc ) {\n");
	fprintf(h,"\t\tif (!( aptr = argv[argi++] )) break;\n");
	fprintf(h,"\t\telse if ( *aptr != '-' ) return( %s_operation( aptr ) );\n",nptr);
	fprintf(h,"\t\telse if ( *(++aptr) = '-' ) return( %s_failure( 30, aptr, \"\" ) );\n",nptr);
	fprintf(h,"\t\telse if (!( argi = accords_configuration_option(aptr, argi, argv) )) break;\n");
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
	fprintf(h,"\npublic int main( int argc, char * argv[] )\n{\n");
	fprintf(h,"\tif ( argc == 1 ) return( banner() );\n");
	fprintf(h,"\telse return( %s(argc,argv) );\n",nptr);
	fprintf(h,"}\n");
	return;
}

/*	---------------------------------------------------	*/
/*	g e n e r a t e _ s e r v i c e _ c o m p o n e n t	*/
/*	---------------------------------------------------	*/
public	int	generate_service_component( char * name, struct occi_category * cptr )
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
		generate_accords_config(h,name);

		generate_file_actions(h,cptr);
		generate_file_operation(h,name,cptr);
		generate_file_start(h,name);
		generate_file_main(h,name);
		generate_file_exclusion(h,buffer,2);
		fclose(h);
		return(0);
	}
}

/*	-------------------------------------------------	*/
/*	g e n e r a t e _ p r o c c i _ c o m p o n e n t	*/
/*	-------------------------------------------------	*/
public	int	generate_procci_component( char * name, struct occi_category * cptr  )
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
		generate_accords_config(h,name);
		
		generate_file_actions(h,cptr);
		generate_file_operation(h,name,cptr);
		generate_file_start(h,name);
		generate_file_main(h,name);
		generate_file_exclusion(h,buffer,2);
		fclose(h);
		return(0);
	}
}

#endif	/* _gencomp_c */
