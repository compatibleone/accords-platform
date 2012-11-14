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

package org.ow2.proactive.compatibleone.misc;

import org.apache.log4j.Logger;
import org.ow2.proactive.resourcemanager.common.NodeState;
import org.ow2.proactive.scheduler.common.task.TaskStatus;



public class CONodeState {
	public static Logger logger = 
			Logger.getLogger(CONodeState.class.getName()); 	// Logger.
	
	private String value;
	/** 
	 * Constructor. */
	public CONodeState(TaskStatus taskstatus){
		if       (taskstatus.equals(TaskStatus.RUNNING)){
			value = "RUNNING";
		}else if (taskstatus.equals(TaskStatus.PENDING)){
			value = "PREPARING";
		}else{
			value = "IDLE";
		}
	}
	
	public CONodeState(NodeState nodestate){
		if       (nodestate.equals(NodeState.BUSY)){
			value = "RUNNING";
		}else if (nodestate.equals(NodeState.DEPLOYING)){
			value = "PREPARING";
		}else{
			value = "IDLE";
		}
	}
	
	public String toString(){
		return value;
	}
	
}
