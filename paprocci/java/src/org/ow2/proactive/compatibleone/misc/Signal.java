package org.ow2.proactive.compatibleone.misc;

public class Signal {
	private boolean value;
	
	public Signal(boolean value){}
	
	public synchronized boolean getValue(){
		return value;
	};
	
	public synchronized void setValue(boolean k){
		value = k;
	};
	
}
