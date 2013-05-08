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
#ifndef	_occi_c
#define	_occi_c

void	generate_occi_request( FILE * h, char * nptr )
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

#endif	/* _occi_c */
	/* ------- */

