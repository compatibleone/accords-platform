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

/**
 * This class allows to compare nodes ID that is provided by two different information
 * sources: RM (which provides the full node URL) and scheduler (which provides partial node URL). */
public class NodeId {
	
	private String id; 
	
	public int hashCode(){ // We force any comparison to be done through the methods equals(Object).
		return 1;
	}
	
	public NodeId(String id){
		this.id = id;
	}
	
	public String getId(){
		return id;
	}
	
	public boolean equals(Object target){
		if (target==null){
			return false;
		}
		if (target instanceof NodeId){
			NodeId nodetarget = (NodeId)target;
			if (nodetarget.id.endsWith(this.id))
				return true;
			if (this.id.endsWith(nodetarget.id))
				return true;
			return false;
		}else{
			return false;
		}
	}
}
