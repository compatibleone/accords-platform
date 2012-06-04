#!/bin/bash
echo Demo ProActive
echo This script generates cosacs module and places it in the local host to be called by ACCORDS. 
mkdir -p /home/cosacs/
cd ../cosacs/
make
./build.cosacs.tarball 
rm -fr /home/cosacs/* 
mkdir -p /home/cosacs/rest
mv cosacstarball.tgz /home/cosacs/
cd /home/cosacs/
tar -xvf cosacstarball.tgz 
cd -
cd ../scripts/

