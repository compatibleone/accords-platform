#ifndef _linkgw_h_
#define _linkgw_h_

struct linkgw
{
	char * id;
	char * name;
	char * intercloudGW;
	char * account;
	char * gwsrc;
	char * gwdst;
	char * tunnelproto;
	char * addressgresrc;
	char * addressgredst;
	char * prefix;
	char * authenticationkey;
	char * endpointsrc;
	char * endpointdst;
	char * state;
};

#endif