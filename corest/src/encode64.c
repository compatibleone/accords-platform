#ifndef	_encode64_c
#define	_encode64_c

#include "encode64.h"

int	EncodeBase64( char FAR * rptr, char FAR * sptr, int i )
{
	int	j=0;
	int	m=0;
	int	l=0;
	union _triochar
	{
		char Chars[3];
		struct _b6
		{
			unsigned long group3            : 6; // low order
			unsigned long group2            : 6;
			unsigned long group1            : 6;
			unsigned long group0            : 6; // high order
		} b6;
	} triochar;     

	while (j < i)	{		// processes the i chars from SMTPbuf

		if (j+3 <= i)  {	// there are 3 chars, from position j, to be processed
			triochar.Chars[2] = *( sptr + j);
			triochar.Chars[1] = *( sptr + j+1);
			triochar.Chars[0] = *( sptr + j+2);
			*( rptr + (m++)) = Base64[triochar.b6.group0];
			*( rptr + (m++)) = Base64[triochar.b6.group1];
			*( rptr + (m++)) = Base64[triochar.b6.group2];
			*( rptr + (m++)) = Base64[triochar.b6.group3];
			j = j+3; l = l+3;
			}

		else if (j+2 == i) {	// the last two chars are j, j+1
			triochar.Chars[2] = *( sptr + j);
			triochar.Chars[1] = *( sptr + j+1);
			triochar.Chars[0] = '\0';
			*( rptr + (m++)) = Base64[triochar.b6.group0];
			*( rptr + (m++)) = Base64[triochar.b6.group1];
			*( rptr + (m++)) = Base64[triochar.b6.group2];
			*( rptr + (m++)) = '=';
			j = j+2; l = l+2;
			}

		else 	{		// the last char: j
			triochar.Chars[2] = *( sptr + j);
			triochar.Chars[1] = '\0';
			triochar.Chars[0] = '\0';
			*( rptr + (m++)) = Base64[triochar.b6.group0];
			*( rptr + (m++)) = Base64[triochar.b6.group1];
			*( rptr + (m++)) = '=';
			*( rptr + (m++)) = '=';
			j++; l++;
			}

		}
	*(rptr +m) = 0;
	return(m);
}



#endif	//	_encode64_c

