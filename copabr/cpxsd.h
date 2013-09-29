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
#ifndef	_cpxsd_h
#define	_cpxsd_h

#define	_XSD_ELEMENT "xsd:element"
#define	_XSD_COMPLEX "xsd:complexType"
#define	_XSD_ATRIBUT "xsd:attribute"
#define	_XSD_SEQUENCE "xsd:sequence"
#define	_XSD_CHOICE "xsd:choice"
#define	_XSD_MAXOCCURS "maxOccurs"
#define	_XSD_MINOCCURS "minOccurs"
#define	_XSD_UNBOUNDED "unbounded"

#define	_XSD_DEFAULT "default"
#define	_XSD_NAME "name"
#define _XSD_TYPE "type"


public	struct	xml_element * xsd_type( struct xml_element * xsd, char * nptr );
public	struct	xml_element * first_xsd_element( struct xml_element * xsd );
public	struct	xml_element * xsd_element( struct xml_element * xsd, char * nptr, struct xml_element * xsdroot );
public	struct	xml_element * xsd_element_type( struct xml_element * wptr, struct xml_element * xsdroot );
public	struct	xml_element * xsd_atribut( struct xml_element * xsd, char * nptr );
public	struct 	xml_element * xsd_validate_element( 
	struct 	xml_element * xsd,
	struct 	xml_element * eptr,
	struct xml_element  * xsdroot );


	/* -------- */
#endif	/* _cpxsd_h */
	/* -------- */

