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

package org.ow2.compatibleone;

import org.ow2.compatibleone.exchangeobjects.ProcciCategory;
import org.ow2.compatibleone.exchangeobjects.RestRequest;
import org.ow2.compatibleone.exchangeobjects.RestResponse;

public interface Procci {
	/* All this methods must return a json String. Its format is user defined
	 * but it must match the definitions in the C layer. */
	public abstract String start_server(Object[] args) throws Exception;
	public abstract String stop_server(Object[] args) throws Exception;
	
}
