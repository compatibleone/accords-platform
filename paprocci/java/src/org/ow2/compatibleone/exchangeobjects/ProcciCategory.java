package org.ow2.compatibleone.exchangeobjects;

import java.util.HashMap;

public class ProcciCategory {

/*
        char *  id;

        char *  name;
        //char *        flavor;
        char *  image;
        char *  profile;

        char *  node;
        char *  price;
        //char *        account;

        char *  number;     // uuid of the ProActive node request? 
        char *  access;
        //char *        rootpass;   
        char *  reference;
        char *  publicaddr;
        char *  privateaddr;
        char *  nopanodes;

        char *  hostname;
        char *  workload;
        char *  account;
        char *  agent;

        int     when;

        int     status;
};	 
 */

	private String id;
	private String name;
	private String image;
	private String profile;
	private String node;
	private String price;
	private String number;
	private String access;
	private String reference;
	private String publicaddr;
	private String privateaddr;
	private String nopanodes;
	private String hostname;
	private String workload;
	private String account;
	private String agent;
	private int when;
	private int status;
    
    public ProcciCategory(String id){
    	this.id = id;
    }
    
    public String getId() {
		return id;
	}
    
}
