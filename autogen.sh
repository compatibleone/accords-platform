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

exit 0
