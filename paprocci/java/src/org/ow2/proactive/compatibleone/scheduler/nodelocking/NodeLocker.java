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
import java.io.Serializable;

import org.ow2.proactive.scheduler.common.task.TaskResult;
import org.ow2.proactive.scheduler.common.task.executable.JavaExecutable;

public class NodeLocker extends JavaExecutable {

	@Override
	public Serializable execute(TaskResult... arg0) throws Throwable {
		//System.out.println(NodeInfoProvider.getInstance().toString());
		System.out.println(NodeInfoHandler.getLocalInformation().toString());
		while (true) {
			synchronized (this) {
				try {
					Thread.sleep(Long.MAX_VALUE);
				} catch (InterruptedException ie) {
					ie.printStackTrace();
				}
			}
		}
	}
}
