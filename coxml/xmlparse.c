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
#ifndef	_xml_parse_c
#define	_xml_parse_c

#include <stdio.h>
#include <stdlib.h>

#include "allocate.h"
#include "xmlparse.h"

/*	------------------------------------	*/
/*	HTML/XML Special Character Mnemonics 	*/
/*	------------------------------------	*/

#define	MAX_MNEMONIC	7

static	char *	Mnemonix[MAX_MNEMONIC] = {
	(char *) "lt",
	(char *) "gt",
	(char *) "amp",
	(char *) "agrave",
	(char *) "egrave",
	(char *) "eacute",
	(char *) "ccedil"
	};

static	int	Mnemonich[MAX_MNEMONIC] = {
	'<',	'>',	'&',
	'\x85',	'\x8a',	'\x82',	'\x87'
	};


/*	-----------------------		*/
/*	XML PRODUCTION ROUTINES		*/
/*	-----------------------		*/
void	xml_linefeed(FILE *h)
{
#ifdef	UNIX
	fprintf(h,"\n");
#else
	fprintf(h,"\r\n");
#endif
}

static	char * duplicate_string( char * sptr )
{
	char *	rptr;
	if ((rptr = allocate( strlen( sptr ) + 1 )) != (char * )0)
		strcpy(rptr,sptr);
	return(rptr);
}


void	xml_put_string(FILE * h, char * name, char * value, int margin)
{
	if ((!( name )) || (!( value )))
		return;
	indent(h,margin);
	fprintf(h,"<%s>%s</%s>",name,value,name);
	xml_linefeed(h);
	return;
}

void	xml_put_filename( FILE * h, char * value)
{
	int	c;
	while ((c = (*(value++) & 0x00FF)) != 0) {
#ifdef	WIN32
		if ( c == '\\' )
			c = '/';
#endif
		fputc( c, h );
		}		
}

void	xml_put_filename_string(FILE * h, char * name, char * value, int margin)
{
	int	c;
	if ((!( name )) || (!( value )))
		return;
	indent(h,margin);
	fprintf(h,"<%s>",name);
	xml_put_filename(h,value);
	fprintf(h,"</%s>",name);
	xml_linefeed(h);
	return;
}


void	xml_put_cdata(FILE * h, char * sptr )
{
	char *	cptr;
	if (!( sptr ))
		return;
	else if (!( cptr = allocate_string( sptr ) ))
		return;
	else	{
		use_xml_macros();
		convert_html_accents( &cptr, 2 );
		use_html_macros();
		fprintf(h,"%s",cptr);
		liberate( cptr );
		return;
		}
}

#ifdef	OLD_CDATA_CONVERSION
void	xml_put_cdata(FILE * h, char * sptr )
{
	int	c;
	int	m;
	while ( *sptr ) {
		c = *(sptr++);
		for ( m=0; m < MAX_MNEMONIC; m++ ) {
			if ( Mnemonich[m] == c ) {
				fprintf(h,"&%s;",Mnemonix[m]);
				break;
				}
			}
		if ( m < MAX_MNEMONIC )
			continue;
		else	{
			fprintf(h,"%c",c);
			continue;
			}
		}
	return;
}
#endif

void	xml_put_word(FILE * h, char * name, int value, int margin)
{
	indent(h,margin);
	fprintf(h,"<%s>%u</%s>",name,value,name);
	xml_linefeed(h);
	return;
}


/*	-----------------------------------------	*/
/*	XML APPLICATION LEVEL : RETRIEVAL METHODS	*/
/*	-----------------------------------------	*/

int	xml_check_buffer(struct xml_application * xptr, int n)
{
	char *	mptr;
	if ((xptr->offset+n) < xptr->buflen)
		return(0);
	else if (!( mptr = allocate( (xptr->buflen + 8192) ) ))
		return(27);
	else	{
		xptr->buflen+= 8192;
		memcpy( mptr, xptr->buffer, xptr->offset );
		liberate( xptr->buffer );
		xptr->buffer = mptr;
		return(0);
		}
}

int	xml_reset(struct xml_application * xptr, int e, void * payload )
{
	char *	eptr;
	int	i;
	if (!(xptr->buffer))
		if (!(xptr->buffer = allocate((xptr->buflen = 8192))))
			return( 27 );

	if ((eptr = getenv("XMLSTRICT")) != (char*) 0)
		xptr->strict = *eptr;
	else	xptr->strict = 0;
	xptr->echo  = e;
	xptr->offset= 0;
	xptr->nested= 0;
	xptr->parent=(struct xml_parser *) 0;
	for ( i=0; i < _XML_MAX_PAYLOAD; i++ )
		xptr->payload[0] = (void*)0;
	xptr->payload[0] = payload;
	return(0);
}

int	xml_mnemonic( char * sptr, char * mptr )
{
	int	i;
	for ( i=0; *(mptr+i) != 0; i++ )
		if ( *(sptr+i) != *(mptr+i) )
			return(0);
	return(1);
}

void	xml_get_word(struct xml_application * xptr, int * iptr )
{
	char *	sptr = xptr->buffer;
	while ( *sptr == ' ' ) sptr++;
	*iptr = 0;
	while ((*sptr >='0') && (*sptr <='9'))
		*iptr = (( *iptr * 10 ) + ( *(sptr++) - '0' ));

	return;
}

void	xml_get_xy(struct xml_application * xptr, int * a, int * b )
{
	int	v;
	char *	sptr = xptr->buffer;
	while ( *sptr == ' ' ) sptr++;
	v = 0;
	while ((*sptr >='0') && (*sptr <='9'))
		v = (( v * 10 ) + ( *(sptr++) - '0' ));
	*a = v;
	while ( *sptr == ' ' ) sptr++;
	v = 0;
	while ((*sptr >='0') && (*sptr <='9'))
		v = (( v * 10 ) + ( *(sptr++) - '0' ));
	*b = v;
	return;
}

void	xml_get_xyz(struct xml_application * xptr, int * a, int * b, int * c )
{
	int	v;
	char *	sptr = xptr->buffer;
	while ( *sptr == ' ' ) sptr++;
	v = 0;
	while ((*sptr >='0') && (*sptr <='9'))
		v = (( v * 10 ) + ( *(sptr++) - '0' ));
	*a = v;
	while ( *sptr == ' ' ) sptr++;
	v = 0;
	while ((*sptr >='0') && (*sptr <='9'))
		v = (( v * 10 ) + ( *(sptr++) - '0' ));
	*b = v;
	while ( *sptr == ' ' ) sptr++;
	v = 0;
	while ((*sptr >='0') && (*sptr <='9'))
		v = (( v * 10 ) + ( *(sptr++) - '0' ));
	*c = v;
	return;
}

void	xml_get_string(struct xml_application * xptr, char ** target )
{
	char *	sptr;
	if ((sptr = *target) != (char *) 0)
		liberate( sptr );

	sptr = duplicate_string( xptr->buffer );
	*target = sptr;
	return;
}

void	xml_get_filename_string(struct xml_application * xptr, char ** target )
{
	int	i;
	char *	sptr;
	if ((sptr = *target) != (char *) 0)
		liberate( sptr );

	sptr = duplicate_string( xptr->buffer );
#ifdef	WIN32
	for (i=0; *(sptr+i) != 0; i++ )
		if ( *(sptr+i) == '/' )
			*(sptr+i) = '\\';
#else
	for (i=0; *(sptr+i) != 0; i++ )
		if ( *(sptr+i) == '\\' )
			*(sptr+i) = '/';
#endif
	*target = sptr;
	return;
}


void	xml_get_text(struct xml_application * xptr, char ** rptr )
{
	char *	cptr;
	if (( cptr = allocate_string( xptr->buffer )) != (char *) 0) {
		use_xml_macros();
		convert_html_accents( &cptr, 1 );
		use_html_macros();
		}
	*rptr = cptr;
}

#ifdef	OLD_CDATA_CONVERSION
void	xml_get_text(struct xml_application * xptr, char ** rptr )
{
	char *	sptr;
	int	s;
	int	t;
	int	m;
	sptr=xptr->buffer;
	s=0;	t=0;
	while ( *(sptr+s) != 0 ) {
		if ( *(sptr+s) != '&' ) {
			*(sptr+t) = *(sptr+s);
			s++; t++;
			}
		else	{
			for ( m=0; m < MAX_MNEMONIC; m++ ) {
				if ( xml_mnemonic( (sptr+s+1) , Mnemonix[m] ) ) {
					s += (strlen( Mnemonix[m] ) + 2);
					*(sptr+t) = Mnemonich[m];
					t++;
					break;
					}
				}
			if ( m < MAX_MNEMONIC )
				continue;
			else	{
				*(sptr+t) = *(sptr+s);
				s++; t++;
				continue;
				}
			}
		}
	*(sptr+t) = 0;
	sptr = duplicate_string( xptr->buffer );
	*rptr = sptr;
	return;
}
#endif

/*	-------------------------------------		*/
/*	S T A N D A R D   X M L   P A R S E R		*/
/*	-------------------------------------		*/

static	int	xml_echo_state=0;

void	set_xml_echo(int v) { xml_echo_state = v; }

static	int	xml_open_tag(struct xml_application * xptr,char * sptr) 
{ 
	if ( xptr->echo )
		printf("<%s>",sptr); 	
	return(0); 
}

static	int	xml_atb_name(struct xml_application * xptr,char * sptr)	
{ 
	if ( xptr->echo )
		printf(" %s",sptr);	
	return(0); 
}

static	int	xml_atb_value(struct xml_application * xptr,char * sptr)
{ 
	if ( xptr->echo )
		printf("=%s",sptr);
	return(0); 
}

static	int	xml_white_space(struct xml_application * xptr, int c )
{
	if ( xptr->echo )
		printf("%c",c);	
	xml_check_buffer(xptr,1);
	*(xptr->buffer+xptr->offset) = c;
	xptr->offset++;
	return(0);
}

static	int	xml_data_punctuation(struct xml_application * xptr, int c )
{
	if ( xptr->echo )
		printf("%c",c);	
	xml_check_buffer(xptr,1);
	*(xptr->buffer+xptr->offset) = c;
	xptr->offset++;
	return(0);
}

static	int	xml_data_token(struct xml_application * xptr, char * tptr)
{
	if ( xptr->echo )
		printf("%s",tptr);	
	xml_check_buffer(xptr,strlen(tptr));
	strcpy((xptr->buffer+xptr->offset),tptr);
	xptr->offset += strlen( tptr );
	return(0);
}

static	int	xml_close_tag(struct xml_application * xptr)
{
	if ( xptr->echo )
		printf("</end>");
  	return(0); 
}

static	int	xml_failure( char * nptr, int status, int nblin )
{
	return( status );
}

struct	xml_application * liberate_xml_application( struct xml_application * aptr )
{
	if ( aptr ) {
		if ( aptr->buffer )
			liberate( aptr->buffer );
		liberate( aptr );
		}
	return((struct xml_application *) 0);
}

struct	xml_parser * xml_close( struct xml_parser * h )
{
	if ( h ) {
		if ( h->handle )
			fclose(h->handle);
		if ( h->name )
			liberate( h->name );
		if ( h->user )
			liberate( h->user );
		if ( h->work )
			liberate_xml_application( h->work );
		liberate( h );
		}
	return((struct xml_parser *) 0);
}

static	struct	xml_parser * xml_open( char * nptr, void * payload )
{
	struct	xml_application	*	xptr;
	struct	xml_parser	*	sptr;
	FILE * h;
	if (!( h = fopen(nptr,"r")))
		return((struct xml_parser*) 0);

	else if (!( sptr = allocate( sizeof( struct xml_parser ) ) )) {
		fclose(h);
		return( sptr);
		}
	else	{
		sptr->handle 		= h;
		sptr->lines  		= 0;
		sptr->length 		= 0;
		sptr->column 		= 0;
		sptr->ungotch		= 0;
		sptr->punctuation	= 0;
		sptr->state 		= _TAG_IDLE;
		sptr->nesting 		= 0;
		sptr->echo 		= xml_echo_state;
		sptr->name 		= (char *) 0;
		sptr->work		= (struct xml_application *) 0;
		sptr->user		= (struct xml_relay *) 0;
		if (!( sptr->work = allocate( sizeof ( struct xml_application ) ) ))
			return( xml_close( sptr ) );
		else	{
			xml_reset(sptr->work,xml_echo_state, payload);
			if (!( sptr->name = duplicate_string( nptr ) ))
				return( xml_close( sptr ) );
			else if (!(sptr->user = allocate( sizeof( struct xml_relay ) )))
				return( xml_close( sptr ) );
			else	{
				sptr->user->open_tag	=	xml_open_tag;
				sptr->user->atb_name	=	xml_atb_name;
				sptr->user->atb_value	=	xml_atb_value;
				sptr->user->white_space	=	xml_white_space;
				sptr->user->token	=	xml_data_token;
				sptr->user->punctuation	=	xml_data_punctuation;
				sptr->user->close_tag	=	xml_close_tag;
				sptr->user->failure     =	xml_failure;
				return( sptr );
				}
			}
		}
}

static	void	xml_ungetch(
	struct	xml_parser	*	sptr,
	int				c
	)
{
	sptr->ungotch = c;
	return;
}

static	int	xml_getch( struct xml_parser * sptr )
{
	int	c;
	if ((c = sptr->ungotch) != 0) {
		sptr->ungotch = 0;
		return(c);
		}
	while(1) {
		switch ((c = fgetc(sptr->handle))) {
			case	-1	:
			case	0	: return( 0 );
			case	'\r'	: continue;
			case	'\n'	:
				sptr->lines++;
				sptr->column=0;
				if ( sptr->echo )
					printf("%c",c);
				return(c);
			case	'\t'	:
			default		:
				sptr->column++;
				if ( sptr->echo )
					printf("%c",c);
				return((c & 0x00FF));					
			}
		}

}

static	int	xml_remove_space(
	struct	xml_parser	*	sptr
	)
{
	int	c;
	while ((c = xml_getch(sptr)) != 0) {
		switch ( c ) {
			case	'\n'	:
			case	' '	:
			case	'\r'	:
			case	'\t'	:
				if ( sptr->state == _TAG_OPEN )
					(sptr->user->white_space)(sptr->work,c);
				continue;
			default		:
				xml_ungetch(sptr,c);
				return(c);
			}			
		}
	return(0);

}

#define	macro_xml_token_character(c) if ( sptr->length >= (_XML_MAX_TOKEN_SIZE-1)) { return(-1); } else	{ sptr->token[sptr->length++]=c; }

static	int	xml_get_token( 
	struct	xml_parser	*	sptr
	)
{
	int	quoting=0;
	int	c;
	sptr->length = 0;
	while(1) {
		if (!( c = xml_getch( sptr )))
			break;
		/* Alphabetical Characters */
		/* ----------------------- */
		else if (( c >= 'a') && ( c <= 'z' )) {
			macro_xml_token_character(c);
			continue;
			}
		else if (( c >= 'A') && ( c <= 'Z' )) {
			macro_xml_token_character(c);
			continue;
			}
		/* International Characters */
		/* ------------------------ */
		else if (( c >= 128) && ( c <= 165 )) {
			macro_xml_token_character(c);
			continue;
			}
		/* Numerical Figures */
		/* ----------------- */
		else if (( c >= '0') && ( c <= '9' )) {
			macro_xml_token_character(c);
			continue;
			}
		else if ( quoting ) {
			if ( c != quoting ) {
#ifdef EVITE_QUOTE_IMPAIR
				if (c=='\n') {
					xml_ungetch(sptr,c);
					break;
					};
#endif
				macro_xml_token_character(c);
				continue;
				}
			else	{
				macro_xml_token_character(c);
				break;
				}
			}
		else if ( c == '"' ) {
			if (!( sptr->length ))
			   quoting = c;
			macro_xml_token_character(c);
			continue;
			}
		else if ( c == 0x0027 ) {
			if (!( sptr->length ))
			    quoting = c;
			macro_xml_token_character(c);
			continue;
			}
		else if (( c == '_' ) || ( c == '-' ) || ( c == ':' ) || ( c == '.' )) {
			macro_xml_token_character(c);
			continue;
			}
		else	{
			xml_ungetch(sptr,c);
			break;
			}
		}
	sptr->token[sptr->length] = 0;
	return( sptr->length );
}

static	int	xml_use_token( struct xml_parser * sptr )
{
	switch ( sptr->state ) {

		case	_TAG_OPEN	:
			(sptr->user->token)(sptr->work,sptr->token);
			break;

		case	_TAG_IDLE	:
			break;

		case	_TAG_NAME	:
			sptr->state = _TAG_ATBNAME;
			return( (sptr->user->open_tag)(sptr->work, sptr->token ) );

		case	_TAG_ATBNAME 	:
			sptr->state = _TAG_ATBEQUAL;
			return( (sptr->user->atb_name)(sptr->work, sptr->token ) );

		case	_TAG_ATBEQUAL	:
			return( _XML_UNEXPECTED_TOKEN );

		case	_TAG_CLOSE	:
			sptr->state = _TAG_FINAL;
			break;

		case	_TAG_ATBVALUE	:
			sptr->state = _TAG_ATBNAME;
			return( (sptr->user->atb_value)(sptr->work, sptr->token ) );
			break;

		case	_TAG_QUESTION	:
			break;

		case	_TAG_ENDQUESTION :
			sptr->state = _TAG_QUESTION;
			break;

		case	_TAG_EXCLAIM 	:
			break;

		}
	return( _XML_SUCCESS );
}

/*	--------------------------------------------------	*/
/*		x m l _ c o n s u m e _ c o m m e n t		*/
/*	--------------------------------------------------	*/
/* 	consumes comments of the form :				*/
/*		<!-- text --/> or <-- text --/>			*/
/*	--------------------------------------------------	*/
static	int	xml_consume_comment(struct xml_parser * sptr )
{
	int	state=0;
	int	c;
	while (1)
	{
		if ((c = xml_getch( sptr )) < 0)
			return( _XML_INCORRECT_PUNCTUATION );
		else
		{
			switch ( state )
			{
			case	0 :
			case	1 : 
				if ( c == '-' )
					state++;
				else	state=0;
				continue;
			case	2 :
				if ( c == '/' )
					state++;
				else	state=0;
				continue;
			case	3 :
				if ( c == '>')
					return(_XML_SUCCESS);
				else	state=0;
				continue;
			}
		}
	}
 }

/*	--------------------------------------------------	*/
/* 		x m l _ u s e _ p u n c t u a t i o n		*/
/*	--------------------------------------------------	*/
static	int	xml_use_punctuation( struct xml_parser * sptr )
{
	switch ( sptr->state ) {

		case	_TAG_FINAL	:
			if ( sptr->punctuation == '>' ) {
				if ( sptr->nesting )
					sptr->nesting--;
				else	return( _XML_INCORRECT_PUNCTUATION );
				if ( sptr->nesting )
					sptr->state = _TAG_OPEN;
				else	sptr->state = _TAG_IDLE;
				return( (sptr->user->close_tag)(sptr->work) );
				}
			else	return( _XML_INCORRECT_PUNCTUATION );

		case	_TAG_OPEN	:
			if ( sptr->punctuation == '<' )
				sptr->state = _TAG_NAME;
			else	(sptr->user->punctuation)(sptr->work,sptr->punctuation);
			break;

		case	_TAG_IDLE	:
			if ( sptr->punctuation == '<' )
				sptr->state = _TAG_NAME;
			break;

		case	_TAG_NAME	:
			if ( sptr->punctuation == '-' ) 
			{
				if ( sptr->nesting )
					sptr->state = _TAG_OPEN;
				else	sptr->state = _TAG_IDLE;
				return( xml_consume_comment( sptr ));
			}
			else if ( sptr->punctuation == '/' ) 
			{
				sptr->state = _TAG_CLOSE;
				break;
			}
			else if ( sptr->punctuation == '?' ) 
			{
				sptr->state = _TAG_QUESTION;
				break;
			}
			else if ( sptr->punctuation == '!' ) 
			{
				sptr->state = _TAG_EXCLAIM;
				break;
			}
			else	return( _XML_INCORRECT_PUNCTUATION );

		case	_TAG_QUESTION	:
			if ( sptr->punctuation == '?' )
				sptr->state = _TAG_ENDQUESTION;
			break;

		case	_TAG_ENDQUESTION :
			if ( sptr->punctuation == '>' ) 
			{
				if ( sptr->nesting )
					sptr->state = _TAG_OPEN;
				else	sptr->state = _TAG_IDLE;
			}
			else	sptr->state = _TAG_QUESTION;
			break;

		case	_TAG_EXCLAIM 	:
			if ( sptr->punctuation == '-' )
			{
				if ( sptr->nesting )
					sptr->state = _TAG_OPEN;
				else	sptr->state = _TAG_IDLE;
				return( xml_consume_comment( sptr ));
			}
			else if ( sptr->punctuation == '>' ) 
			{
				if ( sptr->nesting )
					sptr->state = _TAG_OPEN;
				else	sptr->state = _TAG_IDLE;
			}
			break;

		case	_TAG_ATBNAME 	:
			if ( sptr->punctuation == '>' ) {
				sptr->nesting++;
				sptr->state = _TAG_OPEN;
				break;
				}
			else if ( sptr->punctuation == '/' ) {
				sptr->nesting++;
				sptr->state = _TAG_FINAL;
				break;
				}
			else return( _XML_INCORRECT_PUNCTUATION );

		case	_TAG_ATBEQUAL	:
			if ( sptr->punctuation == '=' ) {
				sptr->state = _TAG_ATBVALUE;
				break;
				}

		case	_TAG_ATBVALUE	:
		case	_TAG_CLOSE	:
			return( _XML_INCORRECT_PUNCTUATION );

		}

	return( _XML_SUCCESS );
}

void	xml_use_relays( struct xml_parser * sptr, struct xml_relay * fptr )
{
	/* install user relay table if provided */
	/* ------------------------------------ */
	if ( fptr ) { 
		if ( fptr->open_tag    ) {  sptr->user->open_tag 	= fptr->open_tag;	}
		if ( fptr->atb_name    ) {  sptr->user->atb_name 	= fptr->atb_name;	}
		if ( fptr->atb_value   ) {  sptr->user->atb_value 	= fptr->atb_value;	}
		if ( fptr->white_space ) {  sptr->user->white_space	= fptr->white_space;	}
		if ( fptr->token       ) {  sptr->user->token		= fptr->token;		}
		if ( fptr->punctuation ) {  sptr->user->punctuation	= fptr->punctuation;	}
		if ( fptr->close_tag   ) {  sptr->user->close_tag	= fptr->close_tag;	}
		}
	return;
}

int	xml_parse_tokens(struct xml_parser * sptr )
{
	int	c;
	int	whoops;
	int	status=_XML_SUCCESS;
	while ((c = xml_remove_space( sptr )) != 0) {
		if ((whoops = xml_get_token( sptr )) > 0) {
			if ((status = xml_use_token(sptr)) != _XML_SUCCESS)
				break;
			}
		else if ( whoops == -1 ) {
			status=_XML_INCORRECT_TOKEN;
			break;
			}
		else if (!( sptr->punctuation = xml_getch(sptr) ))
			break;
		else if ((status = xml_use_punctuation( sptr )) != _XML_SUCCESS)
			break;
		}
	return(status);
}

int	xml_parse( char * nptr, struct xml_relay * fptr, void * aptr )
{
	int	status=0;
	int	c;
	int	lno=0;
	struct	xml_parser	*	sptr;

	/* attempt to open the source context */
	/* ---------------------------------- */
	if (!( sptr = xml_open( nptr, aptr )))
		return(_XML_FILE_ERROR);
	else	{
		/* install user relay table if provided */
		/* ------------------------------------ */
		xml_use_relays( sptr, fptr );

		/* connect application to parser */
		/* ----------------------------- */
		sptr->work->parent = sptr;

		/* Parse until end of source */
		/* ------------------------- */
		status = xml_parse_tokens(sptr);

		/* detect incomplete termination */
		/* ----------------------------- */
		if ( status ) {
			if ( status != 48 ) {
				lno = sptr->lines;
				sptr = xml_close( sptr );
				if (!( fptr->failure ))
					return( status );
				else	return((fptr->failure)(nptr,status,lno));
				}
			else	return( _XML_SUCCESS );
			}
		else if (!( sptr->nesting )) {
			sptr = xml_close( sptr );
			return( _XML_SUCCESS );
			}
		else	{
			sptr = xml_close( sptr );
			return( _XML_UNEXPECTED_EOF );
			}
		}
}

/* Attribute handling. */
static size_t translate_string_with_string_impl(char const * startpos, char const *input, char *buffer, int buffersize, char const *replace, char const *with)
{
	char const *pos;
	char const *previous = input;
	size_t copy_length;
	size_t total_length = 0;
	size_t const with_length = with == NULL ? 0 : strlen(with);
	size_t const replace_length = replace == NULL ? 0 : strlen(replace);
	size_t const diff_length = with_length > replace_length ? with_length - replace_length : replace_length - with_length;
	size_t const input_over_length = input == NULL ? 0 : strnlen(input, buffersize+1);
	size_t const input_length = input_over_length > buffersize ? buffersize : input_over_length;
	size_t overflow_required = 0;

	if (buffer == NULL || input == NULL)
	{
		return 0;
	}

	if (replace == NULL)
	{
		if (with == NULL)
		{
			return 0;
		}
		else
		{
			replace = "";
		}
	}
	else if (with == NULL)
	{
		with = "";
	}

	pos = startpos;
//	pos = strstr(input, replace);

	if (pos == NULL) // If replace isn't in input
	{
		strncpy(buffer, input, input_length);
		total_length = input_length;

		if (input_over_length > input_length) // If more in input string
		{
			overflow_required += 1;

		} // Ends if more in input string
	} // Ends if replace isn't in input
	else // Else replace in input
	{
		while (pos != NULL)
		{
			if (pos != previous) // If intervening chunk
			{
				copy_length = pos-previous;

				if (copy_length <= (buffersize - total_length)) // If room for next chunk
				{
					strncpy(buffer+total_length, previous, copy_length);
					previous += copy_length;
					total_length += copy_length;

				} // Ends if room for next chunk
				else // Else no room for next chunk
				{
					strncpy(buffer+total_length, previous, (buffersize - total_length));
					overflow_required += copy_length - (buffersize - total_length);
					total_length = buffersize;
					break;

				} // Ends else not enough room for next chunk
			} // Ends if intervening chunk

			if (with_length < (buffersize - total_length)) // If room for replacement string
			{
				strncpy(buffer+total_length, with, with_length);
				total_length += with_length;

				if (1 <= (buffersize - total_length)) // If room for another character
				{
					if (pos == previous && *replace == '\0' && *pos != '\0') // If replacing empty string and not off end of input
					{
						*(buffer + total_length) = *pos;
						++pos;
						++total_length;

					} // Ends if replacing empty string and not off end of input
				} // Ends if room for another character
				else // Else not enough room for another character
				{
					++overflow_required;

				} // Ends else not enough room for another character

				pos += replace_length;
				previous = pos;

				if (pos-input >= buffersize)
				{
					overflow_required += (pos-input)-buffersize;
					strncpy(buffer + total_length, pos, (pos - input) - buffersize);
					total_length = buffersize;
					break;
				}
				else if (*pos == '\0')
				{
					break;
				}

				// Without "strnstr()" there's no way to avoid run off end of input buffer here.
				pos = strstr(pos, replace);
				if (pos > (input+input_length)) // If pos beyond acceptable input length
				{
					// Everything from where we are to end of acceptable input buffer.
					copy_length = input_length - (previous-input);
					strncpy(buffer+total_length, previous, copy_length);
					*(buffer + total_length + copy_length) = '\0';
					// Now what ? Assume input is valid and use pos to ask for bigger buffer (optimistic).
					total_length = buffersize;
					overflow_required += ((pos-previous) - copy_length);
					//previous = (input + input_length - 1);
					break;

				} // Ends if pos beyond acceptable input length

			} // Ends if room for replacement string
			else // Else no room for replacement string
			{
				strncpy(buffer+total_length, previous, (buffersize - total_length));
				overflow_required += with_length - (buffersize - total_length);
				total_length = buffersize;
				break;

			} // Ends else no room for replacement string
		} // Ends loop over chunks of input string

		if (total_length < buffersize) // If still space in buffer
		{
			copy_length = input_length - (previous-input);
			if (copy_length > 0) // If input string has final chunk
			{
				if (total_length + copy_length < buffersize) // If room in buffer for final chunk
				{
					strncpy(buffer+total_length, previous, copy_length);
					total_length += copy_length;

					if (input_over_length > input_length) // If there's more input than what we access
					{
						// Need to try again with an output buffer bigger than
						// input buffer.
						// Note that if input is shrinking this isn't really a
						// requirement, but hard to track how much more input we
						// could squeeze in having removed characters.
						total_length = buffersize;
						overflow_required += (input_over_length - input_length);

					} // Ends if there's more input than what we access

				} // Ends if room in buffer for final chunk
				else // Else insufficient room in buffer for final chunk
				{
					strncpy(buffer+total_length, previous, buffersize - total_length);
					overflow_required += copy_length - (buffersize - total_length);
					total_length = buffersize;

				} // Ends else insufficient room in buffer for final chunk

			} // Ends if input string has final chunk
			else if (*replace == '\0' && *input != '\0') // Else if replacing empty string and input wasn't empty string
			{
				// Append with.
				strncpy(buffer+total_length, with, with_length);
				total_length += with_length;

			} // Ends else if replacing empty string and input wasn't empty string
		} // Ends if still space in buffer
		else if (input_over_length > buffersize) // Else if input longer than buffer
		{
			if (overflow_required == 0) // If no overflow registered yet
			{
				overflow_required += input_over_length - buffersize;

			} // Ends if no overflow registered yet

		} // Ends else if input longer than buffer
	} // Ends else replace in input

	buffer[buffersize-1] = '\0';

	if (total_length < buffersize) // If didn't fill buffer
	{
		buffer[total_length] = '\0';

	} // Ends if didn't fill buffer

	if (total_length == buffersize && overflow_required == 0) // If string fits exactly in buffer
	{
		// That means the buffer was 1 character too short.
		++overflow_required;

	} // Ends if string fits exactly in buffer

	if (overflow_required != 0)
	{
		// Add an extra for null terminator.
		++overflow_required;
	}

	return total_length + overflow_required;
}

size_t translate_string_with_string(char const *input, char *buffer, int buffersize, char const *replace, char const *with)
{
	char const *startpos;

	if (buffer == NULL || input == NULL)
	{
		return 0;
	}

	startpos = strstr(input, replace);
	return translate_string_with_string_impl(startpos, input, buffer, buffersize, replace, with);
}

size_t translate_string_with_string_startpos(char const * startpos, char const *input, char *buffer, int buffersize, char const *replace, char const *with)
{
	return translate_string_with_string_impl(startpos, input, buffer, buffersize, replace, with);
}

void fprintf_xml_string_attribute(FILE *h, char const *format, char const *attribute)
{
	static size_t const buffersize = 1024;
	char buffer[buffersize];

	size_t convert_size;
	size_t old_convert_size;
	char *pbuffer = buffer;
	char const *startpos;

	buffer[0] = '\0';

	startpos = strstr(attribute, "\"");

	if (startpos == NULL)
	{
		fprintf(h, format, attribute);
	}
	else
	{
		convert_size = translate_string_with_string_startpos(startpos, attribute, pbuffer, buffersize, "\"", "&quot;");
		old_convert_size = buffersize;

		while (convert_size > old_convert_size)
		{
			if (pbuffer == buffer)
			{
				pbuffer = (char *)malloc(convert_size);
			}
			else
			{
				old_convert_size = convert_size;
				pbuffer = (char *)realloc(pbuffer, convert_size);
			}
			convert_size = translate_string_with_string_startpos(startpos, attribute, pbuffer, buffersize, "\"", "&quot;");
		}

		fprintf(h, format, pbuffer);

		if (pbuffer != buffer)
		{
			free(pbuffer);
		}
	}

}

char  * unserialize_xml_string_attribute_value(char *value)
{
	size_t convert_size;
	size_t old_convert_size;
	char *pbuffer;
	char const *startpos;
	char *result;
	size_t value_length;

	startpos = strstr(value, "&quot;");

	if (startpos == NULL)
	{
		result = value;
	}
	else
	{
		value_length = strlen(value);
		pbuffer = (char *)allocate(value_length);

		convert_size = translate_string_with_string_startpos(startpos, value, pbuffer, value_length, "&quot;", "\"");
		old_convert_size = value_length;

		while (convert_size > old_convert_size)
		{
			liberate(pbuffer);
			pbuffer = (char *)allocate(convert_size);
			old_convert_size = convert_size;
			convert_size = translate_string_with_string_startpos(startpos, value, pbuffer, value_length, "&quot;", "\"");
		}

		liberate(value);
		result = pbuffer;
	}

	return result;
}


#endif	/* _xml_parse_c */
	/* ------------ */

