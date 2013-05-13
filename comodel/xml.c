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
#ifndef	_xml_c
#define	_xml_c

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

#endif	/* _xml_c */
	/* ------ */

