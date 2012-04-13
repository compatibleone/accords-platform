#ifndef	_occi_os_procci_h
#define	_occi_os_procci_h

public struct	rest_response * occi_os_capacities();
public struct	rest_response * create_occi_os_compute(char * machine, char * system);
public struct	rest_response * stop_occi_os_compute(char * vm);
public struct	rest_response * start_occi_os_compute(char * vm);
public struct	rest_response * delete_occi_os_network(char * nw);
public struct	rest_response * attach_occi_os_network(char * vm,char * nw);
public struct	rest_response * create_occi_os_network(char * label);
public struct	rest_response * delete_occi_os_compute(char * vm);
public int	os_occi_initialise_client(char * user,char * password,char * host,char * domain,char * agent,char * tls);

#endif 	/* _occi_client_h */
		/* -------------- */

