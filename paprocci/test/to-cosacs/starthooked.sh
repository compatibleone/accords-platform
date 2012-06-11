#!/bin/bash

# starthooked 'command-line of the process to start with a hook to COSACS'
# This script starts a target process, and makes sure this process is alive if and only if 
# the COSACS process is still there (by checking the LOCKFILE present). 
# Otherwise the target process is killed. 

TARGET=$@ # This is the target process, the one that will end once COSACS ends. 
LOCKFILE=/home/cosacs/lock  

# Check if COSACS_PID exists. 
if [ -e $LOCKFILE ]; then
  echo COSACS running... 
else
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
inotifywait -e delete $LOCKFILE

echo COSACS was killed, killing the target process...
kill -9 $TARGET_PID

