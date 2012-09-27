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

