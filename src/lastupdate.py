#!/usr/bin/python

import os,sys,string,time

def dumpfile(out, path):
	try:
		f = open(path, 'r')
		out.write(f.read())
		f.close()
	except:
		pass

def cmptimes(i1, i2):
	if i1[1] < i2[1]:
		return -1
	if i1[1] > i2[1]:
		return 1
	return 0


dirs = [ 'guide', 'synops' ]

months = [ '', 'January', 'February', 'March', 'April', 'May', 'June', 'July',
		'August', 'September', 'October', 'November', 'December']

outfile = open('lastmod-new.html', 'w')

fp = open('internal/epnames', 'r')
epnames = fp.readlines()
fp.close()

#
# Spit out the page header.
#
dumpfile(outfile, 'internal/lu-header')

mdate = time.localtime(time.time())
outfile.write('%02d:%02d Pacific (US) time on %s %d, %d.\n' %
	(mdate[3], mdate[4], months[mdate[1]], mdate[2], mdate[0]))
outfile.write("If that date isn't recent, you probably need to hit your reload button.\n")

for dir in dirs:
	files = []

	dumpfile(outfile, 'internal/lu-header-' + dir)

	for file in os.listdir(dir):
		if file[0] != '0' and file[0] != '1' and file[0] != '5' or file[-4:] == 'html':
			continue
		
		path = dir + '/' + file

		#
		# Has the HTML been generated?
		#
		try:
			os.stat(path + '.html')
		except:
			continue

		#
		# Figure out when the file was touched last.
		#
		mtime = os.stat(path)[8]
		mdate = time.localtime(mtime)
		lastmod = '%02d/%02d/%02d' % (mdate[0] % 100, mdate[1],mdate[2])

		#
		# Figure out the file's episode name.
		#
		epnum = string.atoi(file[:3])
		epname = epnames[epnum][:-1]
		if len(file) != 3:
			epname = epname + ' (scene in detail)'

		#
		# Generate a list entry.
		#
		text = '%03d. %s: <a href="%s">%s</a><br>\n' % (
			epnum, lastmod, path + '.html', epname)

		files.append((epnum, mtime, text))

	#
	# Print a list sorted by date.
	#
	dumpfile(outfile, 'internal/lu-header-date')
	files.sort(cmptimes)
	files.reverse()

	for (epnum, mtime, text) in files:
		outfile.write(text)
	
#	dumpfile(outfile, 'internal/lu-header-epnum')

	#
	# Print a list sorted by episode number.
	#
#	files.sort()

#	for (epnum, mtime, text) in files:
#		outfile.write(text)
	
	dumpfile(outfile, 'internal/lu-footer-epnum')

dumpfile(outfile, 'internal/lu-footer')
outfile.close()

os.unlink('lastmod.html')
os.rename('lastmod-new.html', 'lastmod.html')
