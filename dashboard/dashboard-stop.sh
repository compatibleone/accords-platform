#!/bin/sh
#
# Wrapper for accords commands
#
# Copyright: 
#   Prologue 2011
#   Mandriva 2011
#   Bull 2012
#   CityPassenger 2012
#
# Author: 
#   Jean Parpaillon
#   Jamie Marshall
#   Liliana Cabalantti
#   Vincent Renardias
#

set -e

export PATH=:$PATH

confdir=/etc/accords
if test -e ./accords.ini; then
    . ./accords.ini
elif test -e $confdir/accords.ini; then
    . $confdir/accords.ini
fi

# -------------------------------------
# to activate the security using nethsm
# set nethsm to 1
# then specify the security directory.
# -------------------------------------
# nethsm=1
tls=security
# -------------------------------------
# sans security 
# -------------------------------------
nethsm=
# tls=
# -------------------------------------
resthost=127.0.0.1
secdir=$tls
zone="europe"
trace=1
threads=1
verbose=0
debug=0
domain=occi
operator=accords
password=co-system

if [ -z "$accords_workdir"]; then
	accords_workdir="."
fi

if [ ! -d "$accords_workdir" ]; then
        echo "$accords_workdir: no such directory"
        exit 1
fi

if [ ! -d "$accords_workdir/rest" ]; then
        echo "$accords_workdir/rest: no such directory"
        exit 1
fi

cd $accords_workdir

if [ -z "$tls" ]; then
    tempo=1
else
    tempo=2
fi

if [ -z "$target" ]; then
    target=accords.xml
fi

if [ -z "$resthost" ]; then
    resthost=127.0.0.1
fi

if [ -z "$restport" ]; then
    restport=8086
fi

if [ -z "$publisher" ]; then
	if [ -z "$tls" ]; then
	    publisher=http://$resthost:$restport
	else
	    publisher=https://$resthost:$restport
	fi
fi

if [ -z "$monitor" ]; then
    monitor=$trace
fi

###
### Aliases
###
### The script must be called through one of the following aliases.
###
# aliases="co-start co-broker co-command co-parser co-resolver co-status co-stop run-azprocci run-broker run-cops run-coees run-comons run-conets run-coobas run-coss run-coips run-ezvm run-fileserver run-onprocci run-osprocci run-paprocci run-parser run-copaas run-jpaasprocci run-procci run-cosched run-publisher accords-config"
aliases="@aliases@"

_co_start() {
    echo "Starting ACCORDS"

# ---------------------------------------------------
#       The basic accords platform components
# ---------------------------------------------------
    echo "Starting accords publication services"
    /usr/bin/run-publisher &
    sleep $tempo

    echo "Starting accords file services"
    /usr/bin/run-fileserver &
    sleep $tempo
    
    echo "Starting accords security services"
    /usr/bin/run-coss &
    sleep $tempo

    echo "Starting accords monitoring services"
    /usr/bin/run-comons &
    sleep $tempo
    
    echo "Starting accords accounting services"
    /usr/bin/run-coobas &
    sleep $tempo
    
    echo "Starting accords scheduling services"
    /usr/bin/run-cosched &
    sleep $tempo

    echo "Starting accords networking services"
    /usr/bin/run-conets &
    sleep $tempo

    echo "Starting accords placement services"
    /usr/bin/run-cops &
    sleep $tempo
    
    echo "Starting accords energy efficiency services"
    /usr/bin/run-coees &
    sleep $tempo
    
    echo "Starting accords image storage services"
    /usr/bin/run-ezvm &
    sleep $tempo

    echo "Starting accords image production services"
    /usr/bin/run-coips &
    sleep $tempo

    echo "Starting accords sla services"
    /usr/bin/run-slam &
    sleep $tempo

    echo "Starting accords paas services"
    /usr/bin/run-copaas &
    sleep $tempo

    echo "Starting accords parser services"
    /usr/bin/run-parser &
    sleep $tempo

    echo "Starting accords broker services"
    /usr/bin/run-broker &
    sleep $tempo

    echo "Starting accords procci services"
    /usr/bin/run-procci &
    sleep $tempo

# ---------------------------------------------------
#       The provisioning services agreements 
# ---------------------------------------------------
    sleep 5
    /usr/bin/co-parser accounts
    /usr/bin/co-parser metrics
    /usr/bin/co-parser coips
# ---------------------------------------------------
#    The accords platform provisioning components
# ---------------------------------------------------
    echo "Starting accords open stack services"
    /usr/bin/co-provider osprocci accords 30 10 1000 30 30
    /usr/bin/co-parser quota-osprocci
    /usr/bin/co-parser sla-osprocci
    /usr/bin/run-osprocci &
    sleep $tempo

    echo "Starting accords open nebula services"
    /usr/bin/co-provider onprocci accords 20 20 1000 20 20   
    /usr/bin/co-parser quota-onprocci
    /usr/bin/co-parser sla-onprocci
    /usr/bin/run-onprocci &
    sleep $tempo
    
    echo "Starting accords proactive services"
    /usr/bin/co-provider paprocci accords 5 5 500 5 5
    /usr/bin/co-parser quota-paprocci
    /usr/bin/co-parser sla-paprocci
    /usr/bin/run-paprocci &
    sleep $tempo

    echo "Starting accords windows azure services"
    /usr/bin/co-provider azprocci accords 10 10 1000 10 10
    /usr/bin/co-parser quota-azprocci
    /usr/bin/co-parser sla-azprocci
    /usr/bin/run-azprocci &
    sleep $tempo

    echo "Starting accords delta cloud services"
    /usr/bin/co-provider dcprocci accords 1 1 1000 1 1
    /usr/bin/co-parser quota-dcprocci
    /usr/bin/co-parser sla-dcprocci
    /usr/bin/run-dcprocci &
    sleep $tempo

    echo "Starting accords jpaas procci services"
    /usr/bin/co-provider jpaasprocci accords 10 10 1000 10 10
    /usr/bin/co-parser quota-jpaasprocci
    /usr/bin/co-parser sla-jpaasprocci
    /usr/bin/run-jpaasprocci &
    sleep $tempo

# ---------------------------------------------------
#    These have been commented out since they are 
#    not currently being used by an active platform
# ---------------------------------------------------
#    echo "Starting accords occi open stack services"
#    /usr/bin/run-osocciprocci &
#    sleep $tempo
# ---------------------------------------------------
#    echo "Starting accords test occi open stack server"
#    /usr/bin/run-testosocci &
#    sleep $tempo
# ---------------------------------------------------
#    echo "Starting accords procci example"
#    /usr/bin/run-example &
#    sleep $tempo
# ---------------------------------------------------

    echo "Starting accords software configuration services"
    /usr/bin/run-cosacs &
    sleep $tempo

    echo "ACCORDS ready"
    /usr/bin/co-status
}

_co_broker() {
    if [ -z "$tls" ]; then
	/usr/bin/testcb --publisher "$publisher" --verbose plan_$1.xml
    else
	/usr/bin/testcb --tls security/testcbTls.xml --publisher "$publisher" --verbose plan_$1.xml
    fi
} 

_co_command() {
    if [ -z "$tls" ]; then
	/usr/bin/command --publisher "$publisher" $*
    else
	/usr/bin/command --tls $secdir/commandTls.xml --publisher "$publisher" $*
    fi
}

_co_parser() {
    if [ -z "$tls" ]; then
	/usr/bin/testcp --publisher "$publisher" --verbose $2 $1.xml
    else
	/usr/bin/testcp --tls $secdir/testcpTls.xml --publisher "$publisher" --verbose $2 $1.xml
    fi
}

_co_provider() {

name=$1
operator=$1
cpu=$3
ram=$4
disk=$5
vm=$6
adr=$7

if [ -z "$cpu" ]; then
cpu=10
fi

if [ -z "$ram" ]; then
ram=10
fi

if [ -z "$disk" ]; then
disk=1000
fi

if [ -z "$vm" ]; then
vm=10
fi

if [ -z "$adr" ]; then
adr=10
fi

echo "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" > quota-$name.xml
echo "<provider name=\"$name\" operator=\"$name\" zone=\"europe\" " > quota-$name.xml
echo " xmlns=\"http://www.compatibleone.fr/schemes/provision.xsd\">" >> quota-$name.xml 
echo "<quota name=\":cpu\" property=\"occi.compute.cores\" offered=\"$cpu\">" >> quota-$name.xml 
echo "<price name=\":cpu:price\" operator=\"$name\" rate=\"0.04\" currency=\"euro\" period=\"hour\"/>" >> quota-$name.xml
echo "</quota>" >> quota-$name.xml
echo "<quota name=\":ram\" property=\"occi.compute.memory\" offered=\"$ram\">" >> quota-$name.xml 
echo "<price name=\":ram:price\" operator=\"$name\" rate=\"0.01\" currency=\"euro\" period=\"hour\"/>" >> quota-$name.xml
echo "</quota>" >> quota-$name.xml
echo "<quota name=\":disk\" property=\"occi.storage.size\" offered=\"$disk\">" >> quota-$name.xml 
echo "<price name=\":disk:price\" operator=\"$name\" rate=\"0.03\" currency=\"euro\" period=\"hour\"/>" >> quota-$name.xml
echo "</quota>" >> quota-$name.xml
echo "<quota name=\":vm\" property=\"accords.vm.count\" offered=\"$vm\">" >> quota-$name.xml 
echo "<price name=\":vm:price\" operator=\"$name\" rate=\"1.00\" currency=\"euro\" fixed=\"true\"/>" >> quota-$name.xml
echo "</quota>" >> quota-$name.xml
echo "<quota name=\":adr\" property=\"accords.address.count\" offered=\"$adr\">" >> quota-$name.xml 
echo "<price name=\":adr:price\" operator=\"$name\" rate=\"0.05\" currency=\"euro\" period=\"hour\"/>" >> quota-$name.xml
echo "</quota>" >> quota-$name.xml
echo "</provider>" >> quota-$name.xml

echo "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" > sla-$name.xml
echo "<agreement name=\"$name\" xmlns=\"http://www.compatibleone.fr/schemes/slam.xsd\" initiator=\"$operator\" responder=\"accords\" serviceprovider=\"initiator\" description=\"generated provider SLA\" initiation=\"now\" expiration=\"never\" template=\"none\">" >> sla-$name.xml 
echo "<terms name=\"$name:services\" type=\"services\">" >> sla-$name.xml 
echo "<term name=\"$name:service1\">" >> sla-$name.xml 
echo "<provider name=\"$name\">" >> sla-$name.xml 
echo "</manifest>" >> sla-$name.xml
echo "</term>" >> sla-$name.xml
echo "</terms>" >> sla-$name.xml
echo "<terms name=\"$name:conditions\" type=\"conditions\">" >> sla-$name.xml 
echo "<term name=\"$name:condition1\">" >> sla-$name.xml 
echo "<variable name=\"$name:v1\" property=\"occi.placement.algorithm\" condition=\"eq\" value=\"zone\"/>" >> sla-$name.xml 
echo "<variable name=\"$name:v2\" property=\"occi.placement.zone\" condition=\"eq\" value=\"europe\"/>" >> sla-$name.xml 
echo "</term>" >> sla-$name.xml
echo "</terms>" >> sla-$name.xml
echo "<terms name=\"accords:guarantees\" type=\"guarantees\"/>" >> sla-$name.xml 
echo "</agreement>" >> sla-$name.xml

}


_co_resolver() {
    if [ -z "$tls" ]; then
	/usr/bin/testresolver --publisher "$publisher" $*
    else
	/usr/bin/testresolver --tls security/testresolverTls.xml --publisher "$publisher" $*
    fi
}

_co_status() {
    echo "-----------------------------"
    echo " Accords Platform Components "
    echo "-----------------------------"
    if [ "$(uname -s)" = 'Linux' ] ; then
	netstat -a -p -n | grep LISTEN | egrep '(publisher|fileserver|coss|comons|conets|coobas|cops|coees|parser|broker|procci|cosched|ezvm|coips|cosacs)'
    elif [ "$(uname -s)" = 'Darwin' ] ; then
	lsof -i | grep LISTEN | grep publisher || true
	lsof -i | grep LISTEN | grep fileserver || true
	lsof -i | grep LISTEN | grep coss || true
	lsof -i | grep LISTEN | grep comons || true
	lsof -i | grep LISTEN | grep conets || true
	lsof -i | grep LISTEN | grep coobas || true
	lsof -i | grep LISTEN | grep cops || true
	lsof -i | grep LISTEN | grep coees || true
	lsof -i | grep LISTEN | grep parser || true
	lsof -i | grep LISTEN | grep broker || true
	lsof -i | grep LISTEN | grep procci || true
	lsof -i | grep LISTEN | grep cosched || true
	lsof -i | grep LISTEN | grep ezvm || true
	lsof -i | grep LISTEN | grep coips || true
	lsof -i | grep LISTEN | grep cosacs || true
    else
	echo "Unsupported OS ($(uname -s))"
	exit 1
    fi
}

_co_stop() {
    _stop_service cosacs
    _stop_service example
    _stop_service slam
    _stop_service jpaasprocci
    _stop_service copaas
    _stop_service onprocci
    _stop_service osocciprocci
    _stop_service testosocci
    _stop_service dcprocci
    _stop_service osprocci
    _stop_service paprocci
    _stop_service azprocci
    _stop_service cosched
    _stop_service procci
    _stop_service broker
    _stop_service parser
    _stop_service ezvm
    _stop_service coips
    _stop_service coees
    _stop_service cops
    _stop_service conets
    _stop_service coobas
    _stop_service comons
    _stop_service coss
    _stop_service fileserver
    _stop_service publisher
}

_run_azprocci() {
    /usr/bin/azprocci --config $target CO-AZPROCCI/1.0
} 

_run_broker() {
    /usr/bin/broker --config $target CO-BROKER/1.0
} 

_run_cops() {
    /usr/bin/cops --config $target CO-ES/1.0
} 

_run_coees() {
    /usr/bin/coees --config $target CO-EES/1.0
} 

_run_comons() {
    /usr/bin/comons --config $target CO-MONS/1.0
}

_run_conets() {
    /usr/bin/conets --config $target CO-NETS/1.0
}

_run_coobas() {
    /usr/bin/coobas --config $target CO-BAS/1.0
}

_run_cosacs() {
    /usr/bin/cosacs --config $target CO-SACS/1.0
}

_run_coss() {
    /usr/bin/coss --config $target CO-SS/1.0
}

_run_ezvm() {
    /usr/bin/ezvm --config $target CO-EZVM/1.0
}

_run_coips() {
    /usr/bin/coips --config $target CO-IPS/1.0
}

_run_fileserver() {
    /usr/bin/fileserver --config $target CO-FS/1.0
}

_run_onprocci() {
    /usr/bin/onprocci --config $target CO-ONPROCCI/1.0
}

_run_osprocci() {
    /usr/bin/osprocci --config $target CO-OSPROCCI/1.0
}

_run_paprocci() {
    paprocci --config $target CO-PAPROCCI/1.0
}

_run_osocciprocci() {
    /usr/bin/osocciprocci --config $target CO-OSOCCIPROCCI/1.0
}

_run_testosocci() {
    /usr/bin/testosocci --config $target CO-TESTOSOCCI/1.0
}

_run_slam() {
    /usr/bin/slam --config $target CO-SLAM/1.0
}

_run_copaas() {
    /usr/bin/copaas --config $target CO-PAAS/1.0
}

_run_jpaasprocci() {
    /usr/bin/jpaasprocci --config $target CO-JPAASPROCCI/1.0
}

_run_example() {
    /usr/bin/example --config $target CO-EXAMPLE/1.0
}

_run_dcprocci() {
    /usr/bin/dcprocci --config $target CO-DCPROCCI/1.0
}

_run_parser() {
    /usr/bin/parser --config $target CO-PARSER/1.0
}

_run_procci() {
    /usr/bin/procci --config $target CO-PROCCI/1.0
}
_run_cosched() {
    /usr/bin/cosched --config $target CO-SCHED/1.0
}
 
_run_publisher() {
    /usr/bin/publisher --config $target CO-PUB/1.0
}

_run_fileserver() {
    /usr/bin/fileserver --config $target CO-FS/1.0
}

_make_security_files() {
	_mksecs publisher 0
	_mksecs fileserver 0
	_mksecs coss 0
	_mksecs comons 1
	_mksecs cops 1
	_mksecs coees 1
	_mksecs coobas 1
	_mksecs conets 1
	_mksecs broker 1
	_mksecs ezvm 1
	_mksecs coips 1
	_mksecs cosacs 1
	_mksecs parser 1
	_mksecs procci 1
	_mksecs cosched 1
	_mksecs osprocci 1
	_mksecs paprocci 1
	_mksecs testosocci 1
	_mksecs osocciprocci 1
	_mksecs onprocci 1
	_mksecs azprocci 1
	_mksecs dcprocci 1
	_mksecs slam 1
	_mksecs copaas 1
	_mksecs jpaasprocci 1
	_mksecs example 1
	_mksecs command
    
	_mksect testcp 1
	_mksect testpa 1
	_mksect testcb 1
	_mksect teston 1
	_mksect testos 1
	_mksect testresolver 1
	_mksect command 1
}

_accords_config() {

    if [ -z "$tls" ]; then
	echo "Accords Security Not Required"
    else
	echo "Creating Accords Security Files"
    
	export calist=Certificates.crt
	export pass=Accords

	if [ -z "$nethsm" ]; then 
		export options=13

	else
		export options=525
	fi
    

	if test -d $secdir; then
	   if [ -z "$nethsm" ]; then
	    	echo "ACCORDS security dir exists ($secdir): please remove it first !"
	    	exit 1
	   else
	    	echo "ACCORDS security dir exists ($secdir): certificates have not been regenerated !"
		_make_security_files
	   fi
	else
	    	mkdir -p $secdir
		_make_security_files

		echo "Creating Accords Certificats"
	
		export bits=2048
		export days=365
		export sslconfig=./openssl.cnf

		_mkcert coss
		_mkcert cops
		_mkcert coees
		_mkcert coobas
		_mkcert comons
		_mkcert conets
		_mkcert broker
		_mkcert parser
		_mkcert ezvm
		_mkcert coips
		_mkcert publisher
		_mkcert fileserver
		_mkcert procci
		_mkcert cosched
		_mkcert osprocci
		_mkcert testosocci
		_mkcert osocciprocci
		_mkcert onprocci
		_mkcert paprocci
		_mkcert azprocci
		_mkcert dcprocci
		_mkcert slam
		_mkcert copaas
		_mkcert jpaasprocci
		_mkcert example
		_mkcert cosacs
	
		_mkcert testcp
		_mkcert testcb
		_mkcert testos
		_mkcert testpa
		_mkcert teston
		_mkcert testresolver
		_mkcert command
	fi
    fi

    echo "Creating Accords Port Configuration : ports.xml"

    echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > ports.xml
    echo "<import name=\"accords-ports\" xmlns=\"http://www.compatibleone.fr/schemes/cords.xsd\">" >> ports.xml
    echo "<network name=\"accords-platform\">" >> ports.xml

    echo "Creating Accords Global Configuration File: $target"
    echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > $target
    echo "<accords>" >> $target

    _mkpub publisher
    _mkmon coss 
    _mkmon comons 
    _mkcfg cops 
    _mkcfg conets 
    _mkcfg coobas 
    _mkcfg parser 
    _mkcfg broker 
    _mkcfg procci 
    _mkpro osprocci  
    _mkpro paprocci  
    _mkcfg testosocci
    _mkpro osocciprocci  
    _mkpro onprocci
    _mkpro azprocci
    _mkpro dcprocci
    _mkcfg fileserver
    _mkcfg ezvm
    _mkcfg coips
    _mkcfg coees 
    _mkcfg cosched
    _mkcfg slam
    _mkcfg copaas
    _mkpro jpaasprocci
    _mkcfg example
    _mkcosacs cosacs

    echo "</accords>" >> $target
    echo "</network>" >> ports.xml
    echo "</import>" >> ports.xml
}

###
### Helper functions
###
_usage() {
    echo "Usage: this script must be called through one of the following name:"
    for alias in $aliases; do
	echo " * $alias"
    done
}

_stop_service() {
    service=$1
    echo "Stopping service: $service"
    if [ "$(uname -s)" = 'Linux' ] ; then
	killall -q $service || true
	killall -q lt-$service || true
    elif [ "$(uname -s)" = 'Darwin' ] ; then
	killall $service >/dev/null 2>&1 || true
	killall lt-$service >/dev/null 2>&1 || true
    else
	echo "Unsupported OS ($(uname -s))"
	exit 1
    fi
    sleep $tempo
}

_mksecs() {
    module=$1
    auth=$2
    echo "<tls key='$secdir/"${module}"PrivateKey.pem'" > $secdir/${module}Tls.xml
    echo "     certificate='$secdir/"${module}"Certificate.pem'" >> $secdir/${module}Tls.xml
    echo "     passphrase='"$pass"'" >> $secdir/${module}Tls.xml
    echo "     authority='"$calist"'" >> $secdir/${module}Tls.xml
    echo "     authenticate='"$auth"'" >> $secdir/${module}Tls.xml
    echo "     option='"$options"' />" >> $secdir/${module}Tls.xml
    chmod 0640 $secdir/${module}Tls.xml
}

_mksect() {
    module=$1
    auth=$2
    echo "<tls key='$secdir/"${module}"PrivateKey.pem'" > $secdir/${module}Tls.xml
    echo "     certificate='$secdir/"${module}"Certificate.pem'" >> $secdir/${module}Tls.xml
    echo "     passphrase='"$pass"'" >> $secdir/${module}Tls.xml
    echo "     authority='"$calist"'" >> $secdir/${module}Tls.xml
    echo "     authenticate='"$auth"'" >> $secdir/${module}Tls.xml
    echo "     option='"$optiont"' />" >> $secdir/${module}Tls.xml
    chmod 0640 $secdir/${module}Tls.xml
}

_mkcert() {
    echo "Creating certificate for module $1"
    module=$1
    if test ! -e $sslconfig; then
	echo "Can not find openSSL config file: $sslconfig"
	exit 1
    fi

    if [ -z "$nethsm" ]; then

	openssl req -config $sslconfig \
	    -newkey $bits -x509 -days $days -sha1 \
	    -keyout ${secdir}/${module}PrivateKey.pem -out ${secdir}/${module}Certificate.pem 2> /dev/null
    else
	openssl req -engine pkcs11 -keyform e -config $sslconfig \
	    -newkey $bits -x509 -days $days -sha1 \
	    -keyout ${secdir}/${module}PrivateKey.pem -out ${secdir}/${module}Certificate.pem
    fi
    chmod 0400 ${secdir}/${module}PrivateKey.pem
    chmod 0444 ${secdir}/${module}Certificate.pem
}

_mktls() {
    module=$1
    if test -z "$tls"; then
	echo "         tls=''/>" >> $target
    else
	echo "         tls='"$tls"/"${module}"Tls.xml'/>" >> $target
    fi
}

_mkpub() {
    module=$1
    echo "<"${module} >> $target
    echo "    publisher=''"           >> $target
    echo "    operator='"$operator"'"           >> $target
    echo "    identity=''" >> $target
    echo "    verbose='"$verbose"'"   >> $target
    echo "    debug='"$debug"'"       >> $target
    echo "    threads='"$threads"'>"  >> $target
    echo "    <rest host='"$resthost"' port='"$restport"'/>" >> $target
    echo "    <security" >> $target
    echo "         user='service-"${module}"'" >> $target
    echo "         password='"$password"'" >> $target
    echo "         monitor='"$trace"'" >> $target
    _mktls ${module}
    echo "    <domain name='"$domain"' zone='"$zone"'/>" >> $target
    echo "</"${module}">" >> $target
    echo "<port name=\"${module}\" protocol=\"tcp\" range=\"0.0.0.0/0\" from=\""$restport"\" to=\""$restport"\"/>" >> ports.xml
}

_mkmon() {
    module=$1
    export restport=$(($restport+1))
    echo "<"${module} >> $target
    echo "    publisher='"$publisher"'"           >> $target
    echo "    operator='"$operator"'"           >> $target
    echo "    identity='"$resthost":"$restport"'" >> $target
    echo "    verbose='"$verbose"'"   >> $target
    echo "    debug='"$debug"'"       >> $target
    echo "    threads='"$threads"'>"  >> $target
    echo "    <rest host='"$resthost"' port='"$restport"'/>" >> $target
    echo "    <security" >> $target
    echo "         user='service-"${module}"'" >> $target
    echo "         password='"$password"'" >> $target
    echo "         monitor='"$trace"'" >> $target
    _mktls ${module}
    echo "    <domain name='"$domain"' zone='"$zone"'/>" >> $target
    echo "</"${module}">" >> $target
    echo "<port name=\""${module}"\" protocol=\"tcp\" range=\"0.0.0.0/0\" from=\""$restport"\" to=\""$restport"\"/>" >> ports.xml
}

_mkcosacs() {
    module=$1
    echo "<"${module} >> $target
    echo "    publisher='"$publisher"'"           >> $target
    echo "    operator='"$operator"'"           >> $target
    echo "    identity='"$resthost":8286'" >> $target
    echo "    verbose='"$verbose"'"   >> $target
    echo "    debug='"$debug"'"       >> $target
    echo "    threads='"$threads"'>"  >> $target
    echo "    <rest host='"$resthost"' port='8286'/>" >> $target
    echo "    <security" >> $target
    echo "         user='service-"${module}"'" >> $target
    echo "         password='"$password"'" >> $target
    echo "         monitor='"$trace"'" >> $target
    _mktls ${module}
    echo "    <domain name='"$domain"' zone='"$zone"'/>" >> $target
    echo "</"${module}">" >> $target
    echo "<port name=\""${module}"\" protocol=\"tcp\" range=\"0.0.0.0/0\" from=\"8286\" to=\"8286\"/>" >> ports.xml
}

_mkcfg() {
    module=$1
    export restport=$(($restport+1))
    echo "<"${module} >> $target
    echo "    publisher='"$publisher"'"           >> $target
    echo "    operator='"$operator"'"           >> $target
    echo "    identity='"$resthost":"$restport"'" >> $target
    echo "    verbose='"$verbose"'"   >> $target
    echo "    debug='"$debug"'"       >> $target
    echo "    threads='"$threads"'>"  >> $target
    echo "    <rest host='"$resthost"' port='"$restport"'/>" >> $target
    echo "    <security" >> $target
    echo "         user='service-"${module}"'" >> $target
    echo "         password='"$password"'" >> $target
    echo "         monitor='"$monitor"'" >> $target
    _mktls ${module}
    echo "    <domain name='"$domain"' zone='"$zone"'/>" >> $target
    echo "</"${module}">" >> $target
    echo "<port name=\""${module}"\" protocol=\"tcp\" range=\"0.0.0.0/0\" from=\""$restport"\" to=\""$restport"\"/>" >> ports.xml
}

_mkpro() {
    module=$1
    export restport=$(($restport+1))
    echo "<"${module} >> $target
    echo "    publisher='"$publisher"'"           >> $target
    echo "    operator='"${module}"'"               >> $target
    echo "    identity='"$resthost":"$restport"'" >> $target
    echo "    verbose='"$verbose"'"   >> $target
    echo "    debug='"$debug"'"       >> $target
    echo "    threads='"$threads"'>"  >> $target
    echo "    <rest host='"$resthost"' port='"$restport"'/>" >> $target
    echo "    <security" >> $target
    echo "         user='service-"${module}"'" >> $target
    echo "         password='"$password"'" >> $target
    echo "         monitor='"$monitor"'" >> $target
    _mktls ${module}
    echo "    <domain name='"$domain"' zone='"$zone"'/>" >> $target
    echo "</"${module}">" >> $target
    echo "<port name=\""${module}"\" protocol=\"tcp\" range=\"0.0.0.0/0\" from=\""$restport"\" to=\""$restport"\"/>" >> ports.xml
}

if test "`basename $0`" != "accords-config"; then
    if test ! -e $target; then
	echo "Can not find target file: $target."
	echo "Please run accords-config first !"
	exit 1
    fi
fi

case `basename $0` in
    co-start) 
	_co_start $*
	;;
    co-broker) 
	_co_broker $*
	;;
    co-command) 
	_co_command $*
	;;
    co-parser) 
	_co_parser $*
	;;
    co-provider) 
	_co_provider $*
	;;
    co-resolver)
	_co_resolver $*
	;;
    co-status)
	_co_status $*
	;;
    co-stop)
	_co_stop $*
	;;
    run-paprocci) 
	_run_paprocci $*
	;;
    run-azprocci) 
	_run_azprocci $*
	;;
    run-broker)
	_run_broker $*
	;;
    run-cops)
	_run_cops $*
	;;
    run-coees)
	_run_coees $*
	;;
    run-comons)
	_run_comons $*
	;;
    run-conets)
	_run_conets $*
	;;
    run-coobas)
	_run_coobas $*
	;;
    run-coss)
	_run_coss $*
	;;
    run-ezvm)
	_run_ezvm $*
	;;
    run-coips)
	_run_coips $*
	;;
    run-fileserver)
	_run_fileserver $*
	;;
    run-onprocci)
	_run_onprocci $*
	;;
    run-osprocci)
	_run_osprocci $*
	;;
    run-osocciprocci)
	_run_osocciprocci $*
	;;
    run-testosocci)
	_run_testosocci $*
	;;
    run-slam)
	_run_slam $*
	;;
    run-copaas)
	_run_copaas $*
	;;
    run-jpaasprocci)
	_run_jpaasprocci $*
	;;
    run-example)
	_run_example $*
	;;
    run-dcprocci)
	_run_dcprocci $*
	;;
    run-parser)
	_run_parser $*
	;;
    run-procci)
	_run_procci $*
	;;
    run-cosched)
	_run_cosched $*
	;;
    run-cosacs)
	_run_cosacs $*
	;;
    run-publisher)
	_run_publisher $*
	;;
    accords-config)
	_accords_config $*
	;;
    *)
	_usage
	exit 1
	;;
esac

exit 0
