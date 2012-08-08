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
