#!/bin/bash
# ---------

# -----------------------------------------------------
# Generation of the Dashboard PHP Initialisation script
# -----------------------------------------------------
echo "<?php" > dashboard.inc

function dashboard_service()
{
	v=`co-resolver $1`
	if [ -z "$v" ]; then
		echo "\$$1=\"https://127.0.0.1:8086\";" 	>> dashboard.inc
		e=fail
	else
		v=`echo $v | cut -f 2 -d =`
		echo "\$$1=\"$v\";" 			>> dashboard.inc
	fi
}

# ---------------------------
# the dashboard refresh timer
# ---------------------------
dashboard_timer=$1
dashboard_number=$2
log_timer=$3

if [ -z "$dashboard_timer" ]; then
	dashboard_timer=30;
fi
if [ -z "$dashboard_number" ]; then
	dashboard_number=1;
fi
if [ -z "$log_timer" ]; then
	log_timer=15;
fi

echo "\$dashboard_timer=\"${dashboard_timer}\";" >> dashboard.inc
echo "\$log_timer=\"${log_timer}\";" >> dashboard.inc
echo "\$dashboard_number=\"${dashboard_number}\";" >> dashboard.inc

configdir=`pwd`
echo "\$configdir=\"$configdir\";" >> dashboard.inc
echo "export ACCORDS_BASE=$configdir" > dashboard.sh

# ---------------------------------------------------
# the publisher information for the parser and broker
# ---------------------------------------------------
var=`grep publisher accords.xml | tail -n 1 | cut -f 2 -d =`
echo "\$publisher=$var;" >> dashboard.inc

echo "/usr/bin/testcp --tls security/testcpTls.xml --publisher $var \$1 \$2 \$3 \$4" > ./dashboard-parser
chmod uog+x dashboard-parser

echo "/usr/bin/testcb --tls security/testcbTls.xml --publisher $var \$1 \$2 \$3 \$4" > ./dashboard-broker
chmod uog+x dashboard-broker

echo "/usr/bin/command --tls security/commandTls.xml --publisher $var invoice \$1 " > ./dashboard-invoice
chmod uog+x dashboard-invoice

echo "/usr/bin/command --tls security/commandTls.xml --publisher $var \$* " > ./dashboard-command
chmod uog+x dashboard-command

chmod uog+rw . rest service co-log
chmod uog+r security security/*

# ----------------------
# description categories
# ----------------------
dashboard_service manifest
dashboard_service node
dashboard_service infrastructure
dashboard_service image
dashboard_service compute
dashboard_service network
dashboard_service storage
dashboard_service port
dashboard_service system
dashboard_service package
dashboard_service configuration
dashboard_service action
dashboard_service release
dashboard_service interface
dashboard_service security

# -----------------------------
# platform operation categories
# -----------------------------
dashboard_service service
dashboard_service contract
dashboard_service instruction
dashboard_service firewall
dashboard_service instruction
dashboard_service placement
dashboard_service quantity
dashboard_service provider
dashboard_service consumer
dashboard_service quota

dashboard_service account
dashboard_service user
dashboard_service authorization
dashboard_service price
dashboard_service transaction
dashboard_service invoice
dashboard_service event
dashboard_service alert
dashboard_service probe
dashboard_service session
dashboard_service connection
dashboard_service monitor
dashboard_service agreement
dashboard_service penalty
dashboard_service reward

# --------------------
# provisioning proccis
# --------------------
dashboard_service openstack
dashboard_service opennebula
dashboard_service windowsazure
dashboard_service amazon
dashboard_service deltacloud
dashboard_service proactive
dashboard_service cloudfoundry
dashboard_service jpaasprocci

echo "?>" >> dashboard.inc













