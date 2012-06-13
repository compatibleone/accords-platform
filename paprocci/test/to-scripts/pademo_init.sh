#!/bin/bash
echo Creating needed folders...
mkdir -p rest
mkdir -p service
mkdir -p security
./pademo_clean.sh

./co-start

echo 'Now parse the manifest:'
echo '  ./co-parser pademo_manifest'
echo 'Now perform the brokering of the generated provisioning plan:'
echo '  ./co-broker pademo_manifest'
echo 'Now check the status of your system...'


