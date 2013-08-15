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
#include "standard.h"
#include "types.h"


struct	macro_conversions {
	int	code;
	char *	macro;
	};

#define	MAXMACMAN	26
struct	macro_conversions MacMan[MAXMACMAN] = {
		0x0085 /* '�' */	,	(char *) "&agrave;", 
		0x00A0 /* '�' */	,	(char *) "&aacute;", 
		0x0083 /* '�' */	,	(char *) "&acirc;" , 
		0x008A /* '�' */	,	(char *) "&egrave;", 
		0x0082 /* '�' */	,	(char *) "&eacute;", 
		0x0088 /* '�' */	,	(char *) "&ecirc;" , 
		0x008D /* '�' */	,	(char *) "&igrave;", 
		0x00A1 /* '�' */	,	(char *) "&iacute;", 
		0x008C /* '�' */	,	(char *) "&icirc;" , 
		0x0095 /* '�' */	,	(char *) "&ograve;", 
		0x00A2 /* '�' */	,	(char *) "&oacute;", 
		0x0093 /* '�' */	,	(char *) "&ocirc;" , 
		0x0097 /* '�' */	,	(char *) "&ugrave;", 
		0x00A3 /* '�' */	,	(char *) "&uacute;", 
		0x0096 /* '�' */	,	(char *) "&ucirc;", 
		0x0087 /* '�' */	,	(char *) "&ccedil;",
		0x0081 /* '�' */	,	(char *) "&uuml;",
		0x0084 /* '�' */	,	(char *) "&auml;",
		0x0094 /* '�' */	,	(char *) "&ouml;",
		0x009A /* 'U' */	,	(char *) "&Uuml;",
		0x008E /* 'A' */	,	(char *) "&Auml;",
		0x0099 /* 'O' */	,	(char *) "&Ouml;",

		/* these are required for XML production but not HTML production */
		/* ------------------------------------------------------------- */
		0x003C /* '<' */	,	(char *) "&lt;",
		0x003E /* '>' */	,	(char *) "&gt;",
		0x0026 /* '&' */	,	(char *) "&amp;",
		0x0022 /* '"' */	,	(char *) "&quot;"
		};

private	int	MaxMacMan=(MAXMACMAN-4);
public	void	use_html_macros()
{
	MaxMacMan = (MAXMACMAN-4);
	return;
}

public	void	use_xml_macros()
{
	MaxMacMan = MAXMACMAN;
	return;
}


private	char *	html_accent(int c)
{
	int	i;
	for ( i=0; i < MaxMacMan; i++ )
		if ((c & 0x00FF) == (MacMan[i].code & 0x00FF))
			return( MacMan[i].macro );
	return( (char *) 0 );
}


private	int		html_macro(char * sptr)
{
	int	i;
	for ( i=0; i < MaxMacMan; i++ )
		if (!( strcmp( sptr, MacMan[i].macro ) ))
			return( (MacMan[i].code & 0x00FF) );
	return(0);
}


private	char *	old_html_accent(int c)
{
	switch ( (c & 0x00FF) ) {
		case	0x0085 /* '�' */	:	return("&agrave;"); 
		case	0x00A0 /* '�' */	:	return("&aacute;"); 
		case	0x0083 /* '�' */	:	return("&acirc;" ); 
		case	0x008A /* '�' */	:	return("&egrave;"); 
		case	0x0082 /* '�' */	:	return("&eacute;"); 
		case	0x0088 /* '�' */	:	return("&ecirc;" ); 
		case	0x008D /* '�' */	:	return("&igrave;"); 
		case	0x00A1 /* '�' */	:	return("&iacute;"); 
		case	0x008C /* '�' */	:	return("&icirc;" ); 
		case	0x0095 /* '�' */	:	return("&ograve;" ); 
		case	0x00A2 /* '�' */	:	return("&oacute;" ); 
		case	0x0093 /* '�' */	:	return("&ocirc;" ); 
		case	0x0097 /* '�' */	:	return("&ugrave;" ); 
		case	0x00A3 /* '�' */	:	return("&uacute;" ); 
		case	0x0096 /* '�' */	:	return("&ucirc;" ); 
		case	0x0087 /* '�' */	:	return("&ccedil;" ); 
		default				:	return((char *) 0);
		}
}

public	void	convert_html_accents( char **mptr, int mode )
{
	int	macros=0;
	char	macname[32];
	int	c;
	int	i;
	int	j;
	int	m;
	char *	sptr=(char *) 0;
	char *	rptr=(char *) 0;
	char *	wptr=(char *) 0;
	char *	aptr=(char *) 0;
	int	slen=0;
	int	rlen=0;
	int	accents=0;
	if (!( sptr = *mptr ))
		return;
	else if (!(slen = strlen( sptr )))
		return;

	switch ( mode ) {
		case 	2	:
			/* convert from accents to macros */
			/* ------------------------------ */

			/* Calculate new string size ( with macros ) */
			/* ----------------------------------------- */
			for ( accents=0, i=0; i < slen; i++ ) {
				if (!( aptr = html_accent( ( *(sptr+i) & 0x00FF ) ) )) 
					rlen++;
				else	{
					rlen += strlen( aptr );
					accents++;
					}
				}
			if (!( accents ))
				break;
			else	rlen++;
			if (!( rptr = allocate( rlen ) ))
				break;
			else	wptr = rptr;

			for (i=0; i < slen; i++ ) {
				if (!( aptr = html_accent( ( *(sptr+i) & 0x00FF ) ) )) {
					sprintf(wptr,"%c",(*(sptr+i) & 0x00FF));
					wptr++;
					}
				else	{
					sprintf(wptr,"%s",aptr);
					wptr  += strlen( aptr );
					}
				}
			liberate( sptr );
			*mptr = rptr;
			break;

		case	1	:
			/* convert from macros to accents */
			/* Calculate new string size ( with macros ) */
			/* ----------------------------------------- */
			for ( j=0,i=0; i < slen; i++ ) {
				if ((c = *(sptr+i)) == '&' ) {
					for (m=0; m < 31; m++ )
						if ((macname[m] = *(sptr+i+m)) == ';' )
							break;
					if ( macname[m] == ';' )
						m++;
					macname[m] = 0;
					if ((c = html_macro( macname )) != 0 ) {
						i += (m-1);
						macros++;
						}
					}
				*(sptr+j)=c;
				j++;
				}
			if ( macros )
				if ( j < slen )
					*(sptr+j) = 0;

		}
	return;
}


