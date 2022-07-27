# Figure out which other elements of an episode page are available.
import os, string

# Check for the existence of a file.
def exists(filename):
	try:
		os.stat(filename)
		return 1
	except:
		return 0

#
# Input: integer episode number
# Output: (guide, background, synopsis, credits, prev, next)
#
def getneighbors(which, page = 'guide'):
	whichstr = '%03d' % which
	prev = '%03d' % (which - 1)
	next = '%03d' % (which + 1)

	hasg = exists('../guide/' + whichstr + '.html')
	hasb = exists('../background/' + whichstr + '.shtml')
	hass = exists('../synops/' + whichstr + '.html')
	hasc = exists('../credits/' + whichstr + '.html')
	if exists(prev + '.html') or exists(prev + '.shtml'):
		hasp = 1
	else:
		hasp = 0
	if exists(next + '.html') or exists(next + '.shtml'):
		hasn = 1
	else:
		hasn = 0

	if page == 'comic' or page == 'novels':
		hasb = 0
		hass = 0
		hasc = 0

	return (hasg, hasb, hass, hasc, hasp, hasn)


#
# Generate a trio of navigational buttons for headers and footers.
#
def navbuttons(num, align = 'middle', which = 'guide'):
	(hasg, hasb, hass, hasc, hasp, hasn) = getneighbors(num, which)

	if which == 'background':
		extension = '.shtml'
	else:
		extension = '.html'

	curep = num
	cureplong = '%03d' % (curep)
	prevep = '%03d' % (curep - 1)
	nextep = '%03d' % (curep + 1)

	if which == 'comic' or which == 'novels':
		output = """<a
href="index.html"><img
	align=""" + align + """ border=0
	src="/lurk/nav/dark/gu-index.gif" width=34 height=24
	alt="[Index] "></a>"""

	else:
		output = """<a
href="../eplist.html"><img
	align=""" + align + """ border=0
	src="/lurk/nav/dark/gu-index.gif" width=34 height=24
	alt="[Episode List] "></a>"""


	if hasp:
		output=output+ """<a
href=""" + '"' + prevep + extension + '"' + """><img
	align=""" + align + """ border=0
	src="/lurk/nav/dark/gu-prev.gif" width=33 height=24
	alt="[Previous] "></a>"""

	else:
		output = output + """<img
align=""" + align + """ border=0
	src="/lurk/nav/dark/gu-blanksmall.gif" width=33 height=24
	alt="">"""



	if hasn:
		output=output+ """<a
href=""" + '"' + nextep + extension + '"' + """><img
	align=""" + align + """ border=0
	src="/lurk/nav/dark/gu-next.gif" width=33 height=24
	alt="[Next] "></a>"""

	else:
		output = output + """<img
align=""" + align + """ border=0
	src="/lurk/nav/dark/gu-blanksmall.gif" width=33 height=24
	alt="">"""
	
	return output


#
# Devise a page header based on the current episode number and which page
# we're making.
#
def pageheader(num, page, do_jpeg = 0):
	(hasg, hasb, hass, hasc, hasp, hasn) = getneighbors(num, page)

	curep = num
	cureplong = '%03d' % (curep)
	prevep = '%03d' % (curep - 1)
	nextep = '%03d' % (curep + 1)

	if page == 'comic':
		titleprefix = 'c'
		titlewidth = 422
		picname = 'comic/' + cureplong
	elif page == 'novels':
		titleprefix = 'n'
		titlewidth = 427
		picname = 'novels/' + cureplong
	else:
		titleprefix = 'e'
		titlewidth = 340

		pix = open('../internal/picnames', 'r')
		for i in range(0, curep + 1):
			picname = pix.readline()[:-1]
		pix.close()
		if picname == '':
			picname = 'XXX'
		if picname == 'none':
			picname = 'tv-blank'
		else:
			picname = cureplong + '/' + picname

	curep = repr(curep)
	picwidth = 500 - titlewidth

	output = """
<p align=center nowrap><nobr><img
	align=middle border=0 usemap="#titlebar"
	src="/lurk/nav/titlebar.gif" width=500 height=32
	alt="[Home]">

<br>"""

	if page == 'comic' or page == 'novels':
		output = output + '<a href="/lurk/gif/' + picname + '.jpg">'

	if do_jpeg and picname != 'tv-blank':
		extension = '.jpeg'
	else:
		extension = '.gif'

	output = output + """<img
align=middle width=""" + repr(picwidth) + """ height=120 border=0
	src="/lurk/gif/""" + picname + extension + '" alt="">'

	if page == 'comic' or page == 'novels':
		output = output + '</a>'

	output = output + """<img
align=middle width=""" + repr(titlewidth) + """ height=120
	src="/lurk/gif/titles/""" + titleprefix + curep + """.gif"
	alt="">

<br>"""

	if page == 'guide' or page == 'novels' or page == 'comic':
		output = output + """<img
align=middle border=0
	src="/lurk/nav/lit/gu-guide.gif" width=100 height=24
	alt=" ### GUIDE ### ">"""

	else:
		if hasg:
			output = output + """<a
href="../guide/""" +cureplong+ """.html"><img
	align=middle border=0
	src="/lurk/nav/dark/gu-guide.gif" width=100 height=24
	alt="[Guide] "></a>"""

		else:
			output = output + """<img
align=middle border=0
	src="/lurk/nav/dark/gu-blank.gif" width=100 height=24
	alt="">"""



	if page == 'background':
		output = output + """<img
align=middle border=0
	src="/lurk/nav/lit/gu-background.gif" width=100 height=24
	alt=" ### BACKGROUND ### ">"""

	else:
		if hasb:
			output=output+ """<a
href="../background/""" + cureplong + """.shtml"><img
	align=middle border=0
	src="/lurk/nav/dark/gu-background.gif" width=100 height=24
	alt="[Background] "></a>"""

		else:
			output = output + """<img
align=middle border=0
	src="/lurk/nav/dark/gu-blank.gif" width=100 height=24
	alt="">"""



	if page == 'synopsis':
		output = output + """<img
align=middle border=0
	src="/lurk/nav/lit/gu-synopsis.gif" width=100 height=24
	alt=" ### SYNOPSIS ### ">"""

	else:
		if hass:
			output=output + """<a
href="../synops/""" +cureplong+ """.html"><img
	align=middle border=0
	src="/lurk/nav/dark/gu-synopsis.gif" width=100 height=24
	alt="[Synopsis] "></a>"""

		else:
			output = output + """<img
align=middle border=0
	src="/lurk/nav/dark/gu-blank.gif" width=100 height=24
	alt="">"""



	if page == 'credits':
		output = output + """<img
align=middle border=0
	src="/lurk/nav/lit/gu-credits.gif" width=100 height=24
	alt=" ### CREDITS ### ">"""

	else:
		if hasc:
			output=output+ """<a
href="../credits/""" +cureplong+ """.html"><img
	align=middle border=0
	src="/lurk/nav/dark/gu-credits.gif" width=100 height=24
	alt="[Credits] "></a>"""

		else:
			output = output + """<img
align=middle border=0
	src="/lurk/nav/dark/gu-blank.gif" width=100 height=24
	alt="">"""


	output = output + navbuttons(num, 'middle', page) + "\n</nobr>\n"

	return output


def pagefooter(num, page = 'guide'):
	output = """<p nowrap>
<nobr><a href="/lurk/lurker.html"><img
	align=bottom border=0
	width=64 height=24 src="/lurk/nav/foot-station.gif"
	alt="[Home] "></a><a
href="#"><img
	align=bottom border=0
	width=72 height=13 src="/lurk/nav/foot-pagetop.gif"
	alt="[Top] "></a><a
href="mailto:koreth+lgfeedback@midwinter.com"><img
	align=bottom border=0
	width=80 height=13 src="/lurk/nav/foot-comments.gif"
	alt="[Comments] "></a><img
align=bottom
	width=184 height=13 src="/lurk/nav/foot-bare-short.gif"
	alt="">"""

	output = output + navbuttons(num, 'bottom', page) + '\n</nobr>\n'

	return output


#
# Given an image path, generate the full path to the corresponding file.
#
def normalize_path(path):
	if path[0:6] == '/lurk/':
		curdir = os.getcwd()
		if (not os.path.exists(curdir + '/lurker.html')):
			curdir = curdir + '/..'
		path = os.path.abspath(curdir) + path[5:]
	elif path[0] == '/':
		path = '/home/midwinter/docroot' + path
	return path


#
# Check all the images in a document and substitute JPEGs for GIFs where
# JPEGs exist.
#
def mungeimages(text, do_jpeg):
	#
	# GIF is what's in the source, so do nothing if that's the output
	#
	if do_jpeg == 0:
		return text

	result = ''

	for chunk in text.split('.gif'):
		#filepos = string.rfind(chunk, '"')
		filepos = chunk.rfind('"')
		if filepos < 0:
			result = result + chunk
			break
		
		path = normalize_path(chunk[filepos + 1:])
		if exists(path + '.jpeg'):
			extension = '.jpeg'
		elif exists(path + '.gif'):
			extension = '.gif'
		else:
			extension = ''

		result = result + chunk + extension
	
	return result


#
# Returns the <head> section for a page, including image maps.
#
def head(title):
	result = "<html><head>\n<title>" + title + "</title>\n"
	mapsfile = open('../maps/maps.html', 'r')
	result = result + mapsfile.read()
	mapsfile.close()
	result = result + "</head>\n"

	return result
