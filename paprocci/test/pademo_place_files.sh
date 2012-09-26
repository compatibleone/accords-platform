#!/bin/bash
INITDIR=`dirname $0`
BASEDIR=$INITDIR/../../

echo PROACTIVE DEMO
echo This script assumes that ACCORDS-PLATFORM is already compiled and installed locally. 
echo Using as root directory: $BASEDIR

echo '   '
echo 1. Copying xsd files locally, so no internet access is required...

sed 's/http:\/\/www.compatibleone.fr\/schemes/file:\/\/\/tmp/g' $BASEDIR/cordstypes.xsd > /tmp/cordstypes.xsd
sed 's/http:\/\/www.compatibleone.fr\/schemes/file:\/\/\/tmp/g' $BASEDIR/cords.xsd > /tmp/cords.xsd
sed 's/http:\/\/www.compatibleone.fr\/schemes/file:\/\/\/tmp/g' $BASEDIR/manifest.xsd > /tmp/manifest.xsd

echo '   '
echo 2. Copying config files...
cp to-scripts/* $BASEDIR/scripts

echo '   '
echo 3. Cleaning environment from previous executions...

cd $BASEDIR/scripts
./pademo_clean.sh

echo Done all.
echo Keep on reading the README file please.
