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

package org.ow2.proactive.compatibleone.pajavaprocci;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import org.apache.log4j.Logger;

/**
 * Class that helps initializing the ProActive environment, specially when referred to the PAMR router
 * connection parameters. */
public class PAEnvironmentInitializer {

	public static final String COMMUNICATION_PROTOCOL = "pamr";									// Default protocol to be used to get connected to the RM.
	public static Logger logger = Logger.getLogger(PAEnvironmentInitializer.class.getName());	// Logger.
	
    /** 
	 * Create a java.policy file to grant permissions, and load it for the current JVM. */
	public static void createPolicyAndLoadIt() throws Exception{
		try{
		    File temp = File.createTempFile("javapolicy", ".policy"); // Create temp file.
		    temp.deleteOnExit(); // Delete temp file when program exits.
		    // Write to temp file.
		    BufferedWriter out = new BufferedWriter(new FileWriter(temp));
		    String policycontent = "grant {permission java.security.AllPermission;};";
		    out.write(policycontent);
		    out.close();
		    String policypath = temp.getAbsolutePath(); 
		    System.setProperty("java.security.policy", policypath); // Load security policy.
		}catch(Exception e){
			throw new Exception("Error while creating the security policy file. " + e.getMessage());
		}
	}

	/**
	 * Initialize the ProActive configuration. It handles the PAMR router configuration.
	 * @param paconf ProActive configuration file (if needed).
	 * @throws Exception */
	public static void initPAConfiguration(String paconf) throws Exception{
		/* Load ProActive configuration. */
		if (paconf!=null){
			if (new File(paconf).exists()==true){
				System.setProperty("proactive.configuration", paconf);
				logger.info("The ProActive configuration file '"+paconf+"' is being used.");
			}else{
				logger.warn("The ProActive configuration file '"+paconf+"' was not found. Using default configuration.");
				throw new FileNotFoundException("ProActive configuration file '"+paconf+"' does not exist.");
			}
		}else{
			logger.warn("No ProActive configuration file provided.");
			throw new Exception("ProActive configuration file must be provided.");
		}
	}

}
