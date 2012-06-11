/********************************************************************************************************/
/* Hamid MEDAJHED & Elyes ZEKRI (c) Prologue                                                            */
/********************************************************************************************************/
#ifndef _CATEGINTERF_H_
#define _CATEGINTERF_H_
#include "categcrud.h"

struct occi_interface *amconfig_interface_Func(){
	struct occi_interface amconfig_interface={amconfig_create,amconfig_retrieve,amconfig_update,amconfig_delete};
	struct occi_interface *optr = (struct occi_interface *) malloc (sizeof(struct occi_interface));//begin amconfig
	optr->create = amconfig_interface.create;
	optr->retrieve = amconfig_interface.retrieve;
	optr->update = amconfig_interface.update;
	optr->delete = amconfig_interface.delete;
	return optr;}//end amconfig
struct occi_interface *ec2amazon_interface_Func(){
	struct occi_interface ec2amazon_interface={ec2amazon_create,ec2amazon_retrieve,ec2amazon_update,ec2amazon_delete};
	struct occi_interface *optr = (struct occi_interface *) malloc (sizeof(struct occi_interface));//begin ec2amazon
	optr->create = ec2amazon_interface.create;
	optr->retrieve = ec2amazon_interface.retrieve;
	optr->update = ec2amazon_interface.update;
	optr->delete = ec2amazon_interface.delete;
	return optr;}//end ec2amazon

struct{
	const char* name;
	struct occi_interface *(*interface_Func)();
}occiCategoryInterface_map[]={
	{ "amconfig" , amconfig_interface_Func },
	{ "ec2amazon" , ec2amazon_interface_Func },
};
#endif
