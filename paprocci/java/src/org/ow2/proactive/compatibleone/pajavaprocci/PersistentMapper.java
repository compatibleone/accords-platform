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
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Properties;

import org.apache.log4j.Logger;

/**
 * This class helps in the mapping between uuid and jobid when a node is locked/released. 
 * Contains a Map key:value that is saved into a file everytime it changes. 
 * */
public class PersistentMapper {

	private static Logger logger =						// Logger. 
			Logger.getLogger(ProcciParametersHandler.class.getName()); 
	
	private String filename; // Filename of the file used for persistence. 
	
	/**
	 * Constructor. 
	 * @param filename of the file used for persistence. */
	public PersistentMapper(String filename){
		this.filename = filename;
	}
	
	/**
	 * Get a value.
	 * @param key to get the value. 
	 * @return the value of the given key.
	 * @throws IOException */
	public String getValue(String key) throws IOException{
		logger.info("Getting value of key: " + key);
		String value = readProperty(key);
		logger.info("key:value " + key + ":" + value);
		return value;
	}
	
	/**
	 * Put a key:value pair. 
	 * It also saves the Map into the file.
	 * @param key
	 * @param value
	 * @throws IOException */
	public void putPair(String key, String value) throws IOException{
		logger.info("Writing key:value " + key + ":" + value);
		writeProperty(key, value);
	}
	
	/**
	 * Remove a key:value pair. 
	 * It also saves the Map into the file with the given changes. 
	 * @param key
	 * @param value
	 * @throws IOException */
	public void removePair(String key) throws IOException{
		logger.info("Removing key " + key);
		removeProperty(key);
	}
	
	private String readProperty(String key) throws IOException{
		Properties pro = new Properties();
		String value;
		File f = new File(filename);
		if(!f.exists()){
			throw new IOException("File not found: " + filename);
		}else{
			FileInputStream in = new FileInputStream(f);
			pro.load(in);
			value = pro.getProperty(key);
			if (value==null){
				throw new IOException("Key '" + key + "' not found. ");
			}
		}
		return value;
	}
	
	private void writeProperty(String key, String value) throws IOException{
		Properties pro = new Properties();
		File f = new File(filename);
		f.createNewFile();
		FileInputStream in = new FileInputStream(f);
		pro.load(in);
		pro.setProperty(key, value);
		pro.store(new FileOutputStream(filename),null);
	}
	
	private void removeProperty(String key) throws IOException{
		Properties pro = new Properties();
		File f = new File(filename);
		f.createNewFile();
		FileInputStream in = new FileInputStream(f);
		pro.load(in);
		pro.remove(key);
		pro.store(new FileOutputStream(filename),null);
	}
}
