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

package org.ow2.proactive.compatibleone.scheduler.nodelocking;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.InterfaceAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Enumeration;
import java.util.Properties;

public class NodeInfoHandler {
	public static final String HOSTNAME = "network.hostname";
	public static final String IPADDRESS = "network.address.ip";
	public static final String MACADDRESS = "network.address.mac";
	public static final String NETWORKINTERFACE = "network.address.interface";
	public static final String SECURITYPASSWORD = "security.password";
	//public final String SEPARATOR = "\\u000a";
	public static final String SEPARATOR = "           ";
	private Properties properties;

	/**
	 * You can instantiate this object, which will contain the information about the host it runs on. 
	 * @throws SocketException 
	 * @throws UnknownHostException */
	public static NodeInfoHandler getLocalInformation() throws SocketException, UnknownHostException{
		Properties properties = new Properties();
		
		properties.setProperty(SECURITYPASSWORD, "demo");
		properties.setProperty(HOSTNAME, java.net.InetAddress.getLocalHost().getHostName());
		Enumeration<NetworkInterface> networkInterfaces = NetworkInterface.getNetworkInterfaces();
		while (networkInterfaces.hasMoreElements()) { // Assuming there is only one non-loopback interface... 
			NetworkInterface nextInterface = networkInterfaces.nextElement();
			if (nextInterface.isLoopback()) { // Do not consider if loopback.
				continue;
			}
			
			for (InterfaceAddress interfaceAddress : nextInterface.getInterfaceAddresses()) {
				InetAddress inetAddress = interfaceAddress.getAddress();
				if (!(inetAddress instanceof Inet4Address)) {
					continue;
				}
				
				properties.setProperty(IPADDRESS, inetAddress.getHostAddress());
				properties.setProperty(NETWORKINTERFACE, nextInterface.getDisplayName());
				byte[] hardwareAddress = nextInterface.getHardwareAddress();

				StringWriter sw = new StringWriter();
				PrintWriter pw = new PrintWriter(sw);
				for (int i = 0; i < hardwareAddress.length; i++) {
					pw.format("%02X%s", hardwareAddress[i],
							(i < hardwareAddress.length - 1) ? "-" : "");
				}
				properties.setProperty(MACADDRESS, sw.toString());
			}
		}
		NodeInfoHandler a = new NodeInfoHandler();
		a.setProperties(properties);
		return a;
	}

	public void set(String key, String value){
		properties.setProperty(key, value);
	}
	
	public String get(String key){
		return properties.getProperty(key);
	}
		
	private void setProperties(Properties p){
		properties = p;
	}
	
	public static NodeInfoHandler parse(String nodeSource) throws IOException {
		Properties properties = new Properties();
		InputStream in = new ByteArrayInputStream(nodeSource.replace(SEPARATOR, "\n").getBytes());
		properties.load(in);
		NodeInfoHandler info = new NodeInfoHandler();
		info.setProperties(properties);
		return info;
	}
	
	public String toString() {
		String ret = "";
		for (Object key: properties.keySet()){
			ret = ret + key + "=" + properties.getProperty(key.toString()) + SEPARATOR;
		}
		return ret;
	}
	
	public String toNiceString() {
		String ret = "";
		for (Object key: properties.keySet()){
			ret = ret + key + "=" + properties.getProperty(key.toString()) + "\n";
		}
		return ret;
	}
	
	public static void main(String args[]) throws IOException{
		NodeInfoHandler a = NodeInfoHandler.getLocalInformation();
		System.out.println(a.toNiceString());
		
		NodeInfoHandler b = NodeInfoHandler.parse(a.toString());
		System.out.println(b.toNiceString());
	}
	
}
