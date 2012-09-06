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

package org.ow2.compatibleone;

import org.ow2.proactive.compatibleone.misc.Arguments;
import org.ow2.proactive.compatibleone.misc.Misc;
import org.ow2.proactive.compatibleone.pajavaprocci.ProActiveProcci;
import org.ow2.proactive.compatibleone.pajavaprocci.ProcciParametersHandler;

/**
 * Starter class for the ProActive Procci Java module. */
public class Starter {

	public static void exit(){
	}

	/**
	 * The procci is started through command line, not through JNI.
	 * @param args
	 */
	public static void main(String[] args){
		Arguments options = new Arguments(args);
		ProcciParametersHandler prob = new ProcciParametersHandler(options); 
		try{
			prob.initialize();
		}catch(Throwable t){
			t.printStackTrace();
			Misc.forcedExit();
		}
		ProActiveProcci procci = new ProActiveProcci(prob.getProperties()); 
		procci.initialize(prob.getArgs().getStr("user"), prob.getArgs().getStr("pass"));  	
		procci.command(prob.getArgs());  	
	}
	
	/**
	 * The procci is started through JNI.
	 * @param args
	 * @return the initialized procci.
	 */
	public static Procci start(Object[] args){
		// For this implementation: user=args[0] and pass=args[1]
		String user = args[0].toString();
		String pass = args[1].toString();
		ProcciParametersHandler prob = new ProcciParametersHandler(); 
		try{
			prob.initialize();
		}catch(Throwable t){
			t.printStackTrace();
			Misc.forcedExit();
		}

		ProActiveProcci procci = new ProActiveProcci(prob.getProperties()); 
		procci.initialize(user, pass);  	
		return procci;
	}

}
