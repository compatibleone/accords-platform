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

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Date;

import org.apache.commons.cli.MissingOptionException;
import org.apache.commons.cli.UnrecognizedOptionException;
import org.apache.log4j.Logger;
import org.ow2.proactive.compatibleone.misc.Arguments;
import org.ow2.proactive.compatibleone.misc.Misc;
import org.ow2.proactive.compatibleone.misc.ProcciProperties;

/**
 * This class provides support to ProActiveProcci dealing
 * with command-line parameters treatment, help, version, etc. */
public final class ProcciParametersHandler {
	
	protected static Logger logger =						// Logger. 
			Logger.getLogger(ProcciParametersHandler.class.getName()); 
	private static final int INIT_UNK = 0;
	private static final int INIT_JNI = 1;
	private static final int INIT_CLI = 2;
	
	private int initialization;
	protected Arguments arguments; 							// Arguments given to the procci. 
	
	protected ProcciProperties properties;
	
	/** 
	 * Constructor of the prober. The map contains all the arguments for the probe to be executed. 
	 * @param args arguments for instantiation. */
	public ProcciParametersHandler(Arguments args){
		this();
		initialization = INIT_CLI;
		this.arguments = args;
		
		// Parameters regarding basic needs. 
		args.addNewOption("h", "help", false);													// Help message.                                	
		args.addNewOption("v", "debug", true, new Integer(0));             						// Level of verbosity.
		args.addNewOption("O", "logconf", true);												// Configuration file for log4j. 
		
		// Parameters regarding basic information needed.
		args.addNewOption("u", "user", true);													// User.
		args.addNewOption("p", "pass", true);													// Pass.
		args.addNewOption("a", "properties-file", true);										// Properties file.
		
		// Parameters regarding functionalities. 
		args.addNewOption("l", "list-nodes", false);											// List the nodes available.
		args.addNewOption("b", "get-cosacs", false);											// Get a node with a cosacs' instance running on it. 
		args.addNewOption("d", "release-node", true);											// Release a node. 
		args.addNewOption("j", "get-node-info", true);											// Get information of a node.  
		args.addNewOption("k", "get-node-output", true);										// Get stdout and stderr of a task (running in a node).  
		
		args.addNewOption("g", "select-by-os", true);
		args.addNewOption("n", "number-of-nodes", true);
		args.addNewOption("i", "list-os", false);
	}
	
	public ProcciParametersHandler(){
		this.properties = new ProcciProperties();
		initialization = INIT_JNI;
	}
	
	/**
	 * Specific initialization for the probe. 
	 * This method can be overwritten, but must be called during initialization. */
	public void initialize() throws Exception{ 
		if (initialization==INIT_CLI){
			try{
				getArgs().parseAll();
			}catch(MissingOptionException e){
				Misc.printMessageUsageAndExit(e.getMessage());
			}catch(UnrecognizedOptionException e){
				Misc.printMessageUsageAndExit(e.getMessage());
			}
			
			this.validateArguments(getArgs());				// Validate its arguments. In case of problems, it throws an IllegalArgumentException.
			Misc.log4jConfiguration(getArgs().getInt("debug"), getArgs().getStr("logconf"));	// Loading log4j configuration. 
		}else{
			// No command-line arguments. 
			Misc.log4jConfiguration(2, null);							// Loading log4j configuration. 
		}
		
		
		logger.info(">>>> Date: " + new Date());		// Print the date in the logs.
		if (initialization==INIT_CLI){
			getArgs().printArgumentsGiven();				// Print a list with the arguments given by the user. 
		
			if (getArgs().getBool("help") == true)	
				Misc.printMessageUsageAndExit("");
		}
		
		PAEnvironmentInitializer.createPolicyAndLoadIt();										// Security policy procedure.
		String filename;
		
		if (initialization==INIT_CLI){
			filename = this.getArgs().getStr("properties-file");
		}else{
			filename = "/var/lib/accords/paprocci/paprocci.properties";
		}
		
		File f = new File(filename);	// Load a set of properties for this procci.
		if(!f.exists()){
			throw new IOException("Properties file not found: " + filename);
		}else{
			FileInputStream in = new FileInputStream(f);
			properties.load(in);
		} 
		logger.info("Properties file loaded: " + filename);
		for (Object key: properties.keySet()){
			logger.info(" - Pair found: '" + key + "'  '" + properties.getProperty(key.toString()) + "'");
		}
		
		PAEnvironmentInitializer.initPAConfiguration(											// PAMR router configuration.
			properties.getProperty("proactive.configuration.file"));
	}
	
	
	/** 
	 * Validate all the arguments. 
	 * This method can be overwritten, but must be called during validation.
	 * @param args arguments to be validated.
	 * @throws IllegalArgumentException in case a non-valid argument is given. */
	private void validateArguments(Arguments args) throws IllegalArgumentException{
		if (!args.getBool("help")){
			args.checkIsValidInt("debug", 0, 3);
			args.checkIsGiven("user");		
			args.checkIsGiven("pass");		
			args.checkIsGiven("properties-file");	
		}
	}
	
	/**
	 * Get set of arguments given by the user.
	 * @return arguments. */
	public Arguments getArgs(){
		return arguments;
	}
	
	/**
	 * Get set of arguments given by the user through the properties file.
	 * @return properties. */
	public ProcciProperties getProperties(){
		return properties;
	}
	
}
