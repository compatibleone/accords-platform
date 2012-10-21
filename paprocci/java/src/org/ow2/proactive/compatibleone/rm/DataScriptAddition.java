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

import org.apache.log4j.Logger;
import org.ow2.proactive.scripting.GenerationScript;

/**
 * This class creates selections scripts to select specific nodes among the whole 
 * set of nodes available in the RM. 
 */
public class DataScriptAddition {
	private static Logger logger =						// Logger. 
			Logger.getLogger(DataScriptAddition.class.getName()); 
	private static final String NEWLINE = "\n";
	private String addition;
	
	private DataScriptAddition(String addition){
		this.addition = addition;
	}
	
	public String getAddition(){
		return addition;
	}
	
	public static DataScriptAddition getOS(){
		String addition = GenerationScript.RESULT_VARIABLE + " += java.lang.System.getProperty(\"os.name\").toLowerCase();" + NEWLINE;
		logger.info("Addition for script created: " + addition);
		return new DataScriptAddition(addition);
	}
	
	public static DataScriptAddition getCPUIdle(){
		String addition = ""; 
		addition += "importClass(org.hyperic.sigar.Sigar);" + NEWLINE;
		addition += "var sigar = new Sigar();" + NEWLINE;
		addition += "var usercpu = sigar.getCpuPerc().getIdle();" + NEWLINE;
		addition += GenerationScript.RESULT_VARIABLE + " += usercpu;" + NEWLINE;
		logger.info("Addition for script created: " + addition);
		return new DataScriptAddition(addition);
	}
	
	
	
}

