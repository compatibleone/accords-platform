#ifndef _CATEGACTION_H_
#define _CATEGACTION_H_
#include "listaction.h"

struct{
    char *name;
    void *func;    
//struct rest_response *(*func)(struct occi_category *optr,struct rest_client *cptr,struct rest_request *rptr,struct rest_response *aptr,void *vptr);
}occiCategoryAction_map[]={
	{"ec2amazon_action0", ec2amazon_action0 },
	{"ec2amazon_action1", ec2amazon_action1 },
	{"ec2amazon_action2", ec2amazon_action2 },
};
#endif
