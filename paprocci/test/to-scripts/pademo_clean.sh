#!/bin/bash
echo Cleaning not needed files...
rm -f rest/*
rm -f service/*
rm -f security/*
rm -f links_*
rm -f plan_*
rm -f proactive.xml
rm -f publication.xml
rm -f co-log
ls cords_* | grep -v user | xargs rm -f 
echo Done. 


