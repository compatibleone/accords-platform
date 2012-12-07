#!/bin/sh
# Sample commands to generate a certificate for a new OCCI Component.

if [ ! -d "security" ]; then
	echo "security: no such directory"
	exit 1
fi

module=$1

if [ -z $module ]; then
	echo "No module name given"	
	exit 1
fi

export bits=2048
export days=365
export sslconfig=./openssl.cnf
echo "<tls key='security/"${module}"PrivateKey.pem'" > security/${module}Tls.xml
echo "     certificate='security/"${module}"Certificate.pem'" >> security/${module}Tls.xml
echo "     passphrase='"Accords"'" >> security/${module}Tls.xml
echo "     authority='"Certificates.crt"'" >> security/${module}Tls.xml
echo "     authenticate='"1"'" >> security/${module}Tls.xml
echo "     option='"13"' />" >> security/${module}Tls.xml
chmod 0640 security/${module}Tls.xml
echo "Creating certificate for module $1"
if test ! -e $sslconfig; then
	echo "Can not find openSSL config file: $sslconfig"
	exit 1
fi
if [ -z "$nethsm" ]; then
	openssl req -config $sslconfig \
	    	-newkey $bits -x509 -days $days -sha1 \
	    	-keyout security/${module}PrivateKey.pem -out security/${module}Certificate.pem 2> /dev/null
else
	openssl req -engine pkcs11 -keyform e -config $sslconfig \
	    	-newkey $bits -x509 -days $days -sha1 \
	    	-keyout security/${module}PrivateKey.pem -out security/${module}Certificate.pem
fi
chmod 0400 security/${module}PrivateKey.pem
chmod 0444 security/${module}Certificate.pem
