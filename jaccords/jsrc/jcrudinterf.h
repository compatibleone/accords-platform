#ifndef _JCATEGINTERF_H_
#define _JCATEGINTERF_H_
#include "jcategcrud.h"
struct{
	const char* name;
	struct occi_interface *(*interface_Func)();
}occiCategoryInterface_map[]={
};

#endif
