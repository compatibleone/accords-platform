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

package org.ow2.proactive.compatibleone.misc;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;
import java.util.Set;

import org.apache.log4j.Logger;
import org.ow2.proactive.compatibleone.exceptions.ElementNotFoundException;

public class ProcciProperties {
	private static Logger logger =	Logger.getLogger(ProcciProperties.class.getName()); 
	private Properties prop;
	
	public ProcciProperties(){
		prop = new Properties();
	}
	
	public String getProperty(String key) throws ElementNotFoundException{
		String value = prop.getProperty(key);
		logger.info("Loading property " + key );
		if (value == null){
			throw new ElementNotFoundException("The key " + key + " has not been defined.");
		}
		logger.info("... value found: " + value );
		return value;
	}
	
	public void load(InputStream inStream) throws IOException{
		prop.load(inStream);
	}
	
	public Set<Object> keySet(){
		return prop.keySet();
	}
}
