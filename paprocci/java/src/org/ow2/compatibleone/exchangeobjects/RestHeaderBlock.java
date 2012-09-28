package org.ow2.compatibleone.exchangeobjects;

import java.util.ArrayList;

public class RestHeaderBlock {

    private ArrayList<RestHeader> list;
    
    public RestHeaderBlock(){
    	list = new ArrayList<RestHeader>();
    }
    
    public void add(RestHeader e) {
    	list.add(e);
	}
    
    public void add(String name, String value) {
    	list.add(new RestHeader(name, value));
	}
    
    public String toString(){
    	String ret = "[";
    	for (RestHeader i: list){
    		ret = ret + i.toString() + ", ";
    	}
    	ret = ret + "]";
    	return ret;
    }
}
