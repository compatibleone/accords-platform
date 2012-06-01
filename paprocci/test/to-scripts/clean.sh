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
