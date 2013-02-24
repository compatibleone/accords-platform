
#ifndef	_cords_contract_h
#define	_cords_contract_h

struct	cords_contract
{
	char *	id;
	char *	name;
	char *	node;
	char *	provider;
	char *	profile;
	char *	reference;
	char *	hostname;
	char *	rootpass;
	char *	workload;
	char *	blob;
	char *	price;
	char *	access;
	char *	common;
	char *	session;
	char *	scope;
	char *	type;
	char *	category;
	char *	service;
	char *	firewall;
	char *	placement;
	char *	parentservice;
	char *	agreement;
	int	flags;
	int	startduration;
	int	stopduration;
	int	saveduration;
	int	instructions;
	int	commons;
	int	when;
	int	state;
};

#endif	

