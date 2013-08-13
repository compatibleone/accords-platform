#ifndef	_jlistoccibuilder_h
#define	_jlistoccibuilder_h

#include "jlistcateg.h"
#include "cordsocci.c"

public struct occi_category * occi_cords_script_builder(char *a,char * b);

const static struct {
	const char *name;
	public struct occi_category * (*func)(char *a,char * b);
} occiCategoryBuilder_map[]={
	{ "script", occi_cords_script_builder },
};

#endif
