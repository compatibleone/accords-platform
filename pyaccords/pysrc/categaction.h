#ifndef _CATEGACTION_H_
#define _CATEGACTION_H_
#include "listaction.h"

struct{
    char *name;
    void *func;    
}occiCategoryAction_map[]={
	{"amazonEc2_start", start_amazonEc2},
	{"amazonEc2_stop", stop_amazonEc2},
	{"amazonEc2_snapshot", snapshot_amazonEc2},
	{"amazonEc2_suspend", suspend_amazonEc2},
	{"amazonEc2_restart", restart_amazonEc2},
	{"ec2config_current", current_ec2config},
	{"gw_start", start_gw },
	{"gw_stop", stop_gw },
	{"linkgw_start", start_linkgw },
	{"linkgw_stop", stop_linkgw },
	{"intercloudGW_start", start_intercloudGW },
	{"intercloudGW_stop", stop_intercloudGW },
};
#endif
