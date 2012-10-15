package org.ow2.compatibleone;

import org.ow2.compatibleone.exchangeobjects.ProcciCategory;
import org.ow2.compatibleone.exchangeobjects.RestRequest;
import org.ow2.compatibleone.exchangeobjects.RestResponse;

public interface Procci {
	/* All this methods must return a json String. Its format is user defined
	 * but it must match the definitions in the C layer. */
	public abstract String start_server(Object[] args) throws Exception;
	public abstract String stop_server(RestRequest restrequest, RestResponse restresponse, ProcciCategory category, Object[] args) throws Exception;
	
}
