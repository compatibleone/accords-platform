package org.ow2.compatibleone.exchangeobjects;

public class RestRequest {
/*
struct  rest_request {
        struct  rest_header     * first;
        struct  rest_header     * last;
        char                    * method;
        char                    * object;
        char                    * parameters;
        char                    * version;
        int                     type;
        char                    * body;
        char                    * host;
};	 
 */

	private RestHeaderBlock all;
    private String method;
    private String object;
    private String parameters;
    private String version;
    private int type;
    private String body;
    private String host;
    
    public RestRequest(
    		RestHeaderBlock all,
    		String method, 
    		String object, 
    		String parameters, 
    		String version, 
    		int type, 
    		String body,
    		String host){
    	
    		this.all = all;
    		this.method = method;
    		this.object = object;
    		this.parameters = parameters;
    		this.version = version;
    		this.type = type;
    		this.body = body;
    		this.host = host;
    }
    public RestHeaderBlock getBlock() {
		return all;
	}
    public String getMethod() {
		return method;
	}
    public String getObject() {
		return object;
	}
    public String getParameters() {
		return parameters;
	}
    public String getVersion() {
		return version;
	}
    public int getType() {
		return type;
	}
    public String getBody() {
		return body;
	}
    public String getHost() {
		return host;
	}
    public String toString(){
    	return 
    		"[" + 
    		"block " +  all + ", \n" + 
    		"method " +  method + ", \n" + 
    		"object " +  object + ", \n" + 
    		"parameters " +  parameters + ", \n" + 
    		"version " +  version + ", \n" + 
    		"type " +  type + ", \n" + 
    		"body " +  body + ", \n" + 
    		"host " +  host + ", \n" + 
    		"]"  ;
    }
}
