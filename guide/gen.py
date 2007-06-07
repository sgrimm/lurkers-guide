#!/usr/bin/python

import sys,string,time,os
sys.path.append('../internal')
import otros

months = [ '', 'January', 'February', 'March', 'April', 'May', 'June', 'July',
		'August', 'September', 'October', 'November', 'December']

do_jpeg = 1
args = sys.argv

for epnum in args[1:]:
	curep = string.atoi(epnum[:3])

	pix = open('../internal/epnames', 'r')
	for i in range(0, curep + 1):
		epname = pix.readline()[:-1]
	pix.close()
	if epname == '':
		epname = 'XXX'

	cureplong = '%03d' % curep

	if len(epnum) == 3:
		which = 'guide'
	else:
		which = 'extra'

	input = open(epnum, 'r')
	mtime = time.localtime(os.stat(epnum)[8]);
	mtime_str = '%s %d, %d' % (months[mtime[1]], mtime[2], mtime[0])

	try:
		os.unlink('#' + epnum + '.html')
	except:
		pass
	try:
		os.rename(epnum + '.html', '#' + epnum + '.html')
	except:
		pass

	output = open(epnum + '.html', 'w')

	output.write("""<html>
<head>
<title>Guide page: """ + '"' + epname + '"')
	if which == 'extra':
		output.write(' (scene in detail)')
	
	output.write("""</title>
<link rev=made href="mailto:koreth@midwinter.com">
<link rel=parent href="../eplist.html">
</head>

<body>

""" + otros.pageheader(curep, which, do_jpeg))

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

	wholefile = input.read()
	input.close()

	rewrite = open('@' + epnum, 'w')
	needsupdate = 0

	pieces = string.splitfields(wholefile, '@@@')
	final = pieces[0]
	rewrite.write(pieces[0])

	for piece in pieces[1:]:
		try:
			timestamp =string.atoi(piece[:string.index(piece, ' ')])
		except:
			timestamp = 0

		if timestamp == 0:
			needsupdate = 1
			timestamp = int(time.time())
			rewrite.write('@@@' + `timestamp`)
			rewrite.write(piece[string.index(piece, ' '):])
		else:
			rewrite.write('@@@' + piece)

		if timestamp >= time.time() - 14 * 60 * 60 * 24:
			timeval = time.localtime(timestamp)
			final = final + ('<strong>[[%d/%d]]</strong>' %
					(timeval[1], timeval[2]));

		final = final + piece[string.index(piece, ' '):]

	rewrite.close()
	if needsupdate:
		try:
			os.unlink('#' + epnum)
		except:
			pass
		os.rename(epnum, '#' + epnum)
		os.rename('@' + epnum, epnum)
	else:
		os.unlink('@' + epnum)

	if do_jpeg:
		final = otros.mungeimages(final, do_jpeg)
	output.write(final)
	output.write('<pre>\n\n</pre>\n' + otros.pagefooter(curep) + """

<h5>
<a href="/lurk/lastmod.html">Last update:</a>
""" + mtime_str + """

</h5>
</body>
</html>
""")

	output.close()

	if do_jpeg:
		print epnum, 'jpeg'
	else:
		print epnum

