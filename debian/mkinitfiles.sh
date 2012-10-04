#!/bin/sh

for i in $(cat debian/services.txt)
do
	sed -e "s/@@service@@/$i/g" < debian/compatibleoneinit.TMPL > debian/compatibleone-$i.init
done

