#!/bin/bash
LOGFILE=/tmp/paprocci-test.txt
LOG4JINPUT=/tmp/pa-java-procci-logs.txt
TEMPFILE=/tmp/tempfile.txt
echo Make sure ACCORDS is running.
./co-status
sleep 2
rm $LOGFILE

echo XXXXXXXXXXXX TEST LOG IN $LOGFILE XXXXXXXXXXXXXX

for i in `seq 0 400`
do 

	echo Run $i... >> $LOGFILE
	echo PARSING... >> $LOGFILE
	./co-parser pademo_notepad  &>> $LOGFILE 


	echo OUTPUT in JAVA >> $LOGFILE
	cat  $LOG4JINPUT >> $LOGFILE
	echo "." > $LOG4JINPUT


	echo BROKERING... >> $LOGFILE
	./co-broker pademo_notepad &>> $LOGFILE 

	echo OUTPUT in JAVA >> $LOGFILE
	cat  $LOG4JINPUT >> $LOGFILE
	echo "." > $LOG4JINPUT

	echo Waiting for deployment... >> $LOGFILE
	sleep 10

	echo Freeing... >> $LOGFILE
	./co-command stop service/* &>> $LOGFILE 


	echo OUTPUT in JAVA >> $LOGFILE
	cat  $LOG4JINPUT >> $LOGFILE
	echo "." > $LOG4JINPUT



	echo PROCESSES >> $LOGFILE
	ps aux | head -1 >> $LOGFILE
	ps aux | grep paprocci | grep config >> $LOGFILE

	echo Waiting1of4... >> $LOGFILE
	sleep 50 
	echo Waiting2of4... >> $LOGFILE
	sleep 50 
	echo Waiting3of4... >> $LOGFILE
	sleep 50 
	echo Waiting4of4... >> $LOGFILE
	sleep 50 
done

echo Done the test.

