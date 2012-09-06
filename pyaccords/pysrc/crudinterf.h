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
struct occi_interface *gw_interface_Func(){
	struct occi_interface gw_interface={gw_create,gw_retrieve,gw_update,gw_delete};
	struct occi_interface *optr = (struct occi_interface *) malloc (sizeof(struct occi_interface));//begin gw
	optr->create = gw_interface.create;
	optr->retrieve = gw_interface.retrieve;
	optr->update = gw_interface.update;
	optr->delete = gw_interface.delete;
	return optr;}//end gw
struct occi_interface *linkgw_interface_Func(){
	struct occi_interface linkgw_interface={linkgw_create,linkgw_retrieve,linkgw_update,linkgw_delete};
	struct occi_interface *optr = (struct occi_interface *) malloc (sizeof(struct occi_interface));//begin linkgw
	optr->create = linkgw_interface.create;
	optr->retrieve = linkgw_interface.retrieve;
	optr->update = linkgw_interface.update;
	optr->delete = linkgw_interface.delete;
	return optr;}//end linkgw
struct occi_interface *intercloudGW_interface_Func(){
	struct occi_interface intercloudGW_interface={intercloudGW_create,intercloudGW_retrieve,intercloudGW_update,intercloudGW_delete};
	struct occi_interface *optr = (struct occi_interface *) malloc (sizeof(struct occi_interface));//begin intercloudGW
	optr->create = intercloudGW_interface.create;
	optr->retrieve = intercloudGW_interface.retrieve;
	optr->update = intercloudGW_interface.update;
	optr->delete = intercloudGW_interface.delete;
	return optr;}//end intercloudGW

struct{
	const char* name;
	struct occi_interface *(*interface_Func)();
}occiCategoryInterface_map[]={
	{ "amazonEc2" , amazonEc2_interface_Func },
	{ "ec2config" , ec2config_interface_Func },
	{ "gw" , gw_interface_Func },
	{ "linkgw" , linkgw_interface_Func },
	{ "intercloudGW" , intercloudGW_interface_Func },
};
#endif
