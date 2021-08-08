#!/bin/sh

cowsay -f tux uwm status script

status () {
	echo -n "$(date '+%Y-%m-%d %H:%M') "
	echo -n "Battery: $(acpi | awk '{print $4}' | sed s/,//)"
}

while :; do

	xsetroot -name "$(status)"
	sleep 1

done

