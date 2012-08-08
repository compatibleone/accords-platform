/* * ################################################################
 *
 * ProActive Parallel Suite(TM): The Java(TM) library for
 *    Parallel, Distributed, Multi-Core Computing for
 *    Enterprise Grids & Clouds
 *
 * Copyright (C) 1997-2011 INRIA/University of
 *                 Nice-Sophia Antipolis/ActiveEon
 * Contact: proactive@ow2.org or contact@activeeon.com
 * modify it under the terms of the GNU Affero General Public License
 * the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 * USA
 *
 * If needed, contact us to obtain a release under GPL Version 2 or 3
 * or a different license than the AGPL.
 *  Initial developer(s):               The ActiveEon Team
 *                        http://www.activeeon.com/
 *  Contributor(s):
 *
 * ################################################################
 * $$ACTIVEEON_INITIAL_DEV$$
 */
package org.ow2.proactive.compatibleone.scheduler;

import java.io.File;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Vector;
import org.apache.log4j.AppenderSkeleton;
import org.apache.log4j.Logger;
import org.apache.log4j.spi.LoggingEvent;
import org.ow2.proactive.compatibleone.exchangeobjects.NodeInfo;
import org.ow2.proactive.compatibleone.exchangeobjects.NodePublicInfo;
import org.ow2.proactive.compatibleone.misc.CONodeState;
import org.ow2.proactive.compatibleone.misc.Misc;
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
import org.ow2.proactive.scheduler.common.task.TaskState;
import org.ow2.proactive.scheduler.common.task.TaskStatus;
import org.ow2.proactive.scheduler.common.util.logforwarder.LogForwardingService;
import org.ow2.proactive.scheduler.job.JobIdImpl;
import org.ow2.proactive.scripting.SelectionScript;
import org.ow2.proactive.authentication.crypto.CredData;
import org.ow2.proactive.authentication.crypto.Credentials;

public class SchedulerClient {
	public static final String JOBNAME = "COApplication";
	private static SchedulerClient instance;
	
	protected static Logger logger  =						// Logger. 
			Logger.getLogger(SchedulerClient.class.getName()); 

	private String pendingJobId;		// Tells if after any operation there is a pending job (useful when 
										// there is a timeout, and one job must be killed).

	private Scheduler scheduler;
	
	public static SchedulerClient initializeInstance(
			String schedulerurl, 
			String username,
			String password) throws Exception{
		if (instance != null){
	        logger.error("Trying to re-initialize the SchedulerClient.");
		}
		instance = new SchedulerClient(schedulerurl, username, password);
		return instance;
	}
	
	public static SchedulerClient getInstance(){
		if (instance == null){
	        logger.error("Trying to access the SchedulerClient without initializing.");
		}
		localCheck(instance);
		return instance;
	}
	
	private static void localCheck(SchedulerClient client){
        logger.info("About to use the SchedulerClient...");
        boolean bool = client.scheduler.isConnected();
        logger.info("SchedulerClient.isConnected()=" + bool);
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
	 * @param nodeselectioncriteria criteria to select the node (it must be a Selection Script of ProActive)
	 * @param applicationpath path of the application to be run in the node. 
	 * @param applicationargs arguments to pass to the application. 
	 * @param nodetoken special token that allow the usage of some privileged nodes. 
	 * @return information about the node in which the application runs. 
	 * @throws Exception if something goes wrong. */
	public String acquireApplicationInNode(
			SelectionScript nodeselectioncriteria, 
			String applicationpath,
			String applicationargs, 
			String nodetoken) throws Exception {
		
		logger.info("Creating job and tasks...");
		NativeTask primaryTask = new NativeTask();
		logger.info("Using application path:  '"+applicationpath+"'...");
		logger.info("Using application args:  '"+applicationargs+"'...");
		String workingdir = (new File(applicationpath)).getParent();
		logger.info("Using working directory: '"+workingdir+"'...");
		primaryTask.setWorkingDir(workingdir);
		primaryTask.setCommandLine(Misc.getStringArray(applicationpath, applicationargs));
		primaryTask.setName(applicationpath);
		logger.info("Using selection script: '"+nodeselectioncriteria.getScript()+"'...");
		primaryTask.addSelectionScript(nodeselectioncriteria);
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
				Thread.sleep(2000);
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
		NodePublicInfo nodePublicInfo = rm.getNodePublicInfo(runningnode, null);
		
		String hostname = nodePublicInfo.getHostname(); 
		String address = null;
		try{
			address = InetAddress.getByName(hostname).getHostAddress();
		}catch(Exception e){
			try{
				address = InetAddress.getByName(runningnode).getHostAddress();
			}catch(Exception ee){
				
				logger.warn("Could not find out the IP address of host with hostname: '" + hostname + "' or '" + runningnode + "'.");
				throw new Exception("Cannot resolve IP address for: '" + hostname + "' or '" + runningnode + "'");
			}
		}
		
		// Return a json formatted object with all information needed. 
		NodeInfo ret = new NodeInfo(
				jobId.toString(), 
				"passWord", 
				new CONodeState(status), 
				hostname, 
				address, 
				address, 
				nodePublicInfo.getNodeURL(), 
				nodePublicInfo.getOwner());
		return ret.toString();
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
				releaseNode(pendingJobId);
			}catch(Exception r){
				logger.warn("Error while killing the job " + pendingJobId + ": " + r.getMessage());
			}
			this.pendingJobId = null;
		}
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
}
