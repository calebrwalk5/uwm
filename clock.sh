#!/bin/sh

status () { 

	echo -n "$(date '+%Y-%m-%d | %H:%M:%S') | "
	echo -n "RAM $(free -m | grep '^Mem' | awk '{print "total: " $2 "MB used: " $3"MB"}')"
}

while :; do
	
	xsetroot -name "$(status)"
	sleep 1

done
