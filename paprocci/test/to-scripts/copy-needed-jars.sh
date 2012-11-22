#!/bin/bash

set -e 

GOODJARS=temp
TEMPZIP=tempzip
LIBS=/usr/share/java/accords/paprocci
URLLIB=http://www.activeeon.com/public_content/releases/ProActive/3.3.0/ProActiveScheduling-3.3.0_bin.zip
FILENAME=ProActiveScheduling-3.3.0_bin.zip

echo Entering to the directory...
sudo mkdir -p $LIBS  
sudo chmod 777 $LIBS  
cd $LIBS  
wget $URLLIB  
echo Unzipping... 
unzip -j -u -d $TEMPZIP $FILENAME  

mkdir -p $GOODJARS  


LISTJARS='hibernate-jpa-2.0-api-1.0.1.Final.jar hibernate-core-4.1.3.Final.jar ProActive.jar ProActive_ResourceManager.jar ProActive_SRM-common-client.jar ProActive_SRM-common.jar ProActive_Scheduler-client.jar ProActive_Scheduler-core.jar bouncycastle.jar commons-cli-1.1.jar commons-httpclient-3.1.jar http-2.0.4.jar javassist.jar log4j.jar trilead-ssh2.jar'

#LISTJARS='hibernate-jpa-2.0-api-1.0.1.Final.jar ProActive.jar ProActive_ResourceManager.jar ProActive_SRM-common-client.jar ProActive_SRM-common.jar ProActive_Scheduler-client.jar ProActive_Scheduler-core.jar bouncycastle.jar commons-cli-1.1.jar commons-httpclient-3.1.jar http-2.0.4.jar javassist.jar log4j.jar trilead-ssh2.jar'

echo Copying good jars... 
mv $TEMPZIP/*.jar .  
mv $LISTJARS $GOODJARS    
echo Removing bad jars... 
rm *.jar   
echo Placing good jars... 
mv $GOODJARS/*.jar .  

rm -fr $GOODJARS  
rm -fr $TEMPZIP  

