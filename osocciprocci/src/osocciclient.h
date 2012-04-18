#ifndef	_occi_os_procci_h
#define	_occi_os_procci_h

#define	_OCCI_OS_SECURITY	"http://www.compatibleone.fr/schemes/security"
#define	_OCCI_SECURITY_GROUP	"http://schemas.ogf.org/occi/infrastructure/security#group"
#define	_OCCI_SECURITY_RULE	"http://schemas.openstack.org/occi/infrastructure/network/security#"

public struct	rest_response * occi_os_capacities();
public struct	rest_response * create_occi_os_compute(char * machine, char * system);
public struct	rest_response * stop_occi_os_compute(char * vm);
public struct	rest_response * start_occi_os_compute(char * vm);
public struct	rest_response * delete_occi_os_network(char * nw);
public struct	rest_response * get_occi_os_compute(char * id);
public struct	rest_response * attach_occi_os_network(char * vm, char * nw);
public struct	rest_response * create_occi_os_network(char * label);
public struct	rest_response * delete_occi_os_compute(char * vm);
public struct	rest_response * create_occi_os_security_group(char * g);
public struct	rest_response * create_occi_os_security_rule(char * g, char * protocol,int from, int to, char * iprange );
public struct	rest_response * list_occi_os_security_rules(char * g);
public struct	rest_response * get_occi_os_security_rule( char * id );
public struct	rest_response * delete_occi_os_security_rule(char * id );
public struct	rest_response * delete_occi_os_security_group(char * g);
public struct	rest_response * create_occi_os_secure_compute(char * machine, char * system, char * g);
public struct	rest_response * allocate_occi_os_floating_ip(char * id);
public struct	rest_response * release_occi_os_floating_ip(char * id);
public	int	check_keystone_authorization();

public int	os_occi_initialise_client(
			char * user,
			char * password,
			char * host,
			char * domain,
			char * agent,
			char * tls);

#endif 	/* _occi_client_h */
	/* -------------- */














