#ifndef	_base64_h
#define	_base64_h
#ifndef	BB64C
#define	BB64C 100
#endif
#ifndef pMIME_ERR_BAD_BASE64
#define pMIME_ERR_BAD_BASE64		510	// bad Base 64
#endif
#ifndef	OK
#define	OK 0
#endif

int DecodeBase64(char * sInput, char * sOutput, int * piBytes);

#endif	// _base64_h

