#!/bin/sh
for i in [012]*.html; do
	sed -e '1,/<\/pre>/d' < $i > `basename $i .html`

	echo $i
done
