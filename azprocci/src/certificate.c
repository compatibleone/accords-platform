* -------------------------------------------------------------------- */
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

