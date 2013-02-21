grep $1 $1.xml | grep -v name | grep -v ${1}s | wc -l
