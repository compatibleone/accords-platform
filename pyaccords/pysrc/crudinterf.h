/********************************************************************************************************/
/* Hamid MEDAJHED & Elyes ZEKRI (c) Prologue                                                            */
/********************************************************************************************************/
#ifndef _CATEGINTERF_H_
#define _CATEGINTERF_H_
#include "categcrud.h"


struct{
	const char* name;
	struct occi_interface *(*interface_Func)();
}occiCategoryInterface_map[]={
};
#endif
