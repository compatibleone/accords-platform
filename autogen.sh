#!/bin/sh
# (c) 2011,2012 Prologue, CityPassenger, Mandriva
#
# Authors:
#   Hamid MEDJAHED <hmedjahed@prologue.fr>
#   Elyes ZEKRI <ezekri@prologue.fr>
#   Vincent Renardias <vrenardias@citypassenger.com>
#   Jean Parpaillon <jparpaillon@mandriva.com>
#
set -e

mkdir -p $(dirname $0)/m4

$(dirname $0)/clean

autoreconf --install

#For PyaccordsSDK configuration
PWD=$(pwd)
sed -e "s#@@PWD@@#${PWD}#" < pyaccords/pysrc/pypath.h.in > pyaccords/pysrc/pypath.h
sed -e "s#@@PWD@@#${PWD}#" < pyaccords/pysrc/pypacksrc/pypacksrc.py.in > pyaccords/pysrc/pypacksrc/pypacksrc.py

exit 0
