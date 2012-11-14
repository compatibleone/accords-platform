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

package org.ow2.proactive.compatibleone.rm;

import java.security.KeyException;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.List;
import java.util.Set;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import javax.security.auth.login.LoginException;
import org.apache.log4j.Logger;
import org.objectweb.proactive.core.node.NodeException;
import org.objectweb.proactive.core.node.NodeFactory;
import org.ow2.proactive.authentication.crypto.CredData;
import org.ow2.proactive.authentication.crypto.Credentials;
import org.ow2.proactive.compatibleone.exceptions.NodeInfoException;
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
	public NodePublicInfo getCompleteNodePublicInfo(String node, Hashtable<NodeId, String> node2job) throws Exception{
        List<RMNodeEvent> listne = rm.getMonitoring().getState().getNodesEvents();
        if (listne.size() == 0) {
            logger.info("No nodes handled by Resource Manager");
        } else { 
            for (RMNodeEvent evt : listne) {
            	
	            logger.debug(" - Comparing '" + evt.getNodeUrl() + "' with target '" + node + "'...");
            	if(new NodeId(evt.getNodeUrl()).equals(new NodeId(node))){
		            logger.info("   - Found!");
                	NodePublicInfo h = getBasicNodePublicInfo(evt);
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
	 * @return the NodePublicInfo object. 
	 * @throws Exception */
	private NodePublicInfo getBasicNodePublicInfo(RMNodeEvent evt) throws NodeInfoException{
		CONodeState state = new CONodeState(evt.getNodeState()); 
		String ipaddress = null;
		try{
			 ipaddress = NodeFactory.getNode(evt.getNodeUrl()).getVMInformation().getInetAddress().getHostAddress();
		}catch(NodeException e){
			logger.warn("Error getting IP of node " + evt.getNodeUrl() + " ", e);
			throw new NodeInfoException("Error getting IP of node '" + evt.getNodeUrl() + "'.");
		}
    	NodePublicInfo h = new NodePublicInfo(
    			evt.getNodeUrl(), 
    			evt.getNodeUrl(), 
    			state, 
    			evt.getHostName(), 
    			evt.getNodeOwner(),
    			ipaddress);
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
                	NodePublicInfo h = getBasicNodePublicInfo(evt);
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
