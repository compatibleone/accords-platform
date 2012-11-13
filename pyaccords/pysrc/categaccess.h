#ifndef _CATEGACCESS_H
#define _CATEGACCESS_H
#include "../../occi/src/occi.h"

struct{
	const char * name;
	int access;
}occiCategoryAccess_map[]={
	{"amazonEc2", _OCCI_PROVIDER},
        {"intercloudGW", _OCCI_PROVIDER},
};

#endif
