#ifndef	_base64_c
#define	_base64_c

#include "base64.h"

#include <string.h>

static unsigned char FromBase64[256] = {
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //  0 -  7
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //  8 - 15
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, // 16 - 23
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, // 24 - 31
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, // 32 - 39
    BB64C, BB64C, BB64C,    62, BB64C, BB64C, BB64C,    63, // 40 - 47
       52,    53,    54,    55,    56,    57,    58,    59, // 48 - 55
       60,    61, BB64C, BB64C, BB64C,     0, BB64C, BB64C, // 56 - 63
    BB64C,     0,     1,     2,     3,     4,     5,     6, // 64 - 71
        7,     8,     9,    10,    11,    12,    13,    14, // 72 - 79
       15,    16,    17,    18,    19,    20,    21,    22, // 80 - 87
       23,    24,    25, BB64C, BB64C, BB64C, BB64C, BB64C, // 88 - 95
    BB64C,    26,    27,    28,    29,    30,    31,    32, // 96 -103
       33,    34,    35,    36,    37,    38,    39,    40, //104 -111
       41,    42,    43,    44,    45,    46,    47,    48, //112 -119
       49,    50,    51, BB64C, BB64C, BB64C, BB64C, BB64C, //120 -127
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //128 -135
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //136 -143
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //144 -151
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //152 -159
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //160 -167
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //168 -175
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //176 -183
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //184 -191
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //192 -199
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //200 -207
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //208 -215
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //216 -223
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //224 -231
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //232 -239
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, //240 -247
    BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C, BB64C  //248 -255
    };

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

	l = strlen((char *)sInput);
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

