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
import java.util.Hashtable;
import java.util.Set;
import java.util.concurrent.*;
import org.apache.log4j.Logger;
import org.ow2.compatibleone.Procci;
import org.ow2.proactive.compatibleone.exceptions.ElementNotFoundException;
import org.ow2.proactive.compatibleone.exchangeobjects.*;
import org.ow2.proactive.compatibleone.misc.*;
import org.ow2.proactive.compatibleone.rm.*;
import org.ow2.proactive.compatibleone.scheduler.*;
import org.ow2.proactive.scripting.*;

/**
 * This class handles the functionalities and formatting of the messages
 * that are obtained from the occi.scheduler.client.SchedulerClient. */
public class ProActiveProcci extends Procci{
	private static Logger logger =						// Logger. 
			Logger.getLogger(ProActiveProcci.class.getName()); 
	private ProcciProperties props;
	
	public ProActiveProcci(ProcciProperties props){
		//executor =  Executors.newFixedThreadPool(1);
		this.props = props;
	}
	
	/**
	 * Here command-line parameters are interpreted and an action takes place accordingly. 
	 * @throws Exception 
	 * @throws ElementNotFoundException */
	public void initialize(final String user, final String pass) throws ElementNotFoundException{
		logger.info("Trying to initialize all (user="+user+" pass="+pass+")...");
		ResourceManagerClient.setUpInitParameters(props.getProperty("rm.url"), user, pass);
		SchedulerClient.setUpInitiParameters( props.getProperty("scheduler.url"), user, pass);
		
	}
	/**
	 * Here command-line parameters are interpreted and an action takes place accordingly. */
	public void command(Arguments args){
		try{
			if (args.getBool("list-nodes")){
				Object[] params = {};
				String nodesinfo = list_servers(params);
				Misc.print(nodesinfo);
			}else if (args.getBool("get-cosacs")){
				String os = args.getStr("select-by-os");
				String hostname = args.getStr("select-by-hostname");
				String file = args.getStr("select-by-file-existent");
				String nonodes = args.getStr("number-of-nodes");
				Object[] params = {os, hostname, file, nonodes};
				String nodeid = start_server(params);
				Misc.print(nodeid);
			}else if (args.getBool("get-node-info")){
				Object[] params = {args.getStr("get-node-info")};
				String nodeinfo = get_server(params);
				Misc.print(nodeinfo);
			}else if (args.getBool("release-node")){
				Object[] params = {args.getStr("release-node")};
				String output = stop_server(params);
				Misc.print(output);
			}else if (args.getBool("monitor-node")){
				Object[] params = {args.getStr("monitor-node"), args.getStr("mbean"), args.getStr("attribute")};
				String output = monitor_server(params);
				Misc.print(output);
			}else if (args.getBool("get-node-output")){
				String output = getNodeOutput(args.getStr("get-node-output"));
				Misc.print(output);
			}else if (args.getBool("list-os")){
				String output = listOperativeSystems();
				Misc.print(output);
			}else{ 
				Misc.print("No command given");  
			}
			Misc.exit();  
			//Thread.sleep(18000);
			//command(args);
		}catch(Throwable e){
			logger.warn("Execution error detected: " + e.getMessage());
			e.printStackTrace();
			Misc.forcedExit();
		}
	}
	
	private void checkparameters(Object[] args, int amount) throws Exception{
		if (args.length != amount){
			throw new Exception("The amount of parameters passed is " + args.length + " but " + amount + " are required.");
		}
	}
	
	private ExecutorService createExecutor(){
		return Executors.newSingleThreadExecutor();
	}
	
	public String listOperativeSystems() throws Exception{
		ResourceManagerClient rm = ResourceManagerClient.getInstance(); 
		Set<String> nodeurls = rm.getNodesUrl(false);
		GenerationScript datascript = DataScriptCreator.createScript(DataScriptAddition.getOS());
		//GenerationScript datascript = DataScriptCreator.createScript(DataScriptAddition.getCPUUser());
		rm.runScript(
				nodeurls, 
				datascript);
		rm.disconnect();
		return "{}";
	}
	
	/**
	 * Execute in a ProActive node an instance of COSACS module. 
	 * @return a json object telling the result of the operation and some extra data. 
	 * @throws Exception if anything goes wrong. 
	 */
	public String start_server(Object[] args) throws Exception{
		checkparameters(args, 4);
		String os = (args[0]==null?null:args[0].toString());
		String hostname = (args[1]==null?null:args[1].toString());
		String file = (args[2]==null?null:args[2].toString());
		String nonodesstr = (args[3]==null?null:args[3].toString());
		
		if (os==null){
			os = "linux";
			logger.warn("No Operative System specified, using as default: " + os);
		}
		
		final String path_sep;
		final String app_path;
		final String app_args;
		
		try{
			path_sep = props.getProperty("compatibleone.cosacs."+os+".path-separator");
			app_path = props.getProperty("compatibleone.cosacs."+os+".path");
			app_args = props.getProperty("compatibleone.cosacs."+os+".args");
		}catch(ElementNotFoundException e){
			throw new ElementNotFoundException("Key not defined for the Operative System required ('" + os + "')... " + e.getMessage());
		}
		
		final String nodetoken = props.getProperty("compatibleone.nodes.token");
		
		SelectionScript selection = null;
		
		if (hostname!=null){
			selection = SelectionScriptCreator.createSelectionScript(
					SelectionScriptCreator.AND, 
					SelectionScriptCondition.nodeByHostname(hostname));
		}else if(file!=null){
			selection = SelectionScriptCreator.createSelectionScript(
					SelectionScriptCreator.AND, 
					SelectionScriptCondition.nodesContainingFile(file));
		}else{
			String app_dir = (new File(app_path)).getParent();
			selection = SelectionScriptCreator.createSelectionScript(
						SelectionScriptCreator.AND, 
						SelectionScriptCondition.nodeWithFileMark(app_dir + path_sep + "cosacs-ready"),
						SelectionScriptCondition.nodeWithoutSpecialLock(app_dir + path_sep + "lock"),
						SelectionScriptCondition.nodeWithOS(os)
					);
		}
		
		Integer nonodes;
		try{
			nonodes = Integer.parseInt(nonodesstr);
		}catch(Exception e){
			nonodes = null;
			logger.warn("Error parsing number of nodes: '" + nonodesstr + "'... Skipping parameter.");
		}
		
		final Integer nonodesf = nonodes;
		final SelectionScript selectionscript = selection;
		final Signal stopsignal = new Signal(false);
		Callable<String> callable = new Callable<String>(){
			@Override
			public String call() throws Exception {
		
				SchedulerClient scheduler = SchedulerClient.getInstance();
				String node = scheduler.acquireApplicationInNode(
						stopsignal,
						selectionscript, 
						app_path, 
						app_args, 
						nodetoken, 
						nonodesf); // Get a json formatted object with all the information.
				logger.info("Obtained the remote with info: " + node);
				scheduler.disconnect();
				return node;
			}
		};
		
		ExecutorService executor = createExecutor();//Executors.newSingleThreadExecutor();
		Future<String> future = executor.submit(callable); // We ask to execute the callable.
		String res = null;
		try{
			res = future.get(new Integer(props.getProperty("scheduler.timeout")), TimeUnit.SECONDS);
			return res;
		}catch(Exception e){
			stopsignal.setValue(true);
			logger.warn("Error: ", e);
			return (new ErrorObject(e).toString());
		}
	}
	
	
	
	/**
	 * Monitors a node.
	 * @return a json object with a result of the monitoring operation. 
	 * @throws Exception in case something goes wrong. 
	 */
	public String monitor_server(Object[] args) throws Exception{
		checkparameters(args, 3);
		final String nodeurl = args[0].toString();
		final String mbean = args[1].toString();
		final String attribute = args[2].toString();
		
		logger.info("Trying to monitor a node...");
		Callable<String> callable = new Callable<String>(){
			@Override
			public String call() throws Exception {
				ResourceManagerClient rm = ResourceManagerClient.getInstance(); 
				String result = null;
				if (mbean == null){
					logger.info("Interpreting attribute parameter as a monitoring property...");
					result = rm.monitorNode(nodeurl, attribute).toString();
				}else{
					logger.info("Using explicitly told mbean and attribute...");
					result = rm.monitorNode(nodeurl, mbean, attribute).toString();
				}
				rm.disconnect();
				return result;
			}
		};
		
		ExecutorService executor = createExecutor();//Executors.newSingleThreadExecutor();
		Future<String> future = executor.submit(callable); // We ask to execute the callable.
		String res = null;
		try{
			res = future.get(new Integer(props.getProperty("scheduler.timeout")), TimeUnit.SECONDS);
			return res;
		}catch(Exception e){
			logger.warn("Error: ", e);
			return (new ErrorObject(e).toString());
		}
	}
	
	/**
	 * Listing of the nodes available in the RM. 
	 * @return a json object with a list of nodes and some info about them. 
	 * @throws Exception in case something goes wrong. 
	 */
	public String list_servers(Object[] args) throws Exception{
		checkparameters(args, 0);
		logger.info("Trying to list the nodes...");
		Callable<String> callable = new Callable<String>(){
			@Override
			public String call() throws Exception {
				SchedulerClient scheduler = SchedulerClient.getInstance();
				Hashtable<NodeId, String> job2node = scheduler.mapNodesWithJobs();
				ResourceManagerClient rm = ResourceManagerClient.getInstance();
				NodePublicInfoList nodes = rm.getAllTheNodesInfo(job2node);
				String result = nodes.toString();
				scheduler.disconnect();
				rm.disconnect();
				return result;
			}
		};
		
		ExecutorService executor = createExecutor();//Executors.newSingleThreadExecutor();
		Future<String> future = executor.submit(callable); // We ask to execute the callable.
		String res = null;
		try{
			res = future.get(new Integer(props.getProperty("scheduler.timeout")), TimeUnit.SECONDS);
			return res;
		}catch(Exception e){
			logger.warn("Error: ", e);
			return (new ErrorObject(e).toString());
		}
	}

	
	/**
	 * Get the output of a task running at a particular node.
	 */
	public String getNodeOutput(final String jobid){
		logger.info("Trying to get public information of '" + jobid + "...");
		Callable<String> callable = new Callable<String>(){
			@Override
			public String call() throws Exception {
				SchedulerClient scheduler = SchedulerClient.getInstance();
				String result = scheduler.acquireOutput(jobid);
				scheduler.disconnect();
				return result;
			}
		};
		
		ExecutorService executor = createExecutor();//Executors.newSingleThreadExecutor();
		Future<String> future = executor.submit(callable); // We ask to execute the callable.
		String res = null;
		try{
			res = future.get(new Integer(props.getProperty("scheduler.timeout")), TimeUnit.SECONDS);
			return res;
		}catch(Exception e){
			logger.warn("Error: ", e);
			return (new ErrorObject(e).toString());
		}
	}

	
	/**
	 * Get public information regarding a node. 
	 *  
	 *  
	 */
	public String get_server(Object[] args) throws Exception{
		checkparameters(args, 1);
		final String nodeid = args[0].toString();
		logger.info("Trying to get public information of '" + nodeid + "...");
		Callable<String> callable = new Callable<String>(){
			@Override
			public String call() throws Exception {
				SchedulerClient scheduler = SchedulerClient.getInstance();
				Hashtable<NodeId, String> job2node = scheduler.mapNodesWithJobs();
				ResourceManagerClient rm = ResourceManagerClient.getInstance();
				NodePublicInfo node = rm.getNodePublicInfo(nodeid, job2node);
				String result = node.toString();
				scheduler.disconnect();
				rm.disconnect();
				return result;
			}
		};
		
		ExecutorService executor = createExecutor();//Executors.newSingleThreadExecutor();
		Future<String> future = executor.submit(callable); // We ask to execute the callable.
		String res = null;
		try{
			res = future.get(new Integer(props.getProperty("scheduler.timeout")), TimeUnit.SECONDS);
			return res;
		}catch(Exception e){
			logger.warn("Error: ", e);
			return (new ErrorObject(e).toString());
		}
	}

	
	/**
	 * This method releases a locked node. 
	 * @param uuid uuid of the node locked. 
	 * @return a json object telling the result of the operation. 
	 */
	public String stop_server(Object[] args) throws Exception{
		checkparameters(args, 1);
		final String uuid = args[0].toString();
		logger.info("Trying to release node whose related id is: " + uuid);
		Callable<String> callable = new Callable<String>(){
			@Override
			public String call() throws Exception {
				SchedulerClient scheduler = SchedulerClient.getInstance();
				String result = scheduler.releaseNode(uuid);
				logger.info("Released the node: " + uuid + ", result: " + result);
				scheduler.disconnect();
				return result;
			}
		};
		
		ExecutorService executor = createExecutor();//Executors.newSingleThreadExecutor();
		Future<String> future = executor.submit(callable); // We ask to execute the callable.
		String res = null;
		try{
			res = future.get(new Integer(props.getProperty("scheduler.timeout")), TimeUnit.SECONDS);
			return res;
		}catch(Exception e){
			logger.warn("Error: ", e);
			return (new ErrorObject(e).toString());
		}
	}
	
}
