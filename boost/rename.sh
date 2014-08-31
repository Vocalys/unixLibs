#!/bin/sh

for f in $( ls -1 *.a ); do
	name=$( echo $f | sed "s/\.a$/\-mt\.a/g" )
	ln -sf $f $name
done
