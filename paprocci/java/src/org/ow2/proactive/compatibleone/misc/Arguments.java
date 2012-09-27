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

package org.ow2.proactive.compatibleone.misc;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.GnuParser;
import org.apache.commons.cli.MissingOptionException;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.Parser;
import org.apache.log4j.Logger;



/**
 * An Arguments object contains all the arguments (entries) that a probe class might need to perform the test.
 * We control that each get has a valid key. */
public class Arguments {
	public static Logger logger = 
			Logger.getLogger(Arguments.class.getName()); 	// Logger.
	private Options options;								// Set of arguments given by the user.
	private ArrayList<String> optionsLongOpt;				// Workaround to get a list of the expected arguments. options.getOptions does not work.
	private HashMap<String, Object> values;					// Set of argument values given by the user.
	private HashMap<String, Object> defaultValues;			// Set of argument values to be used if the user do not provide them.
	private CommandLine parser;								// Parser itself.
	private String[] arguments;								// Raw arguments given by the user.
	
	/** 
	 * Constructor. */
	public Arguments(String[] arguments){
		values = new HashMap<String, Object>();
		optionsLongOpt = new ArrayList<String>();		// Set of arguments given by the user.
		defaultValues = new HashMap<String, Object>();
		options = new Options();						// Configuration for each argument. 
		this.arguments = arguments;
	}
	
	/**
	 * Add a new expected option/argument.
	 * @param shortname a short form to cite the flag (-x).
	 * @param longname a long form to cite the flag (--execution).
	 * @param hasarguments whether this flag expects arguments. */
	public void addNewOption(String shortname, String longname, Boolean hasarguments){
		addNewOption(shortname, longname, hasarguments, null);
	}

	/**
	 * Add a new expected option/argument.
	 * @param shortname a short form to cite the flag (-x).
	 * @param longname a long form to cite the flag (--execution).
	 * @param hasarguments whether this flag expects arguments.
	 * @param defaultValue default value to return in case the user does not provide this flag's value. */
	public void addNewOption(String shortname, String longname, Boolean hasarguments, Object defaultValue){
		Option option = new Option(shortname, longname, hasarguments, "");
		options.addOption(option);
		if (defaultValue!=null)
			defaultValues.put(longname, defaultValue);
		optionsLongOpt.add(longname);
	}
	
	/**
	 * Perform the parsing of the raw arguments.
	 * @throws ParseException in case of an unexpected format of the command-line. 
	 * @throws MissingOptionException in case there is a missing argument. */
	public void parseAll() throws ParseException, MissingOptionException{
        Parser parserrr = new GnuParser();
        parser = parserrr.parse(options, arguments);
	}
	
	/**
	 * Print a summary with the argument given by the user. */
	public void printArgumentsGiven(){
		for (String key: keySet())			// Show all the arguments considered. 
			logger.info("\t" + key + ": isPresent="+ isPresent(key) + " value='" + get(key) + "' defaultValue='" + defaultValues.get(key) + "'.");
	}
	
	/**
	 * Get a String provided the key.
	 * @param key key of the argument.
	 * @return value of the argument. If it was not given, it will return the default value. */
	public String getStr(String key){
		checkKey(key);
		String ret = parser.getOptionValue(key);
		if (ret == null){
			ret = (String)defaultValues.get(key);
		}	
		return ret;
	}
	
	/**
	 * Get a Object provided the key.
	 * @param key key of the argument.
	 * @return value of the argument. If it was not given, it will return the default value. */
	public Object getValue(String key){
		checkKey(key);
		Object ret = parser.getOptionValue(key);
		if (ret == null){
			ret = defaultValues.get(key);
		}	
		return ret;
	}
	
	/**
	 * Get an Integer provided the key.
	 * @param key key of the argument.
	 * @return value of the argument. If it was not given, it will return the default value. */
	public Integer getInt(String key){
		checkKey(key);
		return Misc.parseInteger(parser.getOptionValue(key), (Integer)defaultValues.get(key));
	}
	
	/**
	 * Check that the provided key was provided by the user. 
	 * @param key key of the argument. 
	 * @throws IllegalArgumentException if something goes wrong. */
	public void checkIsGiven(String key) throws IllegalArgumentException{
		checkKey(key);
		if (parser.getOptionValue(key) == null){
			throw new IllegalArgumentException("The argument '" + key + "' is not given.");
		}
	}
	
	/**
	 * Check that the provided key has a value that is valid as an Integer. If the value is null, just skips the checking. 
	 * To check if the value is given use checkIsGiven.
	 * @param key key of the argument. 
	 * @param minincluded lower-bound. 
	 * @param maxincluded upper-bound. 
	 * @throws IllegalArgumentException if something goes wrong. */
	public void checkIsValidInt(String key, Integer minincluded, Integer maxincluded) throws IllegalArgumentException{
		checkKey(key);
		String value = parser.getOptionValue(key);
		if (value == null){
			return;
		}
		Integer inte = null; 
		try{
			inte = Integer.parseInt(value);
		}catch(Exception e){
			throw new IllegalArgumentException("The argument '" + key + "' is not correct. " + e.getMessage());
		}
		if (inte < minincluded || inte > maxincluded){
			throw new IllegalArgumentException("The argument '" + key + "' is out of the bounds [" + minincluded + ", " + maxincluded + "].");
		}
	}
	
	/**
	 * Get a boolean provided the key.
	 * @param key key of the argument.
	 * @return whether this argument was provided or not. */
	public Boolean getBool(String key){
		checkKey(key);
		return new Boolean(parser.hasOption(key));
	}

	/**
	 * Get a boolean telling if the user provided the value.
	 * @param key key of the argument.
	 * @return whether this argument was provided or not. */
	public Boolean isGiven(String key){
		checkKey(key);
		return new Boolean(parser.hasOption(key));
	}

	/**
	 * Perform a checking on the given key (if it was not expected, it throws an exception).
	 * @param key key to be checked. */
	private void checkKey(String key){
		if (optionsLongOpt.contains(key)==false)
			throw new RuntimeException("Problem trying to use key '" + key + "'.");
	}
	
	/**
	 * Simple put method.
	 * @param key key to be used for the new entry.
	 * @param value value to be used for the new entry. */
	public void put(String key, Object value){
		values.put(key, value);
	}
	
	/**
	 * Simple get method. 
	 * It controls that the given key has been 'put' before by a 'put' method.
	 * @param key key to get the right entry.
	 * @return returns the value of the entry/argument. */
	private Object get(String key){
		return parser.getOptionValue(key);
	}
	
	/**
	 * Tell whether the key has been given by the user or not.
	 * @param key to analyse.
	 * @return true if the user provided this key (with or without value). */
	private Boolean isPresent(String key){
		return parser.hasOption(key);
	}
	
	/**
	 * Get the whole key set.
	 * @return key set. */
	private List<String> keySet(){
		return optionsLongOpt;
	}
	
}
