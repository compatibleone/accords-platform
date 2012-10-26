#ifndef	_base64_c
#define	_base64_c

#include "base64.h"

#include <string.h>

int DecodeBase64(
	char * sInput, char * sOutput, int * piBytes)
{
	int iError = OK, i, j, l;

	union _triochar {
		char Chars[3];
		struct _b6 {
			unsigned long group3		: 6; // low order
			unsigned long group2		: 6;
			unsigned long	group1		: 6;
			unsigned long	group0		: 6; // high order
			} b6;
		} triochar;	

	l = strlen((char FAR *)sInput);
	l /= 4;
	l *= 4;
	*(sInput+l) = 0;
	if ((l % 4) != 0) {
		iError = pMIME_ERR_BAD_BASE64;
		}
	else	{
	  	for(	j=0,i=0; 
			i < l; 
			i += 4) {
			if (((FromBase64[sInput[i]]   == BB64C) 
			||   (FromBase64[sInput[i+1]] == BB64C) 
			||   (FromBase64[sInput[i+2]] == BB64C) 
			||   (FromBase64[sInput[i+3]] == BB64C)) 
			||  (sInput[i]   == '=') 
			||  (sInput[i+1] == '=')
			||  ((sInput[i+2] == '=') && (sInput[i+3] != '=')) 
			||  ((sInput[i+2] == '=') && (sInput[i+3] == '=') && ((i+4) != l))
			||  ((sInput[i+3] == '=') && ((i+4) != l))) {
				iError = pMIME_ERR_BAD_BASE64;
				}
			triochar.b6.group0 = FromBase64[sInput[i]];
			triochar.b6.group1 = FromBase64[sInput[i+1]];
			triochar.b6.group2 = FromBase64[sInput[i+2]];
			triochar.b6.group3 = FromBase64[sInput[i+3]];
			sOutput[j] = triochar.Chars[2];
			j++;
			if (sInput[i + 2] != '=')
				sOutput[j++] = triochar.Chars[1];
			if (sInput[i + 3] != '=')
				sOutput[j++] = triochar.Chars[0];
			}
		*piBytes = j;
		}
	return(iError);
}

#endif	// _base64_h

