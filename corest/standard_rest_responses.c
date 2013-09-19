#include <assert.h>

#include "rest.h"
#include "restpublic.h"

struct rest_response *bad_request_html_response(struct rest_response *aptr) {
	return rest_html_response( aptr, 400, "Bad Request");
}

struct rest_response *not_found_html_response(struct rest_response *aptr) {
	return rest_html_response( aptr, 404, "Not Found");
}

struct rest_response *internal_failure_response(struct rest_response * aptr) {
    printf("*************** Server failure *******************\n");
    assert(0); // Useful to know if we're failing
    return rest_html_response( aptr, 500, "Server Failure" );
}

struct rest_response *rest_internal_failure_response(struct rest_response * aptr) {
    printf("*************** Server failure *******************\n");
    assert(0); // Useful to know if we're failing
    return rest_response_status( aptr, 500, "Server Failure" );
}

