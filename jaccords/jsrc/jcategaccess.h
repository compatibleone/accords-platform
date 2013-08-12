#ifndef _JCATEGACCESS_H
#define _JCATEGACCESS_H
#include "../../occi/occi.h"

struct{
	const char * name;
	int access;
}occiCategoryAccess_map[]={
        {"amazonEc2", _OCCI_PROVIDER},
        {"intercloudGW", _OCCI_PROVIDER},
};

#endif
