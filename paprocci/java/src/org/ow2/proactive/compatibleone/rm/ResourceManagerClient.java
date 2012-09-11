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

import java.security.KeyException;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.List;
import java.util.Set;
import javax.security.auth.login.LoginException;
import org.apache.log4j.Logger;
import org.ow2.proactive.authentication.crypto.CredData;
import org.ow2.proactive.authentication.crypto.Credentials;
import org.ow2.proactive.compatibleone.exchangeobjects.NodePublicInfo;
import org.ow2.proactive.compatibleone.exchangeobjects.NodePublicInfoList;
import org.ow2.proactive.compatibleone.misc.CONodeState;
import org.ow2.proactive.compatibleone.monitoring.NodeMonitor;
import org.ow2.proactive.resourcemanager.authentication.RMAuthentication;
import org.ow2.proactive.resourcemanager.common.NodeState;
import org.ow2.proactive.resourcemanager.common.event.RMNodeEvent;
import org.ow2.proactive.resourcemanager.exception.RMException;
import org.ow2.proactive.resourcemanager.frontend.RMConnection;
import org.ow2.proactive.resourcemanager.frontend.ResourceManager;
import org.ow2.proactive.resourcemanager.utils.TargetType;
import org.ow2.proactive.scripting.GenerationScript;
import org.ow2.proactive.scripting.ScriptResult;

/**
 * Class that gets basic information about the Resource Manager. 
 * Respects a Singleton pattern. 
 */
public class ResourceManagerClient {
	public static Logger logger = Logger.getLogger(ResourceManagerClient.class.getName()); 	// Logger.
	
	private static String url_backup;
	private static String username_backup;
	private static String password_backup;
	
	private ResourceManager rm;
	private RMAuthentication auth;
	private Credentials cred;
	
	public static void setUpInitParameters(String url, String user, String pass){
		url_backup = url;
		username_backup = user;
		password_backup = pass;
	}
	
	public static ResourceManagerClient getInstance() throws Exception{
        logger.info("Initializing instance of RM...");
		return new ResourceManagerClient(url_backup, username_backup, password_backup);
	}
	
	/**
	 * Constructor.
	 * @param url url of the RM.
	 * @param user user to get connected to the RM.
	 * @param pass pass to get connected to the RM. 
	 * @throws RMException 
	 * @throws LoginException 
	 * @throws KeyException */
	private ResourceManagerClient(String url, String user, String pass) throws KeyException, LoginException, RMException{
		initializeRM(url, user, pass);
	}
	
	/**
	 * Initialize the connection with the remote Resource Manager.
	 * Uses the url of the RM, and the user/pass to login to it. */
	private void initializeRM(String url, String user, String pass) throws RMException, KeyException, LoginException{
    	logger.info("Joining the Resource Manager at '" + url + "'...");
        RMAuthentication auth = RMConnection.join(url); 	// Join the RM.
        logger.info("Done.");
        logger.info("Creating credentials...");
        Credentials cred = Credentials.createCredentials(new CredData(user, pass), auth.getPublicKey());
        logger.info("Done.");
        logger.info("Logging in...");
        ResourceManager rmStub = auth.login(cred);								// Login against the RM.
        logger.info("Done.");
        this.auth = auth;
        this.cred = cred;
        this.rm = rmStub;
	}
	
    public Object monitorNode(String nodeurl, String mbean, String attribute) throws Exception{
    	NodeMonitor monit = new NodeMonitor(auth, cred);
        logger.info("Created NodeMonitor element...");
    	return monit.monitorNode(nodeurl, mbean, attribute);
    }
	
    public Object monitorNode(String nodeurl, String property) throws Exception{
    	NodeMonitor monit = new NodeMonitor(auth, cred);
        logger.info("Created NodeMonitor element (using property '"+property+"')...");
    	return monit.monitorNode(nodeurl, property);
    }
    
	/**
	 * Get public information about the node. 
	 * If a mapping between locknode-job is given, use it to extend the given information about the node. 
	 * @param node a string containing information (ideally the url) of a particular node. 
	 * @param node2job list containing a mapping between lock-jobs and nodes. 
	 * @return Information about the node. */
	public NodePublicInfo getNodePublicInfo(String node, Hashtable<NodeId, String> node2job) throws Exception{
        List<RMNodeEvent> listne = rm.getMonitoring().getState().getNodesEvents();
        if (listne.size() == 0) {
            logger.info("No nodes handled by Resource Manager");
        } else { 
            for (RMNodeEvent evt : listne) {
            	
	            logger.debug(" - Comparing '" + evt.getNodeUrl() + "' with target '" + node + "'...");
            	if(new NodeId(evt.getNodeUrl()).equals(new NodeId(node))){
		            logger.info("   - Found!");
                	NodePublicInfo h = generateNodePublicInfo(evt);
                	if(node2job!=null){
	                	String jobid = node2job.get(new NodeId(evt.getNodeUrl()));
	                	h.setId(jobid);
                	}
	            	return h;
            	}
            }
        }
		throw new Exception("No " + node + " was found.");
	}
	
	/**
	 * Generate node public information from a RMNodeEvent.
	 * @param evt source of information.
	 * @return the NodePublicInfo object. */
	private NodePublicInfo generateNodePublicInfo(RMNodeEvent evt){
		CONodeState state = new CONodeState(evt.getNodeState()); 
    	NodePublicInfo h = new NodePublicInfo(evt.getNodeUrl(), evt.getNodeUrl(), state, evt.getHostName(), evt.getNodeOwner());
    	return h;
	}
	
	
	/**
	 * Get a list containing information about all the nodes. 
	 * @param node2job list containing a mapping between lock-jobs and nodes. 
	 * @return the list. 
	 * @throws Exception if anything goes wrong. */
	public NodePublicInfoList getAllTheNodesInfo(Hashtable<NodeId, String> node2job) throws Exception{
		NodePublicInfoList hosts = new NodePublicInfoList();
		 try{
            List<RMNodeEvent> listne = rm.getMonitoring().getState().getNodesEvents();
            if (listne.size() == 0) {
                logger.info("No nodes handled by Resource Manager");
            } else { 
                for (RMNodeEvent evt : listne) {
                	NodePublicInfo h = generateNodePublicInfo(evt);
	                //logger.info("- Checking id for node: " + evt.getNodeUrl());
                	if(node2job!=null){
	                	String jobid = node2job.get(new NodeId(evt.getNodeUrl()));
	                	h.setId(jobid);
                	}
                	hosts.add(h);
                }
            }
        } catch (Exception e) {
            logger.warn("Error while retreiving node information", e);
            throw e;
	    }
		return hosts;
	}
	
	/**
	 * Get a set of urls of the nodes added to the RM.  
	 * @param free return only free nodes. 
	 * @return the set of urls.
	 * @throws Exception if anything goes wrong. */
	public HashSet<String> getNodesUrl(boolean free) throws Exception{
		HashSet<String> nodesurl = new HashSet<String>();
		 try{
            List<RMNodeEvent> listne = rm.getMonitoring().getState().getNodesEvents();
            if (listne.size() == 0) {
                logger.info("No nodes handled by Resource Manager");
            } else { 
                for (RMNodeEvent evt : listne) {
                	if (!free || evt.getNodeState() == NodeState.FREE)
	                	nodesurl.add(evt.getNodeUrl());
                }
            }
        } catch (Exception e) {
            logger.warn("Error while retreiving node information", e);
            throw e;
	    }
		return nodesurl;
	}
	
	public List<ScriptResult<String>> runScript(Set<String> nodeurls, GenerationScript datascript){
		//String script = SelectionScriptCreator.createSelectionScript(SelectionScriptCreator.AND, SelectionScriptCondition.anyNode());
		//SelectionScript sel = new SelectionScript(script, "JavaScript");
		int j = 0;
        logger.info("Analysing nodes...");
		for (String url: nodeurls){
			j++;
			//if (j>10) break;
	        logger.info(j + " - Node: " + url);
			Set<String> auxurls = new HashSet<String>();
			auxurls.add(url);
			List<ScriptResult<String>> results = null;
			for (int i=0;i<3;i++){
				try{
					results = rm.executeScript(datascript, TargetType.NODE_URL.toString(), auxurls);
					if (results!=null){
				        for (ScriptResult<String> res: results){
				        	logger.info(url + " : " + res.getResult());
				        }
						break;
					}
				}catch(Throwable e){
			        logger.info(" - Failed("+(i+1)+"): " + e.getMessage());
				}
			}
	        
		}
        return null;
	}
	
	public void disconnect(){
		rm.disconnect();
	}
}
