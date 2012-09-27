package org.ow2.proactive.compatibleone.exchangeobjects;

import org.ow2.proactive.compatibleone.misc.CONodeState;

import com.google.gson.Gson;

/**
 * This class represents a host/node. It is intended to be translated into a 
 * json object respecting a particular format. */
public class NodeInfo {
	private String id;
	private String adminPass;
	private String status;
	private String hostname;
	private String privateip;
	private String publicip;
	private String nodeurl;
	private String owner;
	
	public NodeInfo(
			String id, String adminPass, CONodeState status, 
			String hostname, String privateip, String publicip, String nodeurl, String owner){
		this.id = id;
		this.adminPass = adminPass;
		this.status = status.toString();
		this.hostname = hostname;
		this.privateip = privateip;
		this.publicip = publicip;
		this.nodeurl = nodeurl;
		this.owner = owner;
	}
	
	public String getHostname() {
		return hostname;
	}
	
	public String getPassword() {
		return adminPass;
	}
	
	public String getPrivateip() {
		return privateip;
	}
	
	public String getPublicip() {
		return publicip;
	}
	
	public String getStatus() {
		return status.toString();
	}
	
	public String getId() {
		return id;
	}
	
	public String getNodeurl() {
		return nodeurl;
	}	
	
	public String getOwner() {
		return owner;
	}	
	
	public String toString(){
		Gson g = new Gson();
		return g.toJson(this);
	}
}
