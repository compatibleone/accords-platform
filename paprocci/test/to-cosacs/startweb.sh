#!/bin/bash
CURRDIR=`dirname $0`
nohup $CURRDIR/starthooked.sh python $CURRDIR/httpserver.py &> log_web_hooked.txt &
