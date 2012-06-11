#!/bin/bash
CURRDIR=`dirname $0`
nohup $CURRDIR/starthooked.sh chatplus_server -l vizcacha.cloud.sophia.inria.fr -p 8086 CompatibleOneROOM &> log_chat_hooked.txt &
