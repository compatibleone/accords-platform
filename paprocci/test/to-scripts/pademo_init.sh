#!/bin/bash
echo Creating needed folders...
mkdir -p rest
mkdir -p service
mkdir -p security
echo Cleaning not needed files...
ls cords_* | grep -v user | xargs rm -f 
rm -f links_*
rm -f plan_*
rm -f proactive.xml
rm -f rest/*
rm -f publication.xml
rm -f co-log
echo Done. 

echo 'Now run cosacs locally: '
echo '  /home/cosacs/run-cosacs &> output.txt &'
echo 'Now start up accords:'
echo '  ./co-start'
echo 'Now parse the manifest:'
echo '  ./co-parser pademo_manifest'
echo 'Now perform the brokering of the generated provisioning plan:'
echo '  ./co-broker pademo_manifest'
echo 'Now check the status of your system...'


