#!/usr/bin/python3

import sys,string,time,os,re
sys.path.append('../internal')
import otros
sys.path.insert(0, '.')
import perdir

months = [ '', 'January', 'February', 'March', 'April', 'May', 'June', 'July',
		'August', 'September', 'October', 'November', 'December']

do_jpeg = perdir.get_do_jpeg()
args = sys.argv

for epnum in args[1:]:
#	curep = string.atoi(epnum[:3])
	curep = int(epnum[:3])

	pix = open(perdir.get_episode_names_file(), 'r')
	for i in range(0, curep + 1):
		epname = pix.readline()[:-1]
	pix.close()
	if epname == '':
		epname = 'XXX'

	cureplong = '%03d' % curep

	if len(epnum) == 3:
		which = perdir.get_type()
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

	page_name = perdir.get_page_name_prefix() + ': "' + epname + '"'
	if which == 'extra':
		page_name = page_name + ' (scene in detail)'

	output.write(otros.head(page_name))

	output.write("<body>\n")

	try:
		special = open(epnum + '-head', 'r')
		output.write(special.read())
		special.close()
	except:
		pass

	output.write(otros.pageheader(curep, which, do_jpeg))

	if which == 'extra':
		output.write("""
<pre>

</pre>

""")
	elif perdir.get_type() == 'credits':
		output.write("\n<pre>\n")
	else:
		toc = perdir.get_table_of_contents()
		if len(toc) > 0:
			output.write(toc)
			output.write("\n<p>\n<hr>\n")

	wholefile = input.read()
	input.close()

	rewrite = open('@' + epnum, 'w')
	needsupdate = 0

	pieces = wholefile.split('@@@')
	final = pieces[0]
	rewrite.write(pieces[0])

	for piece in pieces[1:]:
		try:
			timestamp = int(re.split('\\s', piece)[0])
		except:
			timestamp = 0

		if timestamp == 0:
			needsupdate = 1
			timestamp = int(time.time())
			rewrite.write('@@@' + repr(timestamp))
			#rewrite.write(piece[string.index(piece, ' '):])
			rewrite.write(piece[piece.index(' '):])
		else:
			rewrite.write('@@@' + piece)

		if timestamp >= time.time() - 14 * 60 * 60 * 24:
			timeval = time.localtime(timestamp)
			final = final + ('<strong>[[%d/%d]]</strong>' %
					(timeval[1], timeval[2]));

		#final = final + piece[string.index(piece, ' '):]
		final = final + piece[piece.index(' '):]

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

	if perdir.get_type() == 'credits':
		output.write("</pre>\n")

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
		print((epnum, 'jpeg'))
	else:
		print(epnum)

