#!/usr/bin/python3

import sys,string,time
sys.path.append('../internal')
import otros

months = [ '', 'January', 'February', 'March', 'April', 'May', 'June', 'July',
		'August', 'September', 'October', 'November', 'December']

for epnum in sys.argv[1:]:
	curep = int(epnum[:3])
	curepno = curep
	cureplong = '%03d' % (curep)
	prevep = '%03d' % (curep - 1)
	nextep = '%03d' % (curep + 1)

	pix = open('../internal/epnames', 'r')
	for i in range(0, curep + 1):
		epname = pix.readline()[:-1]
	pix.close()
	if epname == '':
		epname = 'XXX'

	curep = repr(curep)

	input = open('story.html', 'r')
	output = open(cureplong + '.shtml', 'w')

	page_name = 'Background: "' + epname + '"'
	output.write(otros.head(page_name))
	output.write("<body>\n")
	output.write(otros.pageheader(curepno, 'background', 1))
	output.write("\n<pre>\n\n</pre>\n")

	printing = 1

	#
	# We have concise outlines of some swaths of the story.
	#
	max_summary = 0
	if curepno > 24:
		max_summary = 24
	if curepno > 44:
		max_summary = 44
	if curepno > 66:
		max_summary = 66

	while 1:
		line = input.readline()
		if line == '':
			break
		if line[:8] == '<episode':
			thisep = int(line[-5:-2])
			if thisep > max_summary:
				if not printing:
					output.write('<!--#include virtual="')
					output.write('/lurk/background/sum-' +
						repr(max_summary) + '" -->\n')

					if thisep < curepno:
						output.write('<h1>More ' +
							'Recently...</h1>\n')
					printing = 1
			else:
				printing = 0
			if thisep >= curepno:
				break
		elif line[:6] == 'EPNAME':
			output.write('"' + epname + '."\n')
		elif printing:
			output.write(line)


	mtime = time.localtime(time.time())
	mtime_str = '%s %d, %d' % (months[mtime[1]], mtime[2], mtime[0])
	output.write("""<pre>

</pre>
""" + otros.pagefooter(curepno, 'background') + """

<h5>
Last update:
""" + mtime_str + """

</h5>
</body>
</html>
""")

	input.close()
	output.close()

	print(epnum)
