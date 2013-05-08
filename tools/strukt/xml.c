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


