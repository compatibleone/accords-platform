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

import java.util.Date;
import org.apache.log4j.Logger;
import org.ow2.proactive.scripting.*;

/**
 * This class creates selections scripts to select specific nodes among the whole 
 * set of nodes available in the RM. 
 */
public class SelectionScriptCreator {
	public static final String OR = "||";
	public static final String AND = "&&";
	
	private static Logger logger =						// Logger. 
			Logger.getLogger(SelectionScriptCreator.class.getName()); 
	private static final String NEWLINE = "\n";
	private static final String HEADER = 
		"importPackage(java.lang);"+ NEWLINE + 
		"importPackage(java.io);"+ NEWLINE + 
		"importPackage(java.util);"+ NEWLINE + 
		"importPackage(java.exception);"+ NEWLINE + 
		"importClass(org.ow2.proactive.scripting.helper.selection.SelectionUtils);" + NEWLINE;
	
	private static String starterTrue(){
		String script = "var selected; selected = true;" + NEWLINE;
		return script;
	}
	
	private static String starterFalse(){
		String script = "var selected; selected = false;" + NEWLINE;
		return script;
	}
	
	private static String printCurrentDecision(){
		String ret = 
	    "println(\"selected = \" + selected);" + NEWLINE;
		return ret;
	}
	
	private static String printTimeMark(){
		return "println(\">>>>" + (new Date().toString()) + "\");" + NEWLINE ;
	}
	
	public static String anyNode(){
		String script = "var selected; selected = true;" + NEWLINE;
		logger.info("Selection script created: " + script);
		return script;
	}
	
	/**
	 * Create a selection script based on the given operator (AND, OR, ...) and the given 
	 * conditions that the selected node/s must satisfy. 
	 * @param operator one of AND, OR, ...
	 * @param conditions conditions that the selected node must satisfy. 
	 * @return the SelectionScript. 
	 * @throws InvalidScriptException 
	 */
	public static SelectionScript createSelectionScript(String operator, SelectionScriptCondition ... conditions) throws InvalidScriptException{
		String script = null;
	
		if (operator == AND){
			script = HEADER + printTimeMark() + starterTrue() ;
		}else{
			script = HEADER + printTimeMark() + starterFalse() ;
		}
		
		for (SelectionScriptCondition cond: conditions){
			script += 
					NEWLINE + 
					cond.getCondition().replace("OPERATOR", operator) + 
					printCurrentDecision();
		}
		//logger.info("Selection script created: " + script);
		// Hack for \ characters that JavaScript interpret as escape characters.   
		script = script.replace("\\", "\\\\");
		return new SelectionScript(script, "JavaScript", true);
	}
	
	/* Testing purposes. */
	public static void main(String args[]) throws Exception{
		SelectionScript script = SelectionScriptCreator.createSelectionScript( 
				SelectionScriptCreator.AND, 
				SelectionScriptCondition.nodesContainingFile("/home/cosacs/run-cosacs"), 
				SelectionScriptCondition.nodeWithFileMark("/home/cosacs/filemark"), 
				SelectionScriptCondition.nodeWithoutSpecialLock("/home/cosacs/lock"),
				SelectionScriptCondition.nodeWithOS("linux"));
		System.out.println("Script: \n" + script);

		ScriptResult<Boolean> result = script.execute();
		System.out.println(result.getResult());
	}
}
