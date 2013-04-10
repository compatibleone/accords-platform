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



	return;
}

/*	--------------------------------------------------	*/
/*	g e n e r a t e _ c a t e g o r y  _ a c t i o n s 	*/
/*	--------------------------------------------------	*/
private	void	generate_category_actions( FILE * h, struct occi_category * cptr )
{
	struct	occi_action * aptr;
	char *	nptr;
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
	for (;	
		cptr != (struct occi_category *) 0; 
		cptr = cptr->next )
		generate_add_category(h, cptr );

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
	/* ---------- */
