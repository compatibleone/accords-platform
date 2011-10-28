/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons PaternitÇ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */
#include "standard.h"
#include "types.h"


struct	macro_conversions {
	int	code;
	char *	macro;
	};

#define	MAXMACMAN	25
struct	macro_conversions MacMan[MAXMACMAN] = {
		0x0085 /* 'Ö' */	,	(char *) "&agrave;", 
		0x00A0 /* '†' */	,	(char *) "&aacute;", 
		0x0083 /* 'É' */	,	(char *) "&acirc;" , 
		0x008A /* 'ä' */	,	(char *) "&egrave;", 
		0x0082 /* 'Ç' */	,	(char *) "&eacute;", 
		0x0088 /* 'à' */	,	(char *) "&ecirc;" , 
		0x008D /* 'ç' */	,	(char *) "&igrave;", 
		0x00A1 /* '°' */	,	(char *) "&iacute;", 
		0x008C /* 'å' */	,	(char *) "&icirc;" , 
		0x0095 /* 'ï' */	,	(char *) "&ograve;", 
		0x00A2 /* '¢' */	,	(char *) "&oacute;", 
		0x0093 /* 'ì' */	,	(char *) "&ocirc;" , 
		0x0097 /* 'ó' */	,	(char *) "&ugrave;", 
		0x00A3 /* '£' */	,	(char *) "&uacute;", 
		0x0096 /* 'ñ' */	,	(char *) "&ucirc;", 
		0x0087 /* 'á' */	,	(char *) "&ccedil;",
		0x0081 /* 'Å' */	,	(char *) "&uuml;",
		0x0084 /* 'Ñ' */	,	(char *) "&auml;",
		0x0094 /* 'î' */	,	(char *) "&ouml;",
		0x009A /* 'U' */	,	(char *) "&Uuml;",
		0x008E /* 'A' */	,	(char *) "&Auml;",
		0x0099 /* 'O' */	,	(char *) "&Ouml;",

		/* these are required for XML production but not HTML production */
		/* ------------------------------------------------------------- */
		0x003C /* '<' */	,	(char *) "&lt;",
		0x003E /* '>' */	,	(char *) "&gt;",
		0x0026 /* '&' */	,	(char *) "&amp;"
		};

private	int	MaxMacMan=(MAXMACMAN-3);
public	void	use_html_macros()
{
	MaxMacMan = (MAXMACMAN-3);
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
		case	0x0085 /* 'Ö' */	:	return("&agrave;"); 
		case	0x00A0 /* '†' */	:	return("&aacute;"); 
		case	0x0083 /* 'É' */	:	return("&acirc;" ); 
		case	0x008A /* 'ä' */	:	return("&egrave;"); 
		case	0x0082 /* 'Ç' */	:	return("&eacute;"); 
		case	0x0088 /* 'à' */	:	return("&ecirc;" ); 
		case	0x008D /* 'ç' */	:	return("&igrave;"); 
		case	0x00A1 /* '°' */	:	return("&iacute;"); 
		case	0x008C /* 'å' */	:	return("&icirc;" ); 
		case	0x0095 /* 'ï' */	:	return("&ograve;" ); 
		case	0x00A2 /* '¢' */	:	return("&oacute;" ); 
		case	0x0093 /* 'ì' */	:	return("&ocirc;" ); 
		case	0x0097 /* 'ó' */	:	return("&ugrave;" ); 
		case	0x00A3 /* '£' */	:	return("&uacute;" ); 
		case	0x0096 /* 'ñ' */	:	return("&ucirc;" ); 
		case	0x0087 /* 'á' */	:	return("&ccedil;" ); 
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


