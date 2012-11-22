#!/bin/bash
set -e
echo Cleaning not needed files...
mkdir -p rest
mkdir -p service
rm -f rest/*
rm -f service/*
rm -f links_*
rm -f plan_*
rm -f proactive.xml
rm -f sla-*.xml 
rm -f quota-*.xml 
rm -f publication.xml
rm -f co-log
ls cords_* | grep -v user | xargs rm -f 
echo Done. 


