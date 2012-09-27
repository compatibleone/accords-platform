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

/**
 * This class creates selections scripts to select specific nodes among the whole 
 * set of nodes available in the RM. 
 */
public class SelectionScriptCondition {
	private static Logger logger =						// Logger. 
			Logger.getLogger(SelectionScriptCondition.class.getName()); 
	private static final String NEWLINE = "\n";
	private String condition;
	
	private SelectionScriptCondition(String condition){
		this.condition = condition;
	}
	
	public String getCondition(){
		return condition;
	}
	
	/**
	 * Returns a condition such that any node is selected with it. 
	 * @return the condition.
	 */
	public static SelectionScriptCondition anyNode(){
		String condition = "var selected; selected = true;";
		//logger.info("Condition for selection script created: " + condition);
		return new SelectionScriptCondition(condition);
	}
	
	/**
	 * Returns a condition such that only nodes matching the given hostname are selected. 
	 * @param hostname the name of the host.
	 * @return the condition.
	 */
	public static SelectionScriptCondition nodeByHostname(String hostname){
		String condition = 
				"if (SelectionUtils.checkHostName(\"HOSTNAME\")) " + NEWLINE + 
				"{selected = selected OPERATOR true; println(\"hostname = HOSTNAME ==> SELECTED\");}"+ NEWLINE + 
				"else "+ NEWLINE + 
				"{selected = selected OPERATOR false; println(\"hostname /= HOSTNAME ==> NOT selected\");}" + NEWLINE ; 
		condition = condition.replace("HOSTNAME", hostname);
		//logger.info("Condition for selection script created: " + condition);
		return new SelectionScriptCondition(condition);
		                     
	}
	
	/**
	 * Returns a condition such that only nodes where the given file exists are selected. 
	 * @param filename of the file to be checked.
	 * @return the condition.
	 */
	public static SelectionScriptCondition nodesContainingFile(String filename){
		String condition = 
	    "if (SelectionUtils.checkFileExist(\"FILENAME\"))"+ NEWLINE +
	    "{selected = selected OPERATOR true; println(\"FILENAME exists ==> SELECTED\");}"+NEWLINE +
	    "else"+NEWLINE +
	    "{selected = selected OPERATOR false;println(\"FILENAME does not exist ==> NOT selected\");}" + NEWLINE;
		condition = condition.replace("FILENAME", filename);
		//logger.info("Condition for selection script created: " + condition);
		return new SelectionScriptCondition(condition);
	}
	
	/**
	 * Returns a condition such that only nodes where the given file exists are selected. 
	 * @param filename of the file to be checked.
	 * @return the condition.
	 */
	public static SelectionScriptCondition nodesNotContainingFile(String filename){
		String condition = 
	    "if (SelectionUtils.checkFileExist(\"FILENAME\"))"+NEWLINE +
	    "{selected = selected OPERATOR false; println(\"FILENAME exists ==> NOT selected\");}"+NEWLINE +
	    "else"+NEWLINE +
	    "{selected = selected OPERATOR true; println(\"FILENAME does not exist ==> SELECTED\");}" + NEWLINE;
		condition = condition.replace("FILENAME", filename);
		//logger.info("Condition for selection script created: " + condition);
		return new SelectionScriptCondition(condition);
	}

	/**
	 * Returns a condition such that only nodes where the given special lock file DOES NOT exist are selected. 
	 * The special lock file consists of: prefix + '-' + hostname 
	 * As an example, if this selection script is executed in a ProActive node whose host is 'node99', 
	 * and the prefix given is '/home/user/lock', the node will be selected only if the file 
	 * named '/home/user/lock-node99' exists. 
	 * @param speciallockprefix prefix of the file to be checked. 
	 * @return the condition.
	 */
	public static SelectionScriptCondition nodeWithoutSpecialLock(String speciallockprefix){
		String condition = 
		"var host = \"-\" + java.net.InetAddress.getLocalHost().getHostName();" + NEWLINE +
		"var filet = \"FILENAME\" + host; " + NEWLINE + 
	    "if (SelectionUtils.checkFileExist(filet))"+NEWLINE +
	    "{selected = selected OPERATOR false; println(filet + \" exists ==> NOT selected\");}"+NEWLINE +
	    "else"+NEWLINE +
	    "{selected = selected OPERATOR true; println(filet + \" does not exist ==> SELECTED\");}" + NEWLINE;
		condition = condition.replace("FILENAME", speciallockprefix);
		//logger.info("Condition for selection script created: " + condition);
		return new SelectionScriptCondition(condition);
	}
	
	/**
	 * In order to be selected by this condition, a node MUST contain in its filesystem the file mark. 
	 * The file mark name consists of: prefix + '-' + hostname
	 * where prefix is replaced by the given parameter, and hostname is obtained in the host where the script is launched.
	 * @param prefix of the file mark whose existence will be checked
	 * @return the condition.
	 */
	public static SelectionScriptCondition nodeWithFileMark(String prefix){
		String condition = 
		"var host = \"-\" + java.net.InetAddress.getLocalHost().getHostName();" + NEWLINE +
		"var filet = \"FILENAME\" + host; " + NEWLINE + 
	    "if (SelectionUtils.checkFileExist(filet))"+NEWLINE +
	    "{selected = selected OPERATOR true; println(filet + \" exists ==> SELECTED\");}"+NEWLINE +
	    "else"+NEWLINE +
	    "{selected = selected OPERATOR false; println(filet + \" does not exist ==> NOT selected\");}" + NEWLINE;
		condition = condition.replace("FILENAME", prefix);
		//logger.info("Condition for selection script created: " + condition);
		return new SelectionScriptCondition(condition);
	}
	
	/**
	 * Returns a condition such that only nodes whose Operative System name (obtained from the 
	 * java property os.name) strictly contain the given ostarget name.
	 * @param ostarget string that must be entirely contained by the OS name of the node.
	 * @return the condition.
	 */
	public static SelectionScriptCondition nodeWithOS(String ostarget){
		String condition = 
		"var os = java.lang.System.getProperty(\"os.name\").toLowerCase();" + NEWLINE +
	    "if (os.indexOf(\"OSTARGET\") >= 0)"+NEWLINE +
	    "{selected = selected OPERATOR true; println(os + \" matches OSTARGET ==> SELECTED\");}"+NEWLINE +
	    "else"+NEWLINE +
	    "{selected = selected OPERATOR false; println(os + \" does not match OSTARGET ==> NOT selected\");}" + NEWLINE;
		condition = condition.replace("OSTARGET", ostarget.toLowerCase());
		//logger.info("Condition for selection script created: " + condition);
		return new SelectionScriptCondition(condition);
	}
	
	
	/**
	 * In order to be selected by this condition, a node must have an idle CPU percentage value (in %) greater than
	 * the given minimum value. 
	 * @param minimum value of idle CPU (as %, between 0 and 1). 
	 * @return the SelectionScriptCondition associated. 
	 */
	public static SelectionScriptCondition nodeWithCPUIdleGreaterThan(double minimum){
		String condition = "";
		condition += "importClass(org.hyperic.sigar.Sigar);" + NEWLINE;
		condition += "var sigar = new Sigar();" + NEWLINE;
		condition += "var usercpu = sigar.getCpuPerc().getUser();" + NEWLINE;
	    condition += "if (usercpu < MINIMUM)"+NEWLINE ;
	    condition += "{selected = selected OPERATOR true; println(usercpu \" < than minimum MINIMUM ==> SELECTED\");}"+NEWLINE ;
	    condition += "else"+NEWLINE ;
	    condition += "{selected = selected OPERATOR false; println(usercpu \" > than minimum MINIMUM  ==> NOT selected\");}" + NEWLINE;
	    condition = condition.replace("MINIMUM", new Double(minimum).toString());
		//logger.info("Condition for selection script created: " + condition);
		return new SelectionScriptCondition(condition);
	}
	/**
	 * In order to be selected by this condition, a node must have an X greater than
	 * the given minimum value. 
	 * @param minimum value of X. 
	 * @return the SelectionScriptCondition associated. 
	 */
	public static SelectionScriptCondition nodeWithXGreaterThan(String X, double minimum){
		//X = "getCpuPerc().getUser()"
		String condition = "";
		condition += "importClass(org.hyperic.sigar.Sigar);" + NEWLINE;
		condition += "var sigar = new Sigar();" + NEWLINE;
		condition += "var usercpu = sigar.XXX;" + NEWLINE;
	    condition += "if (usercpu < MINIMUM)"+NEWLINE ;
	    condition += "{selected = selected OPERATOR true; println(usercpu \" < than minimum MINIMUM ==> SELECTED\");}"+NEWLINE ;
	    condition += "else"+NEWLINE ;
	    condition += "{selected = selected OPERATOR false; println(usercpu \" > than minimum MINIMUM  ==> NOT selected\");}" + NEWLINE;
	    condition = condition.replace("MINIMUM", new Double(minimum).toString());
	    condition = condition.replace("XXX", X);
		//logger.info("Condition for selection script created: " + condition);
		return new SelectionScriptCondition(condition);
	}
}
