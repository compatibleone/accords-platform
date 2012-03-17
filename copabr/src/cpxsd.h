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
#ifndef	_cpxsd_h
#define	_cpxsd_h

#define	_XSD_ELEMENT "xsd:element"
#define	_XSD_COMPLEX "xsd:complexType"
#define	_XSD_ATRIBUT "xsd:attribute"
#define	_XSD_SEQUENCE "xsd:sequence"
#define	_XSD_MAXOCCURS "maxOccurs"
#define	_XSD_MINOCCURS "minOccurs"
#define	_XSD_UNBOUNDED "unbounded"

#define	_XSD_DEFAULT "default"
#define	_XSD_NAME "name"
#define _XSD_TYPE "type"


public	struct	xml_element * xsd_type( struct xml_element * xsd, char * nptr );
public	struct	xml_element * first_xsd_element( struct xml_element * xsd );
public	struct	xml_element * xsd_element( struct xml_element * xsd, char * nptr );
public	struct	xml_element * xsd_element_type( struct xml_element * wptr );
public	struct	xml_element * xsd_atribut( struct xml_element * xsd, char * nptr );
public	struct 	xml_element * xsd_validate_element( 
	struct 	xml_element * xsd,
	struct 	xml_element * eptr );


	/* -------- */
#endif	/* _cpxsd_h */
	/* -------- */

