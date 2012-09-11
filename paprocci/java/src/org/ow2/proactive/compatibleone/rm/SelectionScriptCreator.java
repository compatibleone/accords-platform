/*
 * ################################################################
 *
 * ProActive Parallel Suite(TM): The Java(TM) library for
 *    Parallel, Distributed, Multi-Core Computing for
 *    Enterprise Grids & Clouds
 *
 * Copyright (C) 1997-2011 INRIA/University of
 *                 Nice-Sophia Antipolis/ActiveEon
 * Contact: proactive@ow2.org or contact@activeeon.com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation; version 3 of
 * the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 * USA
 *
 * If needed, contact us to obtain a release under GPL Version 2 or 3
 * or a different license than the AGPL.
 *
 *  Initial developer(s):               The ProActive Team
 *                        http://proactive.inria.fr/team_members.htm
 *  Contributor(s):
 *
 * ################################################################
 * $$PROACTIVE_INITIAL_DEV$$
 */

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
