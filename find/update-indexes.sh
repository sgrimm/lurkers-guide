#!/bin/sh
LD_LIBRARY_PATH=/usr/ucblib:/usr/openwin/lib
export LD_LIBRARY_PATH
PATH=/opt/local/bin:/usr/local/bin:$PATH
export PATH

umask 022

#
# Split the latest Usenet messages up into files.
#
cd /home/koreth/lurk/find/prep-files

./usenet-split.pl /home/woodstock/ftp/pub/Babylon-5/Usenet/latest
./cis-split.pl /home/woodstock/ftp/pub/Babylon-5/CompuServe/latest
./aol-split.pl /home/woodstock/ftp/pub/Babylon-5/America-Online/latest

cd ../indexes

swish-e -c usenet-latest.conf
swish-e -M usenet-static.swish usenet-latest.swish usenet-tmp.swish
mv usenet-tmp.swish usenet.swish
mv usenet-tmp.swish.prop usenet.swish.prop

swish-e -c cs-latest.conf
swish-e -M cs-static.swish cs-latest.swish cs-tmp.swish
mv cs-tmp.swish cs.swish
mv cs-tmp.swish.prop cs.swish.prop

swish-e -c aol-latest.conf
swish-e -M aol-static.swish aol-latest.swish aol-tmp.swish
mv aol-tmp.swish aol.swish
mv aol-tmp.swish.prop aol.swish.prop

swish-e -c guide-update.conf
mv guide-update.swish guide.swish
mv guide-update.swish.prop guide.swish.prop

swish-e -c synops-update.conf
mv synops-update.swish synops.swish
mv synops-update.swish.prop synops.swish.prop

swish-e -c credits-update.conf
mv credits-update.swish credits.swish
mv credits-update.swish.prop credits.swish.prop

