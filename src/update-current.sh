#!/bin/sh

cd /home/woodstock/hyperion/docs/lurk

thisweek=`date "+%y/%m/%d"`
export thisweek

filename="`egrep '^'$thisweek eplist.html | head -1 | sed 's/.*\\([0-9][0-9][0-9].html\\).*/\\1/'`"

cd guide
rm current.html
ln -s $filename current.html
