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
	public static void main(String[] args) throws Exception{
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
	 * The procci is started through JNI (not through command line).
	 * @param args
	 * @return the initialized procci.
	 */
	public static Procci start(Object[] args) throws Exception{
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
