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
#ifndef	_certificate_c
#define	_certificate_c

#include "certificate.h"

public	char * 	azure_create_certificate( char * stub, int days )
{
	char 	cname[1024];
	char 	kname[1024];
	char 	dname[1024];
	char 	pname[1024];
	char 	command[16000];

	sprintf(cname,"%s/Certificate.pem",stub);
	sprintf(kname,"%s/PrivateKey.pem",stub);
	sprintf(dname,"%s/Certificate.cer",stub);
	sprintf(pname,"%s/Certificate.pfx",stub);

	/* create the directory */
	/* -------------------- */
	mkdir ( stub, 0640 );

	/* create the key */
	/* -------------- */
	sprintf(command,"openssl req -newkey 2048 -x509 -days %u -sha1",days);
	strcat( command, " -keyout ");
	strcat( command, kname);
	strcat( command, " -out ");
	strcat( command, cname);

	printf("launch: %s\n",command);
	system( command );

	/* protect the key */
	/* --------------- */
	sprintf(command,"chmod 0400 %s",kname);
	printf("launch: %s\n",command);
	system( command );

	/* -------------------------------------------- */
	/* convert the certificate to binary DER format */
	/* This can be used for the Management Portal   */
	/* -------------------------------------------- */
	strcpy( command, "openssl x509 -outform der" );
	strcat( command, " -in ");
	strcat( command, cname);
	strcat( command, " -out ");
	strcat( command, dname);

	printf("launch: %s\n",command);
	system( command );

	/* -------------------------------------------- */
	/* convert the certificate to binary DER format */
	/* This can be used for the Management Portal   */
	/* -------------------------------------------- */
	strcpy( command, "openssl pkcs12 ");
	strcat( command, " -inkey ");
	strcat( command, kname);
	strcat( command, " -in ");
	strcat( command, cname);
	strcat( command, " -export -out ");
	strcat( command, pname);

	printf("launch: %s\n",command);
	system( command );

	return( allocate_string( pname ) );
}

/*
 * TO DO .....
 * 1) the name of a directory into which the files will be created.
 * 2) the pass phrase
 * 3) the country two letter prefix
 * 4) the country name
 * 5) the town
 * 6) the service 
 * 7) the certificate owner
 * 8) the email contact address of the certificate owner.
 */





#endif	/* _certificate_c */
	/* -------------- */

