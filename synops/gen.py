#!/usr/bin/python

import sys,string,time,os
sys.path.append('../internal')
import otros

months = [ '', 'January', 'February', 'March', 'April', 'May', 'June', 'July',
		'August', 'September', 'October', 'November', 'December']

for epnum in sys.argv[1:]:
	curep = string.atoi(epnum[:3])

	pix = open('../internal/epnames', 'r')
	for i in range(0, curep + 1):
		epname = pix.readline()[:-1]
	pix.close()
	if epname == '':
		epname = 'XXX'

	cureplong = '%03d' % curep

	which = 'synopsis'

	input = open(epnum, 'r')
	mtime = time.localtime(os.stat(epnum)[8]);
	mtime_str = '%s %d, %d' % (months[mtime[1]], mtime[2], mtime[0])

	output = open(epnum + '.html', 'w')

	output.write("""<html>
<head>
<title>Synopsis: """ + '"' + epname + '"' + """</title>
<link rev=made href="mailto:koreth@midwinter.com">
<link rel=parent href="../eplist.html">
</head>

<body>

""" + otros.pageheader(curep, which, 1) + """

<pre>

</pre>

""")

	output.write(otros.mungeimages(input.read(), 1))

	output.write('\n<pre>\n\n</pre>' +
			otros.pagefooter(curep, 'synopsis') + """

<h5>
<a href="/lurk/lastmod.html">Last update:</a>
""" + mtime_str + """

</h5>
</body>
</html>
""")

	input.close()
	output.close()

	print epnum
