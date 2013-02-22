#!/bin/bash
source dashboard.sh
cd $ACCORDS_BASE/usr/local/bin/testresolver --publisher https://192.168.2.7:8086 --tls security/testcpTls.xml $1 $2 $3 $4

