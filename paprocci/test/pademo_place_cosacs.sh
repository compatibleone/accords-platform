#!/bin/bash
echo Demo ProActive
echo This script generates cosacs module and places it in the local host to be called by ACCORDS. 
echo Building COSACS...
cd ../../cosacs/
make
./build.cosacs.tarball 

echo Placing COSACS locally...
mkdir -p /home/cosacs/
rm -fr /home/cosacs/* 
mv cosacstarball.tgz /home/cosacs/
cd /home/cosacs/
tar -xvf cosacstarball.tgz 
./install-cosacs
chmod 777 /home/cosacs/rest

echo Going back to previous folder...
cd -
cd ../paprocci/test/
cp to-cosacs/* /home/cosacs

echo Done. 

