/********************************************************************************************************/
/* Hamid MEDAJHED & Elyes ZEKRI (c) Prologue                                                            */
/********************************************************************************************************/
#ifndef _CATEGINTERF_H_
#define _CATEGINTERF_H_
#include "categcrud.h"

struct occi_interface *amazonEc2_interface_Func(){
	struct occi_interface amazonEc2_interface={amazonEc2_create,amazonEc2_retrieve,amazonEc2_update,amazonEc2_delete};
	struct occi_interface *optr = (struct occi_interface *) malloc (sizeof(struct occi_interface));//begin amazonEc2
	optr->create = amazonEc2_interface.create;
	optr->retrieve = amazonEc2_interface.retrieve;
	optr->update = amazonEc2_interface.update;
	optr->delete = amazonEc2_interface.delete;
	return optr;}//end amazonEc2
struct occi_interface *ec2config_interface_Func(){
	struct occi_interface ec2config_interface={ec2config_create,ec2config_retrieve,ec2config_update,ec2config_delete};
	struct occi_interface *optr = (struct occi_interface *) malloc (sizeof(struct occi_interface));//begin ec2config
	optr->create = ec2config_interface.create;
	optr->retrieve = ec2config_interface.retrieve;
	optr->update = ec2config_interface.update;
	optr->delete = ec2config_interface.delete;
	return optr;}//end ec2config

struct{
	const char* name;
	struct occi_interface *(*interface_Func)();
}occiCategoryInterface_map[]={
	{ "amazonEc2" , amazonEc2_interface_Func },
	{ "ec2config" , ec2config_interface_Func },
};
#endif
