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
import org.ow2.proactive.scripting.*;

/**
 * This class creates data scripts that allow to find out specific information about the whole 
 * set of nodes available in the RM. 
 */
public class DataScriptCreator {
	private static Logger logger =						// Logger. 
			Logger.getLogger(DataScriptCreator.class.getName()); 
	private static final String NEWLINE = "\n";
	private static final String HEADER = 
		"importPackage(java.lang);"+ NEWLINE + 
		"importPackage(java.io);"+ NEWLINE + 
		"importPackage(java.util);"+ NEWLINE + 
		"importPackage(java.exception);"+ NEWLINE + 
		"importClass(org.ow2.proactive.scripting.helper.selection.SelectionUtils);" + NEWLINE;
	
	private static String starter(){
		String script = "var " + GenerationScript.RESULT_VARIABLE + " = \"\";" + NEWLINE;
		return script;
	}
	
	
	/**
	 * Create a GenerationScript to get some information about all the available nodes in the RM. 
	 * The information is created by adding DataScriptAddition objects. Each of them 
	 * add a section in the GenerationScript that add to the returned value specific information
	 * (like the OS of the node for instance).
	 * @param additions
	 * @return the GenerationScript to be launched in the nodes of the RM. 
	 * @throws InvalidScriptException 
	 */
	public static GenerationScript createScript(DataScriptAddition ... additions) throws InvalidScriptException{
		String script = HEADER + starter() ;
		for (DataScriptAddition add: additions){
			script += 
					NEWLINE + 
					add.getAddition(); 
		}
		logger.info("Script created: " + script);
		return new GenerationScript(script, "JavaScript");
	}
	
	/* Testing. */
	public static void main(String args[]) throws Exception{
		System.out.println("Executing locally the script...\n");
		GenerationScript script = DataScriptCreator.createScript( 
				DataScriptAddition.getCPUIdle() 
				);
		System.out.println("Script used: \n" + script);

		ScriptResult<String> result = script.execute();
		System.out.println("Local result: \n" + result.getResult());
	}
}
