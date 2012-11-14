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

package org.ow2.compatibleone.exchangeobjects;

public class RestResponse {
/*
struct  rest_response {
        struct  rest_header     * first;
        struct  rest_header     * last;
        int                     status;
        char                    * version;
        char                    * message;
        int                     type;
        char                    * body;
};	 
 */

	private RestHeaderBlock all;
    private int status;
    private String version;
    private String message;
    private int type;
    private String body;
    
    public RestResponse(
    		RestHeaderBlock all,
    		int status, 
    		String version, 
    		String message, 
    		int type, 
    		String body){
    	
    		this.all = all;
    		this.status = status;
    		this.version = version;
    		this.message = message;
    		this.type = type;
    		this.body = body;
    }
    public RestHeaderBlock getAll() {
		return all;
	}
    public int getStatus() {
		return status;
	}
    public String getVersion() {
		return version;
	}
    public String getMessage() {
		return message;
	}
    public int getType() {
		return type;
	}
    public String getBody() {
		return body;
	}
    public String toString(){
    	return 
    		"[" + 
    		"block " +  all + ", \n" + 
    		"status " +  status + ", \n" + 
    		"version " +  version + ", \n" + 
    		"message " +  message + ", \n" + 
    		"type " +  type + ", \n" + 
    		"body " +  body + ", \n" + 
    		"]"  ;
    }
}
