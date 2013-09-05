
#include "rest.h"
#include "restpublic.h"

struct rest_response *bad_request_html_response(struct rest_response *aptr) {
	return rest_html_response( aptr, 400, "Bad Request");
}

struct rest_response *not_found_html_response(struct rest_response *aptr) {
	return rest_html_response( aptr, 404, "Not Found");
}

