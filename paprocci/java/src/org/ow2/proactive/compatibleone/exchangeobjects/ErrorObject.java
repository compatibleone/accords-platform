package org.ow2.proactive.compatibleone.exchangeobjects;

import com.google.gson.Gson;

/**
 * Instances of this class are supposed to be translated into json objects that respect a 
 * particular format, such that the client can interpret correctly any problem. */
public class ErrorObject {
	
	private String error;
				
	public ErrorObject(
			Exception e){
		this.error = e.getClass().getName() + ", message " + e.getMessage();
	}
	
	public String getError() {
		return error;
	}
	
	public String toString(){
		Gson g = new Gson();
		return g.toJson(this);
	}
}
