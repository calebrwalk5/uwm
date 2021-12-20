#!/bin/sh

echo "uwm status script"

status () {
	echo -n "$(date '+%Y-%m-%d | %H:%M') | "
	echo -n "Battery: $(acpi | awk '{print $4}' | sed s/,//) | "
	echo -n "RAM $(free -m | grep '^Mem' | awk '{print "used: " $3"MB"}')"
}

while :; do

	xsetroot -name "$(status)"
	sleep 1

done
