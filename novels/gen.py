#!/usr/bin/python

import sys,string,time,os
sys.path.append('../internal')
import otros

months = [ '', 'January', 'February', 'March', 'April', 'May', 'June', 'July',
		'August', 'September', 'October', 'November', 'December']

for epnum in sys.argv[1:]:
	curep = string.atoi(epnum[:3])

	pix = open('../internal/novelnames', 'r')
	for i in range(0, curep + 1):
		epname = pix.readline()[:-1]
	pix.close()
	if epname == '':
		epname = 'XXX'

	cureplong = '%03d' % curep

	which = 'novels'

	input = open(epnum, 'r')
	mtime = time.localtime(os.stat(epnum)[8]);
	mtime_str = '%s %d, %d' % (months[mtime[1]], mtime[2], mtime[0])

	output = open(epnum + '.html', 'w')

	page_name = 'Novel: "' + epname + '"'
	if which == 'extra':
		page_name = page_name + ' (scene in detail)'
	
	output.write(otros.head(page_name))
	output.write("<body>\n")
	output.write(otros.pageheader(curep, which))

	output.write("""

<p>
<b>Contents:</b>
<a href="#BC">Back Cover</a> -
<a href="#SY">Synopsis</a> -
<a href="#UQ">Questions</a> -
<a href="#AN">Analysis</a> -
<a href="#NO">Notes</a> -
<a href="#JS">Author</a>
 
<p>
<hr>

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
