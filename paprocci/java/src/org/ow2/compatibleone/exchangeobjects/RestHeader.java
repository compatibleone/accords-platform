package org.ow2.compatibleone.exchangeobjects;

public class RestHeader {

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

	private RestHeader previous;
	private RestHeader next;
	private RestHeader parent;
    private String name;
    private String value;
    
    public RestHeader(
    		RestHeader previous, 
    		RestHeader next, 
    		RestHeader parent, 
    		String name,
    		String value){
    	
    		this.previous = previous;
    		this.next = next;
    		this.parent = parent;
    		this.name = name;
    		this.value = value;
    }
    
    public RestHeader(String name, String value){
    		this.previous = null;
    		this.next = null;
    		this.parent = null;
    		this.name = name;
    		this.value = value;
    }
    
    public RestHeader(){
    		this.previous = null;
    		this.next = null;
    		this.parent = null;
    		this.name = null;
    		this.value = null;
    }
    
    public RestHeader getPrevious() {
		return previous;
	}
    public RestHeader getNext() {
		return next;
	}
    public RestHeader getParent() {
		return parent;
	}
    public void setPrevious(RestHeader previous) {
		this.previous = previous;
	}
    public void setNext(RestHeader next) {
    	this.next = next;
	}
    public void setParent(RestHeader parent) {
    	this.parent = parent;
	}
    public String getName() {
		return name;
	}
    public String getValue() {
		return value;
	}
    public String toString(){
    	return "[" + "name " +  name + ", " + "value " +  value + "]"  ;
    }
}
