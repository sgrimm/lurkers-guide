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

	if len(epnum) > 3:
		cureplong = epnum
	else:
		cureplong = '%03d' % curep

	input = open(cureplong, 'r')
	output = open(cureplong + '.html', 'w')

	mtime = time.localtime(os.stat(cureplong)[8]);
	mtime_str = '%s %d, %d' % (months[mtime[1]], mtime[2], mtime[0])

	output.write("""<html>
<head>
<title>Credits: """ + '"' + epname + '"' + """</title>
<link rev=made href="mailto:koreth@midwinter.com">
<link rel=parent href="../guide/""" + cureplong + """.html">
</head>

<body>

""" + otros.pageheader(curep, 'credits', 1) + '</p>\n\n')

	try:
		special = open(cureplong + '-head', 'r')
		output.write(special.read())
		special.close()
	except:
		pass

	output.write('<pre>\n\n')
	output.write(input.read())

	output.write("""

</pre>
""" + otros.pagefooter(curep) + """

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
