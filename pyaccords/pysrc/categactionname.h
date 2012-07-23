#ifndef _CATEGACTIONNAME_H_
#define _CATEGACTIONNAME_H_
#include "listaction.h"

struct{
    char *name;
    char *(*func) (int a);    
}occiCategoryActionName_map[]={
	{"amazonEc2", amazonEc2_getname },
	{"ec2config", ec2config_getname },
};
#endif
