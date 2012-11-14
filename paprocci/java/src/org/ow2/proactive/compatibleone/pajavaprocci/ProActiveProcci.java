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
import org.ow2.compatibleone.exchangeobjects.ProcciCategory;
import org.ow2.compatibleone.exchangeobjects.RestRequest;
import org.ow2.compatibleone.exchangeobjects.RestResponse;
import org.ow2.proactive.compatibleone.exceptions.ElementNotFoundException;
import org.ow2.proactive.compatibleone.exchangeobjects.*;
import org.ow2.proactive.compatibleone.misc.*;
import org.ow2.proactive.compatibleone.rm.*;
import org.ow2.proactive.compatibleone.scheduler.*;
import org.ow2.proactive.scripting.*;

/**
 * This class handles the functionalities and formatting of the messages
 * that are obtained from the occi.scheduler.client.SchedulerClient. */
public class ProActiveProcci implements Procci{
	private static Logger logger =						// Logger. 
			Logger.getLogger(ProActiveProcci.class.getName()); 
	private ProcciProperties props;
	private ProActiveProcciExtras paprocciextras;
	
	public ProActiveProcci(ProcciProperties props){
		//executor =  Executors.newFixedThreadPool(1);
		this.props = props;
		paprocciextras = new ProActiveProcciExtras(props);
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
				String nodesinfo = paprocciextras.list_servers(params);
				Misc.print(nodesinfo);
			}else if (args.getBool("get-cosacs")){
				String par = args.getStr("params");
				// OS null null nonodes
				String[] params = par.split(";");
				logger.info("Parameters found:");
				for (String s: params){
					logger.info("  '" + s + "'");
				}
				//String os = args.getStr("select-by-os");
				//String hostname = null; //args.getStr("select-by-hostname");
				//String file = null; //args.getStr("select-by-file-existent");
				//String nonodes = args.getStr("number-of-nodes");
				//Object[] param = {os, null, null, nonodes};
				String nodeid = start_server(params);
				Misc.print(nodeid);
			}else if (args.getBool("get-node-info")){
				Object[] params = {args.getStr("get-node-info")};
				String nodeinfo = paprocciextras.get_server(params);
				Misc.print(nodeinfo);
			}else if (args.getBool("release-node")){
				Object[] params = {args.getStr("release-node")};
				String output = stop_server(params);
				Misc.print(output);
			}else if (args.getBool("get-node-output")){
				Object[] params = {args.getStr("get-node-output")};
				String output = paprocciextras.get_node_output(params);
				Misc.print(output);
			}else if (args.getBool("list-os")){
				Object[] params = {};
				String output = paprocciextras.list_os(params);
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
	
	private Double getvalue(String str){
		return new Double(str.substring(0, str.length()-1));
	}
	
	private String normalizem(String str){
		String output;
		
		if (str.toUpperCase().endsWith("G"))
			output = new String("" + (getvalue(str)*1024.00));
		else if (str.toUpperCase().endsWith("M"))
			output = new String("" + (getvalue(str)));
		else if (str.toUpperCase().endsWith("K"))
			output = new String("" + (getvalue(str)/1024.00));
		else
			output = new String("" + (getvalue(str)/(1024.0 * 1024.0)));
		
		logger.info("Normalized: " + str + " to: " + output + " (in Mega).");
		return output;
	}
	
	/**
	 * Execute in a ProActive node an instance of COSACS module. 
	 * @return a json object telling the result of the operation and some extra data. 
	 * @throws Exception if anything goes wrong. 
	 */
	public String start_server(
			Object[] args) throws Exception{
		
		checkparameters(args, 1);
		
		String[] argss = args[0].toString().split(";");
		
		checkparameters(argss, 5);
		
		String os = (argss[0]==null||argss[0].toString().isEmpty()?"linux":argss[0].toString());
		String rammb = normalizem((argss[1]==null||argss[1].toString().isEmpty()?"0":argss[1].toString()));
		String mhz = normalizem((argss[2]==null||argss[2].toString().isEmpty()?"0":argss[2].toString()));
		String nonodesstr = (argss[3]==null||argss[3].toString().isEmpty()?"1":argss[3].toString());
		String diskmb = normalizem((argss[4]==null||argss[4].toString().isEmpty()?"0":argss[4].toString()));
		
		logger.info("OS: " + nonodesstr);
		logger.info("RAMMB: " + rammb);
		logger.info("MHZ: " + mhz);
		logger.info("CORES: " + nonodesstr);
		logger.info("DISKMB: " + diskmb);
		
		final String path_sep;
		final String app_path;
		final String workingdir;
		final String app_args[];
		try{
			path_sep = props.getProperty("compatibleone.cosacs."+os+".path-separator");
			workingdir = props.getProperty("compatibleone.cosacs."+os+".workingdir");
			app_path = props.getProperty("compatibleone.cosacs."+os+".path");
			int app_nargs = Integer.valueOf(props.getProperty("compatibleone.cosacs."+os+".nargs"));
			app_args = new String[app_nargs];
			for(int i=0;i<app_nargs;i++){
				app_args[i] = props.getProperty("compatibleone.cosacs."+os+".arg" + i);
			}
		}catch(ElementNotFoundException e){
			throw new ElementNotFoundException("Key not defined for the Operative System required ('" + os + "')... " + e.getMessage());
		}
		
		String nodetokenstr;
		try {
			nodetokenstr = props.getProperty("compatibleone.nodes.token");
		}catch(ElementNotFoundException e){
			nodetokenstr = null;
		}
		final String nodetoken = nodetokenstr;
		
		String gridlockfile;

		try {
			gridlockfile = props.getProperty("compatibleone.cosacs.gridlockfile");
		}catch(ElementNotFoundException e){
			gridlockfile = null;
		}
		SelectionScript selection;
		if (gridlockfile != null){
			selection = SelectionScriptCreator.createSelectionScript(
					SelectionScriptCreator.AND, 
					SelectionScriptCondition.nodeWithFileMark(workingdir + path_sep + gridlockfile),
					SelectionScriptCondition.nodeWithoutSpecialLock(workingdir + path_sep + "lock"),
					SelectionScriptCondition.nodeWithOS(os)
				);
		}else{	
			selection = SelectionScriptCreator.createSelectionScript(
					SelectionScriptCreator.AND, 
					SelectionScriptCondition.nodesContainingFile(app_path),
					SelectionScriptCondition.nodeWithoutSpecialLock(workingdir + path_sep + "lock"),
					SelectionScriptCondition.nodeWithOS(os),
					SelectionScriptCondition.nodeWithDiskMb(Float.valueOf(diskmb)),
					SelectionScriptCondition.nodeWithMemoryMb(Float.valueOf(rammb)),
					SelectionScriptCondition.nodeWithMHz(Float.valueOf(mhz))
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
						workingdir,
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
	 * This method releases a locked node. 
	 * @param uuid id of the node locked. 
	 * @return a json object telling the result of the operation. 
	 */
	/*          
 			start_proactive ( 
				struct occi_category * optr,
				struct rest_client * cptr,
				struct rest_request * rptr,
                struct rest_response * aptr,
                void * vptr 
            )  
            despues de recibir la llamada esta funcion llama a C para actualizar la category o no
	*/
	public String stop_server(
			Object[] args) throws Exception{
		checkparameters(args, 1);
		final String id = args[0].toString();
		//final String id = category.getNumber();
		logger.info("Trying to release node whose related id is: " + id);
		Callable<String> callable = new Callable<String>(){
			@Override
			public String call() throws Exception {
				SchedulerClient scheduler = SchedulerClient.getInstance();
				String result = scheduler.releaseNode(id);
				logger.info("Released the node: " + id + ", result: " + result);
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
