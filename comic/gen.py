#!/usr/bin/python

import sys,string,time,os
sys.path.append('../internal')
import otros

months = [ '', 'January', 'February', 'March', 'April', 'May', 'June', 'July',
		'August', 'September', 'October', 'November', 'December']

for epnum in sys.argv[1:]:
	curep = string.atoi(epnum[:3])

	pix = open('../internal/comicnames', 'r')
	for i in range(0, curep + 1):
		epname = pix.readline()[:-1]
	pix.close()
	if epname == '':
		epname = 'XXX'

	cureplong = '%03d' % curep

	which = 'comic'

	input = open(epnum, 'r')
	mtime = time.localtime(os.stat(epnum)[8]);
	mtime_str = '%s %d, %d' % (months[mtime[1]], mtime[2], mtime[0])

	output = open(epnum + '.html', 'w')

	output.write("""<html>
<head>
<title>Comic: """ + '"' + epname + '"')
	if which == 'extra':
		output.write(' (scene in detail)')
	
	output.write("""</title>
<link rev=made href="mailto:koreth@midwinter.com">
<link rel=parent href="../eplist.html">
</head>

<body>

""" + otros.pageheader(curep, which))

	if which == 'guide':
		output.write("""

<p>
<b>Contents:</b>
<a href="#OV">Overview</a> -
<a href="#BP">Backplot</a> -
<a href="#UQ">Questions</a> -
<a href="#AN">Analysis</a> -
<a href="#NO">Notes</a> -
<a href="#JS">JMS</a>
 
<p>
<hr>

""")
	else:
		output.write("""
<pre>

</pre>

""")

	output.write(input.read())

	output.write('<pre>\n\n</pre>\n' + otros.pagefooter(curep) + """

<h5>
Last update:
""" + mtime_str + """

</h5>
</body>
</html>
""")

	input.close()
	output.close()

	print epnum
