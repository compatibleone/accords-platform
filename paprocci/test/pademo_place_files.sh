#!/bin/bash
INITDIR=`dirname $0`
BASEDIR=$INITDIR/../../

echo Root directory is $BASEDIR

echo Copying xsd files locally, so no internet access is required...

sed 's/http:\/\/www.compatibleone.fr\/schemes/file:\/\/\/tmp/g' $BASEDIR/cordstypes.xsd > /tmp/cordstypes.xsd
sed 's/http:\/\/www.compatibleone.fr\/schemes/file:\/\/\/tmp/g' $BASEDIR/cords.xsd > /tmp/cords.xsd
sed 's/http:\/\/www.compatibleone.fr\/schemes/file:\/\/\/tmp/g' $BASEDIR/manifest.xsd > /tmp/manifest.xsd

echo Copying config files...
cp to-scripts/* $BASEDIR/scripts

echo Done.

cd $BASEDIR/scripts
./pademo_clean.sh

echo NOW PROCEED AS FOLLOWS 
echo 'cd ../../scripts'
echo 'export LD_LIBRARY_PATH=/usr/local/lib'
echo 'export PA_PROCCI_PATH=<corresponding path of java-proactive-procci>'

echo Put in the remote ProActive node the cosacs module. 
echo Start up the ProActive Scheduler. 
