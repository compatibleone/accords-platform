#ifndef _CATEGACTION_H_
#define _CATEGACTION_H_
#include "listaction.h"

struct{
    char *name;
    void *func;    
}occiCategoryAction_map[]={
	{"amazonEc2_action0", amazonEc2_action0 },
	{"amazonEc2_action1", amazonEc2_action1 },
	{"amazonEc2_action2", amazonEc2_action2 },
	{"amazonEc2_action3", amazonEc2_action3 },
	{"amazonEc2_action4", amazonEc2_action4 },
	{"amazonEc2_action5", amazonEc2_action5 },
	{"ec2config_action0", ec2config_action0 },
};
#endif
