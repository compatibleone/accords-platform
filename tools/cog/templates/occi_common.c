//[[[cog pass]]] No need for this, it's just a little nicer for the build system to handle this file as a cog template
//[[[end]]]
 
#ifndef _occi_common_c
#define _occi_common_c

private struct rest_response *bad_request_html_response(struct rest_response *aptr) {
	return rest_html_response( aptr, 400, "Bad Request");
}

private struct rest_response *not_found_html_response(struct rest_response *aptr) {
	return rest_html_response( aptr, 404, "Not Found");
}

typedef int (*callback_func)(struct occi_category * optr, void * vptr,struct rest_request * rptr);

#endif
