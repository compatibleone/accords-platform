#ifndef	_rest_response
#define	_rest_response

struct	rest_response
{
	struct	rest_header	* first;
	struct	rest_header	* last;
	struct	rest_request	* request;
	int			status;
	char			* version;
	char			* message;
	int			type;
	char			* body;
};

struct rest_response *bad_request_html_response(struct rest_response *aptr);
struct rest_response *not_found_html_response(struct rest_response *aptr);

#endif






