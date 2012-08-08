package org.ow2.proactive.compatibleone.exchangeobjects;

import org.ow2.proactive.compatibleone.misc.CONodeState;
import com.google.gson.Gson;

/**
 * This class represents a host/node. It is intended to describe
 * the json format of the json object that the user gets after each operation regarding nodes/hosts. */
public class NodePublicInfo {
	/*
	 * This method should return a json object with the following attributes: 
	 *  id 			jobId
	 *  adminPass	sshpass
	 *  status		status
	 *  hostId		hostname
	 *  private 		private ip address 
	 *  public 		public ip address
	 * */
	
	private String nodeid;
	private String nodeurl;
	private String status;
	private String hostname;
	private String owner;
	private String id;
				
	
	public NodePublicInfo(String nodeid, String nodeurl, CONodeState status, String hostname, String owner){
		this.nodeid = nodeid;
		this.nodeurl = nodeurl;
		this.status = status.toString();
		this.hostname = hostname;
		this.owner = owner;
	}
	
	public String getId() {
		return id;
	}
	
	public void setId(String id) {
		this.id = id;
	}
	
	public String getHostname() {
		return hostname;
	}
	
	public String getNodeURL() {
		return nodeurl; 
	}
	
	public String getStatus() {
		return status;
	}
	
	public String getNodeId() {
		return nodeid;
	}
	
	public String getOwner() {
		return owner;
	}
	
	public String toString(){
		Gson g = new Gson();
		return g.toJson(this);
	}
}
