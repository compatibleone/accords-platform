#!/bin/bash

# starthooked 'command-line of the process to start with a hook to COSACS'
# This script starts a target process, and makes sure this process is alive if and only if 
# the COSACS process is still there. Otherwise the target process is killed. 

TARGET=$@ # This is the target process, the one that will end once COSACS ends. 

COSACS_PID=`ps -ef | grep cosacs.xml | grep -v grep | tee log_cosacs_found.txt  | awk {'print $2'} | sort -n | head -1`
#COSACS_PID=`ps -ef | grep cosacs.xml | grep -v grep | tee loscosacs.txt     | awk {'print $2'} `
echo COSACS_PID is $COSACS_PID

# Check if COSACS_PID exists. 
if [ "x$COSACS_PID" = "x" ]; then 
#if [ -z "${COSACS_PID+xxx}" ]; then 
     echo No COSACS running... Exiting...
     exit 0;
fi

# Launch the target and keep the TARGET_PID.
$TARGET &> log_target.txt &
TARGET_PID=$!

echo Launched target $TARGET with PID $TARGET_PID

# Check periodically if COSACS_PID exists, and if it doesn't just 
# kill the TARGET_PID. 
echo Waiting for COSACS to be killed to kill the target process... 
strace -p $COSACS_PID 

echo COSACS was killed, killing the target process...
kill -9 $TARGET_PID

