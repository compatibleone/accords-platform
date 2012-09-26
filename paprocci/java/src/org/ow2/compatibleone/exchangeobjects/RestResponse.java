package org.ow2.compatibleone.exchangeobjects;

public class RestResponse {
/*
struct  rest_response {
        struct  rest_header     * first;
        struct  rest_header     * last;
        int                     status;
        char                    * version;
        char                    * message;
        int                     type;
        char                    * body;
};	 
 */

	private RestHeader first;
	private RestHeader last;
    private int status;
    private String version;
    private String message;
    private int type;
    private String body;
    
    public RestResponse(
    		RestHeader first, 
    		RestHeader last, 
    		int status, 
    		String version, 
    		String message, 
    		int type, 
    		String body){
    	
    		this.first = first;
    		this.last = last;
    		this.status = status;
    		this.version = version;
    		this.message = message;
    		this.type = type;
    		this.body = body;
    }
    public RestHeader getFirst() {
		return first;
	}
    public RestHeader getLast() {
		return last;
	}
    public int getStatus() {
		return status;
	}
    public String getVersion() {
		return version;
	}
    public String getMessage() {
		return message;
	}
    public int getType() {
		return type;
	}
    public String getBody() {
		return body;
	}
}
