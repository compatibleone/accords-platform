#!/bin/bash
INITDIR=`dirname $0`
BASEDIR=$INITDIR/../../

echo Remember to run this as 
echo '$ source init.sh'

echo Root directory is $BASEDIR

echo Copying xsd files locally...
cp to-tmp/* /tmp/

echo Copying config files...
cp to-scripts/* $BASEDIR/scripts

echo Done.

echo NOW PROCEED AS FOLLOWS 
echo 'export LD_LIBRARY_PATH=/usr/local/lib'
echo 'cd ../../scripts'
echo './clean.sh'
echo 'export PA_PROCCI_PATH=<corresponding path of java-proactive-procci>'

echo Put in the remote ProActive node the cosacs module. 
echo Start up the ProActive Scheduler. 
