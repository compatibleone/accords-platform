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

echo REMEMBER TO DO 
echo 'export LD_LIBRARY_PATH=/usr/local/lib'

