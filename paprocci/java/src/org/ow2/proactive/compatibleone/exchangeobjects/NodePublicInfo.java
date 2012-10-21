/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2012 by Oasis (INRIA Sophia Antipolis) and ActiveEon teams.     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */

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
	private String ipaddress;
	
	public NodePublicInfo(String nodeid, String nodeurl, CONodeState status, String hostname, String owner, String ipaddress){
		this.nodeid = nodeid;
		this.nodeurl = nodeurl;
		this.status = status.toString();
		this.hostname = hostname;
		this.owner = owner;
		this.ipaddress = ipaddress;
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
	
	public String getIpAddress() {
		return ipaddress;
	}
	
	public String toString(){
		Gson g = new Gson();
		return g.toJson(this);
	}
}
