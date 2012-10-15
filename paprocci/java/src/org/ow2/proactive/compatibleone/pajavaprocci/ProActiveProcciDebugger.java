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

import java.util.Hashtable;
import java.util.Set;
import java.util.concurrent.*;
import org.apache.log4j.Logger;
import org.ow2.proactive.compatibleone.exchangeobjects.*;
import org.ow2.proactive.compatibleone.misc.*;
import org.ow2.proactive.compatibleone.rm.*;
import org.ow2.proactive.compatibleone.scheduler.*;
import org.ow2.proactive.scripting.*;

/**
 * This class handles the functionalities and formatting of the messages
 * that are obtained from the occi.scheduler.client.SchedulerClient. */
public class ProActiveProcciDebugger{
	private static Logger logger =						// Logger. 
			Logger.getLogger(ProActiveProcciDebugger.class.getName()); 
	private ProcciProperties props;
	
	public ProActiveProcciDebugger(ProcciProperties props){
		//executor =  Executors.newFixedThreadPool(1);
		this.props = props;
	}
	
	
	/**
	 * Here command-line parameters are interpreted and an action takes place accordingly. */
	public void command(Arguments args){
		try{
			if (args.getBool("list-nodes")){
				Object[] params = {};
				String nodesinfo = list_servers(params);
				Misc.print(nodesinfo);
			}else if (args.getBool("get-node-info")){
				Object[] params = {args.getStr("get-node-info")};
				String nodeinfo = get_server(params);
				Misc.print(nodeinfo);
			}else if (args.getBool("monitor-node")){
				Object[] params = {args.getStr("monitor-node"), args.getStr("mbean"), args.getStr("attribute")};
				String output = monitor_server(params);
				Misc.print(output);
			}else if (args.getBool("get-node-output")){
				Object[] params = {args.getStr("get-node-output")};
				String output = get_node_output(params);
				Misc.print(output);
			}else if (args.getBool("list-os")){
				Object[] params = {};
				String output = list_os(params);
				Misc.print(output);
			}else{ 
				Misc.print("No command given");  
			}
			Misc.exit();  
		}catch(Throwable e){
			logger.warn("Execution error detected: " + e.getMessage());
			e.printStackTrace();
			Misc.forcedExit();
		}
	}
	
	/**
	 * Check if the given amount of parameters passed through JNI is correct.
	 * @param args arguments passed to a method through JNI.
	 * @param amount amount of arguments.
	 * @throws Exception if not correct.
	 */
	private void checkparameters(Object[] args, int amount) throws Exception{
		if (args.length != amount){
			throw new Exception("The amount of parameters passed is " + args.length + " but " + amount + " are required.");
		}
	}
	
	/**
	 * Create an executor (thread pool). 
	 * @return executor.
	 */
	private ExecutorService createExecutor(){
		return Executors.newSingleThreadExecutor();
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
	 * Get public information regarding a node. 
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
				NodePublicInfo node = rm.getCompleteNodePublicInfo(nodeid, job2node);
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
	 * Get the output of a task running at a particular node.
	 */
	public String get_node_output(Object[] args) throws Exception{
		checkparameters(args, 1);
		final String jobid = args[0].toString();
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
	
	public String list_os(Object[] args) throws Exception{
		checkparameters(args, 0);
		logger.info("Trying list Operating Systems...");
		Callable<String> callable = new Callable<String>(){
			@Override
			public String call() throws Exception {
				ResourceManagerClient rm = ResourceManagerClient.getInstance(); 
				Set<String> nodeurls = rm.getNodesUrl(false);
				GenerationScript datascript = DataScriptCreator.createScript(DataScriptAddition.getOS());
				//GenerationScript datascript = DataScriptCreator.createScript(DataScriptAddition.getCPUUser());
				rm.runScript(
						nodeurls, 
						datascript);
				rm.disconnect();
				return "";
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
