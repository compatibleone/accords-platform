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

package org.ow2.proactive.compatibleone.scheduler;

import java.net.InetAddress;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Vector;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import org.apache.log4j.AppenderSkeleton;
import org.apache.log4j.Logger;
import org.apache.log4j.spi.LoggingEvent;
import org.ow2.proactive.compatibleone.exchangeobjects.NodeInfo;
import org.ow2.proactive.compatibleone.exchangeobjects.NodePublicInfo;
import org.ow2.proactive.compatibleone.misc.CONodeState;
import org.ow2.proactive.compatibleone.misc.Misc;
import org.ow2.proactive.compatibleone.misc.Signal;
import org.ow2.proactive.compatibleone.rm.NodeId;
import org.ow2.proactive.compatibleone.rm.ResourceManagerClient;
import org.ow2.proactive.scheduler.common.Scheduler;
import org.ow2.proactive.scheduler.common.SchedulerAuthenticationInterface;
import org.ow2.proactive.scheduler.common.SchedulerConnection;
import org.ow2.proactive.scheduler.common.job.JobId;
import org.ow2.proactive.scheduler.common.job.JobPriority;
import org.ow2.proactive.scheduler.common.job.JobResult;
import org.ow2.proactive.scheduler.common.job.JobState;
import org.ow2.proactive.scheduler.common.job.JobStatus;
import org.ow2.proactive.scheduler.common.job.TaskFlowJob;
import org.ow2.proactive.scheduler.common.task.Log4JTaskLogs;
import org.ow2.proactive.scheduler.common.task.NativeTask;
import org.ow2.proactive.scheduler.common.task.ParallelEnvironment;
import org.ow2.proactive.scheduler.common.task.TaskState;
import org.ow2.proactive.scheduler.common.task.TaskStatus;
import org.ow2.proactive.scheduler.common.util.logforwarder.LogForwardingService;
import org.ow2.proactive.scheduler.job.JobIdImpl;
import org.ow2.proactive.scripting.SelectionScript;
import org.ow2.proactive.topology.descriptor.TopologyDescriptor;
import org.ow2.proactive.authentication.crypto.CredData;
import org.ow2.proactive.authentication.crypto.Credentials;

public class SchedulerClient {
	public static final String JOBNAME = "COApplication";
	
	private static String schedulerurl_backup;
	private static String username_backup;
	private static String password_backup;
	
	protected static Logger logger  =						// Logger. 
			Logger.getLogger(SchedulerClient.class.getName()); 

	private String pendingJobId;		// Tells if after any operation there is a pending job (useful when 
										// there is a timeout, and one job must be killed).

	private Scheduler scheduler;
	
	public static void setUpInitiParameters(
			String schedulerurl, 
			String username,
			String password){
		schedulerurl_backup = schedulerurl;
		username_backup = username;
		password_backup = password;
	}
	
	public static SchedulerClient getInstance() throws Exception{
        logger.info("Initializing instance of Scheduler...");
		return new SchedulerClient(schedulerurl_backup, username_backup, password_backup);
	}
	
	/**
	 * Constructor.
	 * @param schedulerurl
	 * @param rmurl
	 * @param username
	 * @param password 
	 * @throws Exception */
	private SchedulerClient(
			String schedulerurl, 
			String username,
			String password) throws Exception{
		this.pendingJobId = null;
		
		//logger.info("HARDCODED");
		//int a = 3; if (a>2){return;}
		
		scheduler = initializeSchedulerProxy(schedulerurl, username, password);
	}
	
	/** 
	 * Get the proxy of the Scheduler. */
	private Scheduler initializeSchedulerProxy(String schedulerurl, String username, String password) throws Exception {
		logger.info("Joining the scheduler at '" + schedulerurl + "'...");
        SchedulerAuthenticationInterface auth = SchedulerConnection.join(schedulerurl);
        logger.info("Done.");
        logger.info("Logging in...");
        Credentials cred = Credentials.createCredentials(
        		new CredData(username, password), auth.getPublicKey());
        Scheduler schedulerStub = auth.login(cred);
        logger.info("Done.");
        return schedulerStub;
	}

	/**
	 * Method to create a job with one task, which runs a specific application (i.e. COSACS of 
	 * CompatibleOne) and later returns the information of that node.
	 * @param stopsignal signal to listen to to know if the job submission attempt is no longer needed.
	 * @param nodeselectioncriteria criteria to select the node (it must be a Selection Script of ProActive).
	 * @param applicationpath path of the application to be run in the node. 
	 * @param applicationargs arguments to pass to the application. 
	 * @param appworkingdir working directory for the application. 
	 * @param nodetoken special token that allow the usage of some privileged nodes. 
	 * @param nonodes number of nodes needed by this application. 
	 * @return information about the node in which the application runs. 
	 * @throws Exception if something goes wrong. */
	public String acquireApplicationInNode(
			Signal stopsignal,
			SelectionScript nodeselectioncriteria, 
			String applicationpath,
			String applicationargs[], 
			String appworkingdir, 
			String nodetoken,
			Integer nonodes) throws Exception {
		
		//logger.info("HARDCODED");
		//int a = 3; if (a>2){ return "{\"id\":\"12385\",\"adminPass\":\"passWord\",\"status\":\"RUNNING\",\"hostname\":\"cper-PC\",\"privateip\":\"138.96.126.112\",\"publicip\":\"138.96.126.112\",\"nodeurl\":\"pamr://6131/xps1\",\"owner\":\"mjost\"}"; }
		
		
		logger.info("Creating job and tasks...");
		NativeTask primaryTask = new NativeTask();
		logger.info("Using application path:  '"+applicationpath+"'...");
		logger.info("Using application args:  '"+applicationargs+"'...");
		for (String arg:applicationargs){
			logger.info(" - '"+arg+"'");
		}
		
		logger.info("Using app. working dir:  '"+appworkingdir+"'...");
		
		primaryTask.setWorkingDir(appworkingdir);
		primaryTask.setCommandLine(Misc.getStringArray(applicationpath, applicationargs));
		primaryTask.setName(applicationpath);
		
		logger.info("Using selection script: '"+nodeselectioncriteria.getScript()+"'...");
		primaryTask.addSelectionScript(nodeselectioncriteria);
		
		if (nonodes != null && nonodes > 1){
			logger.info("Using topology made of " + nonodes + " nodes...");
			TopologyDescriptor topologyDescriptor = TopologyDescriptor.SINGLE_HOST;
			ParallelEnvironment parallelEnvironment = new ParallelEnvironment(nonodes, topologyDescriptor);
			primaryTask.setParallelEnvironment(parallelEnvironment);
		}else{
			logger.info("Only one node required, no topology needed.");
		}
		
		if (nodetoken != null){
			logger.info("Using node token: '"+nodetoken+"'...");
			primaryTask.addGenericInformation("NS_AUTH", nodetoken);
		}
		
		TaskFlowJob taskFlowJob = new TaskFlowJob();
		taskFlowJob.setName(JOBNAME);
		taskFlowJob.setPriority(JobPriority.NORMAL);
		taskFlowJob.addTask(primaryTask);

		logger.info("Submitting job...");
		JobId jobId = scheduler.submit(taskFlowJob);
		pendingJobId = jobId.toString();
		logger.info("Submitted the job (JobId " + jobId + ")");
	
		JobState jobstate = null;
		while(true){
			jobstate = scheduler.getJobState(jobId);
			if (jobstate.getStatus().equals(JobStatus.PENDING)){ // It is still pending... We need to wait...
				logger.info("Waiting for the pending job...");
				if (stopsignal.getValue()==true){
					logger.info("Waited too much for the job to execute, trying to remove it...");
					removeJobAlone(jobId);
					logger.info("Removed the job " + jobId);
					throw new Exception("Obliged to stop, removed the job " + jobId + ". ");
				}
				Thread.sleep(3000);
			}else{ 	// Not pending anymore, so it went through execution but it's not sure that it's running, so
					// the correct execution must be checked. 
				if (!jobstate.getStatus().equals(JobStatus.RUNNING)){ // Not running? There is a problem then...
					JobResult jr = scheduler.getJobResult(jobId);
					logger.warn("The job is not running (it is " + jobstate.getStatus().name() + "), so something went wrong.");
					String output = 
							"Stdout: '" + jr.getResult(applicationpath).getOutput().getStdoutLogs(false) + "'." + 
							"Stderr: '" + jr.getResult(applicationpath).getOutput().getStderrLogs(false) + "'.";
					logger.warn(output);
					throw new Exception("The job is not running as expected (status=" + jobstate.getStatus().name() + "): " + output);
				}
				logger.info("Job not pending anymore!!!");
				break;
			}
		}
		
		// Getting information about the tasks/application and its host node. 
		ArrayList<TaskState> tasks = jobstate.getTasks();
		int taskssize = tasks.size();
		if (taskssize != 1){
			logger.warn("The amount of tasks is supposed to be 1, but it is: " + taskssize);
			throw new Exception("Incorrect amount of tasks: " + taskssize);
		}
		// There is only one task for the submitted job. 
		String runninghostname = tasks.get(0).getExecutionHostName();
		
		logger.info("Execution host name: " + runninghostname);
		String runningnode = runninghostname.substring(runninghostname.indexOf("(")).replace("(", "").replace(")","");
		logger.info("Execution node: " + runningnode);
		TaskStatus status = tasks.get(0).getStatus();
		logger.info("Execution status: " + status);

		ResourceManagerClient rm = ResourceManagerClient.getInstance();
		NodePublicInfo nodePublicInfo = rm.getCompleteNodePublicInfo(runningnode, null);
		rm.disconnect();
		
		logger.info("IP address detected: " + nodePublicInfo.getIpAddress());
		
		// Return a json formatted object with all information needed. 
		NodeInfo ret = new NodeInfo(
				jobId.toString(), 
				"passWord", 
				new CONodeState(status), 
				nodePublicInfo.getHostname(), 
				nodePublicInfo.getIpAddress(), 
				nodePublicInfo.getIpAddress(), 
				nodePublicInfo.getNodeURL(), 
				nodePublicInfo.getOwner());
				
		if (stopsignal.getValue()==true){
			logger.info("Job executed but too late...");
			removeJobAlone(jobId);
			logger.info("Removed the job " + jobId);
			throw new Exception("Job " + jobId + " executed but too late...");
		}
		return ret.toString();
	}

	
	/**
	 * Best effort to remove a job.
	 * @param jobId of the job to remove.
	 * @throws Exception if something goes wrong.
	 */
	public void removeJobAlone(final JobId jobId) throws Exception{
		logger.info("Trying to FINAL release node whose related id is: " + jobId);
		Callable<String> callable = new Callable<String>(){
			@Override
			public String call() throws Exception {
				SchedulerClient scheduler = SchedulerClient.getInstance();
				String result = scheduler.releaseNode(jobId.toString());
				logger.info("Released the node: " + jobId + ", result: " + result);
				scheduler.disconnect();
				return result;
			}
		};
		
		ExecutorService executor = Executors.newSingleThreadExecutor();
		Future<String> future = executor.submit(callable); // We ask to execute the callable.
		String res = null;
		try{
			res = future.get(Integer.MAX_VALUE, TimeUnit.SECONDS);
		}catch(Exception e){
			logger.error("Error: ", e);
		}
	}
	
	/**
	 * Maps a given node with a running job. 
	 * @return a hashtable containing the mapping.
	 * @throws Exception if something fails. */
	public Hashtable<NodeId, String> mapNodesWithJobs() throws Exception {
		Hashtable<NodeId, String> ret = new Hashtable<NodeId, String>();
		logger.info("Mapping nodes with jobs...");
		Vector<JobState> jobs = scheduler.getState().getRunningJobs();
		for(JobState job: jobs){
			if (job.getName().equals(JOBNAME)){
				ArrayList<TaskState> tasks = job.getTasks();
				int taskssize = tasks.size();
				if (taskssize != 1){
					logger.warn("The amount of tasks is supposed to be 1, but it is: " + taskssize);
					throw new Exception("Incorrect amount of tasks: " + taskssize);
				}
				// There is only one task for the submitted job. 
				String runninghostname = tasks.get(0).getExecutionHostName();
				String runningnode = runninghostname.substring(runninghostname.indexOf("(")).replace("(", "").replace(")","");
				logger.info(" - Node: " + runningnode + " mapped with job " + job.getId().toString());
				ret.put(new NodeId(runningnode), job.getId().toString());
			}
		}
		return ret;
	}

	/**
	 * Method to get a snapshot of the output of a given job.
	 * The output used is stdout.
	 * @param jobId job to observe. 
	 * @return the result of the operation. 
	 * @throws Exception if something fails. */
	public String acquireOutput(String jobId) throws Exception {
		LogForwardingService lfsPA = new LogForwardingService(
            "org.ow2.proactive.scheduler.common.util.logforwarder.providers.ProActiveBasedForwardingProvider");
        lfsPA.initialize();

        Logger l1 = Logger.getLogger(Log4JTaskLogs.JOB_LOGGER_PREFIX + jobId);

        l1.setAdditivity(false);
        l1.removeAllAppenders();

        AppenderTester test1 = new AppenderTester();
        l1.addAppender(test1);

        scheduler.listenJobLogs(jobId, lfsPA.getAppenderProvider());

		while (true) {
			
			try {
				logger.info("Waiting... ");
				Thread.sleep(20000);
			} catch (InterruptedException e) {
				e.printStackTrace();
				break;
			}
		}
		return "{}";
	}
	
	public class AppenderTester extends AppenderSkeleton {
		protected Logger logger  =						// Logger. 
			Logger.getLogger(AppenderTester.class.getName()); 
        @Override
        protected void append(LoggingEvent loggingevent) {
            logger.info(">> " + loggingevent.getMessage());
        }
        @Override
        public void close() {
            super.closed = true;
        }
        @Override
        public boolean requiresLayout() {
            return false;
        }
    }
	
	/**
	 * Release the node (by killing the application/task that it runs).
	 * @param uuid of the node/task/application/job.
	 * @return the result of the operation.
	 * @throws Exception if something fails. */
	public String releaseNode(String uuid) throws Exception {
		logger.info("Getting scheduler proxy...");
		JobId jobId = JobIdImpl.makeJobId(uuid);
		this.killJob(jobId);
		return "{}";
	}

	/** 
	 * Kill the job from the Scheduler. 
	 * @param jobId the ID of the job. 
	 * @exception Exception if something fails. */
	private void killJob(JobId jobId) throws Exception{
		boolean killed = scheduler.killJob(jobId);
		if (killed == false){
			logger.warn("Error while trying to kill job " + jobId + " (returned=false).");
			throw new Exception("Can't kill job " + jobId + ".");
		}else{
			logger.info("Job " + jobId + " killed successfully.");
		}
	}
	
	/** 
	 * Kill the pending job from the Scheduler. 
	 * @exception Exception if something fails. */
	public void releasePotentialNode(){
		if (this.pendingJobId != null){
			logger.warn("There is a pending job (" + pendingJobId + "). Trying to kill it...");
			try{
				this.releaseNode(pendingJobId);
			}catch(Exception r){
				logger.warn("Error while killing the job " + pendingJobId + ": " + r.getMessage());
			}
			this.pendingJobId = null;
		}
	}
	
	/** 
	 * Disconnect this scheduler stub.
	 **/
	public void disconnect(){
		try{
			scheduler.disconnect();
		}catch(Exception e){
			logger.warn("Error while trying to disconnect...", e);
		}
	}
}
