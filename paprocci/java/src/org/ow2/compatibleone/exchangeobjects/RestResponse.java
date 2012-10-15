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

	private RestHeaderBlock all;
    private int status;
    private String version;
    private String message;
    private int type;
    private String body;
    
    public RestResponse(
    		RestHeaderBlock all,
    		int status, 
    		String version, 
    		String message, 
    		int type, 
    		String body){
    	
    		this.all = all;
    		this.status = status;
    		this.version = version;
    		this.message = message;
    		this.type = type;
    		this.body = body;
    }
    public RestHeaderBlock getAll() {
		return all;
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
    public String toString(){
    	return 
    		"[" + 
    		"block " +  all + ", \n" + 
    		"status " +  status + ", \n" + 
    		"version " +  version + ", \n" + 
    		"message " +  message + ", \n" + 
    		"type " +  type + ", \n" + 
    		"body " +  body + ", \n" + 
    		"]"  ;
    }
}
