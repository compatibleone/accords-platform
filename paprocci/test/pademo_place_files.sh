#!/bin/bash
INITDIR=`dirname $0`
BASEDIR=$INITDIR/../../

echo PROACTIVE DEMO
echo This script assumes that ACCORDS-PLATFORM is already compiled and installed locally. 
echo Using as root directory: $BASEDIR

echo '   '
echo 1. Copying xsd files locally, so no internet access is required...

sed 's/http:\/\/www.compatibleone.fr\/schemes/file:\/\/\/tmp/g' $BASEDIR/cordstypes.xsd > /tmp/cordstypes.xsd
sed 's/http:\/\/www.compatibleone.fr\/schemes/file:\/\/\/tmp/g' $BASEDIR/cords.xsd > /tmp/cords.xsd
sed 's/http:\/\/www.compatibleone.fr\/schemes/file:\/\/\/tmp/g' $BASEDIR/manifest.xsd > /tmp/manifest.xsd

echo '   '
echo 2. Copying config files...
cp to-scripts/* $BASEDIR/scripts

echo '   '
echo 3. Cleaning environment from previous executions...

cd $BASEDIR/scripts
./pademo_clean.sh

echo '   '
echo '4. NOW PROCEED AS FOLLOWS'
echo ' a. Start up the ProActive Resource Manager and Scheduler' 
echo '    (use PROACTIVE/bin/unix/start_everything.sh script provided).'
echo ' b. Start up some ProActive nodes (better if it happens in in some remote host).' 
echo '    You can use the script provided PROACTIVE/bin/unix/start_node_here.sh .'
echo ' c. Add those nodes to the ProActive Resource Manager (use add_node.sh script '
echo '    and the output of each executed start_node_here.sh).'
echo ' d. Put in place the Java layer of the ProActive procci and compile it.'
echo ' e. Run: '
echo '      $ export PA_PROCCI_PATH=<corresponding path of java-proactive-procci>'
echo '    This will allow the c-layer of ProActive procci call the java-layer.'
echo ' f. Test the Java layer by doing: '
echo '      $ $PA_PROCCI_PATH --user demo --pass demo -v 2 --list-nodes'
echo '    It should list all the nodes available in the RM.'
echo ' g. Put in each ProActive node the cosacs module (running '
echo '    the script pademo_place_cosacs.sh in ACCORDS/paprocci/test).' 
echo '    This will deploy in /home/cosacs/ whatever is needed by COSACS for this example.' 
echo ' h. Go to ACCORDS/scripts'
echo ' i. Run: '
echo '   $ export LD_LIBRARY_PATH=/usr/local/lib'
echo ' j. Run: '
echo '   $ ./pademo_init.sh '
echo '    This will start up ACCORDS. Follow its instructions.'
