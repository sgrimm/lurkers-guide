#!/bin/sh
for i in [012]*.html; do
	(
		echo '<h2><a name="OV">Overview</a></h2>'
		sed -e '1,/<h2>/d' -e '/nowrap>$/,$d' < $i
	) > `basename $i .html`

	echo $i
done
