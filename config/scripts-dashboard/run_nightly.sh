#!/bin/sh
LOGFILE=/homelocal/isa/Desktop/dashboard.log
cd /projects/dashboards/lemsvxl/
rm -r /projects/dashboards/lemsvxl/bin-nightly
mkdir /projects/dashboards/lemsvxl/bin-nightly
echo "---------- Building lemsvxl Nightly ----------" >> $LOGFILE
echo "---------- " `date` " ----------" >> $LOGFILE
cd /projects/dashboards/lemsvxl/scripts
ctest -V -S lemsvxl_nightly.cmake >> $LOGFILE
echo "----------  Done ----------" >> $LOGFILE
