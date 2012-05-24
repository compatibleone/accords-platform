#!/bin/bash
INITDIR=`dirname $0`
BASEDIR=$INITDIR/../../

echo Remember to run this as 
echo '$ source init.sh'

echo Root directory is $BASEDIR

echo Copying xsd files locally...
cp xsds/* /tmp/

echo Copying xml files locally...
cp xmls/* $BASEDIR 

echo Copying config files...
cp configs/* $BASEDIR/scripts

echo Copying manifests...
cp manifests/* $BASEDIR/scripts

echo Copying hacks...
cp hack/* $BASEDIR/scripts

echo Done.

echo REMEMBER TO DO 
echo 'export LD_LIBRARY_PATH=/usr/local/lib'

