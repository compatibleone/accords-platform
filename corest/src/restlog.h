#ifndef	_rest_log_h
#define	_rest_log_h


public	int	rest_log_send_request ( char * agent, char * method, struct url * uptr );
public	int	rest_log_recv_response( char * agent, struct rest_response * aptr );

public	int	rest_log_send_response( char * agent, struct rest_response * aptr );
public	int	rest_log_recv_request ( char * agent, struct rest_request  * rptr );

	/* ---------- */
#endif	/* _restlog_h */
	/* ---------- */

