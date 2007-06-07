#!/bin/sh

#
# Run once a night to regenerate guide pages so that the timestamps
# on new items go away after a couple weeks.
#

cd /home/woodstock/koreth/Lurk/guide

./gen.py [0-9]?? | egrep -v '^[0-9][0-9][0-9]$' | egrep -v '^[0-9][0-9][0-9] jpeg$'

for i in [0-9]??; do
	if [ -f \#$i ]; then
		if cmp -s $i \#$i; then
			mv \#$i $i
		else
			echo $i source differed:
			diff \#$i $i
			echo ''
			rm \#$i
		fi
	fi

	if [ -f \#$i.html ]; then
		if cmp -s $i.html \#$i.html; then
			mv \#$i.html $i.html
		else
			echo $i HTML differed
			rm \#$i.html
		fi
	fi
done

