#ifndef _computenext_h
#define _computenext_h

struct  computenext
{
        char *  id;
        char *  name;
        char *  description;
        char *  hostname;
		char *  placement;
		
        int     state;

        char *  workload;
        char *  workloadelement;
        char *  transaction;
        char *  image;
        char *  instancetype;
        char *  virtualmachine;
        char *  volumestorage;

        char *  securitygroup;
        char *  port;
        char *  keypair;
        char *  password;
        char *  username;

        char *  ipaddress;
        char *  status;

        char *  provider;
        char *  region;
        char *  zone;
        char *  price;
        char *  location;

        char *  apikey;
        char *  apisec;
};

#endif

