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
	private String when;
	private String status;
	
	public ProcciCategory(){
	}

	public ProcciCategory(String id, String name, String image, String profile,
			String node, String price, String number, String access,
			String reference, String publicaddr, String privateaddr,
			String nopanodes, String hostname, String workload, String account,
			String agent, String when, String status) {
		super();
		this.id = id;
		this.name = name;
		this.image = image;
		this.profile = profile;
		this.node = node;
		this.price = price;
		this.number = number;
		this.access = access;
		this.reference = reference;
		this.publicaddr = publicaddr;
		this.privateaddr = privateaddr;
		this.nopanodes = nopanodes;
		this.hostname = hostname;
		this.workload = workload;
		this.account = account;
		this.agent = agent;
		this.when = when;
		this.status = status;
	}
	
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getImage() {
		return image;
	}
	public void setImage(String image) {
		this.image = image;
	}
	public String getProfile() {
		return profile;
	}
	public void setProfile(String profile) {
		this.profile = profile;
	}
	public String getNode() {
		return node;
	}
	public void setNode(String node) {
		this.node = node;
	}
	public String getPrice() {
		return price;
	}
	public void setPrice(String price) {
		this.price = price;
	}
	public String getNumber() {
		return number;
	}
	public void setNumber(String number) {
		this.number = number;
	}
	public String getAccess() {
		return access;
	}
	public void setAccess(String access) {
		this.access = access;
	}
	public String getReference() {
		return reference;
	}
	public void setReference(String reference) {
		this.reference = reference;
	}
	public String getPublicaddr() {
		return publicaddr;
	}
	public void setPublicaddr(String publicaddr) {
		this.publicaddr = publicaddr;
	}
	public String getPrivateaddr() {
		return privateaddr;
	}
	public void setPrivateaddr(String privateaddr) {
		this.privateaddr = privateaddr;
	}
	public String getNopanodes() {
		return nopanodes;
	}
	public void setNopanodes(String nopanodes) {
		this.nopanodes = nopanodes;
	}
	public String getHostname() {
		return hostname;
	}
	public void setHostname(String hostname) {
		this.hostname = hostname;
	}
	public String getWorkload() {
		return workload;
	}
	public void setWorkload(String workload) {
		this.workload = workload;
	}
	public String getAccount() {
		return account;
	}
	public void setAccount(String account) {
		this.account = account;
	}
	public String getAgent() {
		return agent;
	}
	public void setAgent(String agent) {
		this.agent = agent;
	}
	public String getWhen() {
		return when;
	}
	public void setWhen(String when) {
		this.when = when;
	}
	public String getStatus() {
		return status;
	}
	public void setStatus(String status) {
		this.status = status;
	}
    
    
}
