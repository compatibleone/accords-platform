#!/bin/bash

set -e 

CURRDIR=`dirname $0`

echo Stopping ACCORDS... 
#$CURRDIR/co-stop

echo Removing unneeded files... 
$CURRDIR/pademo_clean.sh

echo "Waiting for closed ports ... "
COUNT=1
while [ $COUNT -ne "0" ]; do 
	sleep 1
	COUNT=`netstat  -n | grep 8086  | wc | awk '{print $1}'`
	echo -n "."
done


echo Setting up Java environment...
export JDK=$JAVA_HOME
echo Using Java in $JAVA_HOME
# Check that it is JDK from Sun HERE!
export LD_LIBRARY_PATH=/usr/local/lib/:$JDK/jre/lib/i386/client:$JDK/jre/lib/i386

echo "Copying paprocci configuration files..."

DST_CONF=/var/lib/accords/paprocci
echo Copying configuration files to $DST_CONF...
sudo mkdir -p $DST_CONF
sudo chmod 777 $DST_CONF
rm -f $DST_CONF/*
cp $CURRDIR/../paprocci/conf/* $DST_CONF

DST_JARS=/usr/share/java/accords/paprocci
echo Copying .jar libraries to $DST_JARSs... 
sleep 2
sudo mkdir -p $DST_JARS
sudo chmod 777 $DST_JARS

#rm -f $DST_JARS/*
#cp $CURRDIR/../paprocci/java/lib/* $DST_JARS
cp $CURRDIR/../paprocci/java/dist/* $DST_JARS

echo "Starting ACCORDS..."
$CURRDIR/accords-config
echo Copying accords to /user/local/bin...
sudo cp $CURRDIR/accords /usr/local/bin/accords
sudo cp $CURRDIR/accords-config /usr/local/bin/accords-config
$CURRDIR/co-start
echo "Done."

echo "Backing up scripts..."
cp $CURRDIR/*.sh $CURRDIR/../paprocci/test/to-scripts/

$CURRDIR/co-status

