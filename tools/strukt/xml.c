/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons Paternit‚ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */
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


