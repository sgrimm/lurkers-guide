#!/bin/sh
mkdir $1
cd $1
cp ../skel eplist.html
ln -s eplist.html index.html
ln -s ../../guide ../../background ../../misc ../../synops ../../credits .
