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
