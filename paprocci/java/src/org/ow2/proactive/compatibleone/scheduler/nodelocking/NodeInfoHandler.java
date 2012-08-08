/*
 * ################################################################
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
