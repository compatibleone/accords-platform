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
	private static final String CONF_PATH = "/var/lib/accords/paprocci/";
	private static final String PROPERTIES_FILE = "paprocci.properties";
	private static final String LOG4J_FILE = "log4j.properties";
	
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
		args.addNewOption("r", "params", true);												    // Parameters for a specific command.  
		
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
			//Misc.log4jConfiguration(2, null);							// Loading log4j configuration. 
			Misc.log4jConfiguration(3, CONF_PATH + LOG4J_FILE);			// Loading log4j configuration. 
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
			filename = CONF_PATH + PROPERTIES_FILE ;
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
