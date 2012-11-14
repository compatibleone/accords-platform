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

import java.util.ArrayList;

public class RestHeaderBlock {

    private ArrayList<RestHeader> list;
    
    public RestHeaderBlock(){
    	list = new ArrayList<RestHeader>();
    }
    
    public void add(RestHeader e) {
    	list.add(e);
	}
    
    public void add(String name, String value) {
    	list.add(new RestHeader(name, value));
	}
    
    public String toString(){
    	String ret = "[";
    	for (RestHeader i: list){
    		ret = ret + i.toString() + ", ";
    	}
    	ret = ret + "]";
    	return ret;
    }
}
