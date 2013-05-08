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


