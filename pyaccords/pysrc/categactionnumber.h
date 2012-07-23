#ifndef _CATEGACTIONACTIONNUMBER_H_
#define _CATEGACTIONACTIONNUMBER_H_
#include "listaction.h"

struct{
    char *name;
    int (*func)();    
}occiCategoryActionNumber_map[]={
	{"amazonEc2", amazonEc2_getnumber },
	{"ec2config", ec2config_getnumber },
};
#endif
