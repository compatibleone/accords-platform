#ifndef	_occi_listbuilder_h
#define	_occi_listbuilder_h

#include "listcateg.h"
#include "cordsocci.c"

public struct occi_category * occi_amazonEc2_builder(char * a,char *b);
public struct occi_category * occi_ec2config_builder(char * a,char *b);
public struct occi_category * occi_gw_builder(char * a,char *b);
public struct occi_category * occi_linkgw_builder(char * a,char *b);
public struct occi_category * occi_intercloudGW_builder(char * a,char *b);
const static struct {
	const char *name;
	public struct occi_category * (*func)(char *a,char * b);
} occiCategoryBuilder_map[]={
	{ "amazonEc2", occi_amazonEc2_builder },
	{ "ec2config", occi_ec2config_builder },
	{ "gw", occi_gw_builder },
	{ "linkgw", occi_linkgw_builder },
	{ "intercloudGW", occi_intercloudGW_builder },
};

#endif
