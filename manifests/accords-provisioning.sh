#!sh
#
# ----------------------------------------------------
# Parsing of manifests for the Accords Provisioning 
# ----------------------------------------------------
#
./co-parser accords-osprocci
./co-parser accords-onprocci
./co-parser accords-azprocci
./co-parser accords-paprocci
./co-parser accords-dcprocci
./co-parser accords-osocciprocci
./co-parser accords-provisioning
./co-parser sla6
#
# ----------------------------------------------------
# Brokering of an Accords Provisioning Procci Instance
# ----------------------------------------------------
#
./co-broker sla6

