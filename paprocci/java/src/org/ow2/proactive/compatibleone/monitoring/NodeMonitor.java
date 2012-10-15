package org.ow2.proactive.compatibleone.monitoring;

import java.util.ArrayList;

import javax.security.auth.login.LoginException;

import org.apache.log4j.Logger;
import org.ow2.proactive.authentication.crypto.Credentials;
import org.ow2.proactive.compatibleone.exceptions.ElementNotFoundException;
import org.ow2.proactive.resourcemanager.authentication.RMAuthentication;
import org.ow2.proactive.resourcemanager.common.event.RMNodeEvent;
import org.ow2.proactive.resourcemanager.common.util.RMCachingProxyUserInterface;
import org.ow2.proactive.resourcemanager.frontend.ResourceManager;

public class NodeMonitor {
    
	private static Logger logger =
			Logger.getLogger(NodeMonitor.class.getName()); 	// Logger.
    
	private RMAuthentication auth;
	private Credentials credentials;
	
    public NodeMonitor(RMAuthentication auth, Credentials credentials){
    	this.auth = auth;
    	this.credentials = credentials;
    }

    private String getJMXNodeURL(String nodeurl) throws ElementNotFoundException, LoginException{
        ResourceManager rmStub = auth.login(credentials);	// Login against the RM.
        ArrayList<RMNodeEvent> list = rmStub.getMonitoring().getState().getNodesEvents();
        for(RMNodeEvent node: list){
        	if (node.getNodeUrl().equals(nodeurl)){
        		String ret = node.getProactiveJMXUrl();
		        rmStub.disconnect();
        		return ret;
        	}
        }
        rmStub.disconnect();
        throw new ElementNotFoundException("Node with URL " + nodeurl + " not found.");
    }
    
    public Object monitorNode(String nodeurl, String mbean, String attribute) throws Exception{
        logger.info("Ready to monitor '" + nodeurl + "', mbean '" + mbean + "' attribute '" + attribute + "'...");
        String jmxnodeurl = null;
        if (nodeurl.startsWith("service:jmx")){
	        logger.info("Using directly the nodeurl '" + nodeurl + "' as jmxnodeurl...");
        	jmxnodeurl = nodeurl;
        }else{
	        logger.info("Translating '" + nodeurl + "' into a jmxnodeurl...");
	        jmxnodeurl = getJMXNodeURL(nodeurl);
	        logger.info("Translated nodeurl '"+nodeurl+"' into jmxnodeurl '"+jmxnodeurl+"' successfully.");
        }
    	return monitor(jmxnodeurl, mbean, attribute);
    }
    
    public Object monitorNode(String nodeurl, String property) throws Exception{
        logger.info("Ready to monitor '" + nodeurl + "', property '" + property + "'...");
        String[] mbean_att = mapProperty(property);
        
        String mbean = mbean_att[0];
        String attribute = mbean_att[1];
        
        return monitorNode(nodeurl, mbean, attribute);
    }
    
    public String[] mapProperty(String property) throws Exception{
    	String[] ret = null;
    	if (property.equals("memory")){
    		ret = new String[]{"java.lang:type=Memory","HeapMemoryUsage"};
    	}else if(property.equals("disk")){
    		ret = new String[]{"sigar:Type=FileSystem,Name=/","Free"};
    	}else if(property.equals("cores")){
    		ret = new String[]{"sigar:Type=CpuCore,Name=1","Total"};
    	}else if(property.equals("cpu")){
    		ret = new String[]{"sigar:Type=Cpu","Idle"};
    	}else if(property.equals("aaa")){
    		ret = new String[]{"sigar:Type=FileSystem,Name=/","Free"};
    	}else if(property.equals("aaa")){
    		ret = new String[]{"sigar:Type=FileSystem,Name=/","Free"};
    	}else if(property.equals("aaa")){
    		ret = new String[]{"sigar:Type=FileSystem,Name=/","Free"};
    	}
    	
    	if (ret != null){
    		String mbean = ret[0];
    		String attribute = ret[1];
	        logger.info("Monitoring property '" + property + "' was mapped to mbean '" + mbean + "' and attribute '" + attribute + "'.");
    		return ret;
    	}else{
	    	throw new Exception("Could not find mbean for property '" + property + "'.");
    	}
    }
    
    public Object monitor(String jmxnodeurl, String mbean, String attribute) throws Exception {
        
        RMCachingProxyUserInterface rm = new RMCachingProxyUserInterface();
        
        logger.info("Getting connected to RMJMX...");
        boolean connected = rm.init(auth.getHostURL(), credentials);
        
    	if (connected){
	        logger.info("Connected to JMX.");
	    	ArrayList<String> atts = new ArrayList<String>();
	    	atts.add(attribute);
			logger.info("Getting attribute '" + attribute + "'...");
	    	Object object = rm.getNodeMBeanInfo(jmxnodeurl, mbean, atts);
			logger.info("Obtained: '" + object.toString() + "'.");
			//rm.disconnect(); // Cant disconnect...
	    	return object;
    	}else{
    		logger.warn("Failed to get connected to JMX...");
    		throw new Exception("Failed to get connected to JMX.");
    	}
    	
    }
    
}
