#!/bin/sh

status () { 

	echo -n "$(date '+%Y-%m-%d %H:%M:%S')"
}

while :; do
	
	xsetroot -name "$(status)"
	sleep 1

done
