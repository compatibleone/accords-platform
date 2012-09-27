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
