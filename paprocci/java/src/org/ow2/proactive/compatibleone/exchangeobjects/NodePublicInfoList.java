package org.ow2.proactive.compatibleone.exchangeobjects;

import java.util.ArrayList;

import com.google.gson.Gson;

/**
 * This class represents info of many nodes. It is intended to describe
 * the json format of the json object that the user gets after each operation. */
public class NodePublicInfoList {
	private ArrayList<NodePublicInfo> nodes;
	
	public NodePublicInfoList(){
		nodes = new ArrayList<NodePublicInfo>();
	}
	
	public void add(NodePublicInfo h){
		nodes.add(h);
	}
	
	public ArrayList<NodePublicInfo> getHosts() {
		return nodes;
	}
	
	public String toString(){
		Gson g = new Gson();
		return g.toJson(this);
	}
}
