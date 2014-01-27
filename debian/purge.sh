#!/bin/sh

/etc/init.d/compatibleone stop

dpkg -P $(dpkg -l \*compatibleone\*|grep compatibleone|awk '{print $2}') libcocspi0 libcocspi-dev libcocarrier0 libcocarrier-dev libcocci0 libcocci-dev libcopabr0 libcopabr-dev libcords0 libcords-dev libcorest0 libcorest-dev libcoxml0 libcoxml-dev libocci0 libocci-dev libpubocci0 libpubocci-dev cosacs
rm -rf /var/l{ib,og}/accords /etc/accords
exit 0
