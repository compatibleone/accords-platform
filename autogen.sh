#!/bin/sh
# (c) 2011 Mandriva, http://www.mandriva.com
#
# Authors:
#   Jean Parpaillon <jparpaillon@mandriva.com>
#
set -e

mkdir -p $(dirname $0)/m4

$(dirname $0)/clean

autoreconf --install

#For PyaccordsSDK configuration
PWD=$(pwd)
sed -i '/#define PYPATH/d' pyaccords/pysrc/pypath.h
sed -i "4i\#define PYPATH \"$PWD\"" pyaccords/pysrc/pypath.h

sed -i '/srcpydir/d' pyaccords/pysrc/pypacksrc/pypacksrc.py
sed -i "4i\srcpydir=\"$PWD\"" pyaccords/pysrc/pypacksrc/pypacksrc.py

exit 0
