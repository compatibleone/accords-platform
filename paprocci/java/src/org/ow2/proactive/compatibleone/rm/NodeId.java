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

package org.ow2.proactive.compatibleone.rm;

/**
 * This class allows to compare nodes ID that is provided by two different information
 * sources: RM (which provides the full node URL) and scheduler (which provides partial node URL). */
public class NodeId {
	
	private String id; 
	
	public int hashCode(){ // We force any comparison to be done through the methods equals(Object).
		return 1;
	}
	
	public NodeId(String id){
		this.id = id;
	}
	
	public String getId(){
		return id;
	}
	
	public boolean equals(Object target){
		if (target==null){
			return false;
		}
		if (target instanceof NodeId){
			NodeId nodetarget = (NodeId)target;
			if (nodetarget.id.endsWith(this.id))
				return true;
			if (this.id.endsWith(nodetarget.id))
				return true;
			return false;
		}else{
			return false;
		}
	}
}
