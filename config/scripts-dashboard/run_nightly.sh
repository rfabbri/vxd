#!/bin/sh
LOGFILE=/homelocal/isa/Desktop/dashboard.log
cd /projects/dashboards/vxd/
rm -r /projects/dashboards/vxd/bin-nightly
mkdir /projects/dashboards/vxd/bin-nightly
echo "---------- Building vxd Nightly ----------" >> $LOGFILE
echo "---------- " `date` " ----------" >> $LOGFILE
cd /projects/dashboards/vxd/scripts
ctest -V -S vxd_nightly.cmake >> $LOGFILE
echo "----------  Done ----------" >> $LOGFILE
